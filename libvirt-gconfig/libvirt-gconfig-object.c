/*
 * libvirt-gconfig-config-object.c: base object for XML configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010 Red Hat
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <string.h>

#include <libxml/relaxng.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"
#include "libvirt-gconfig/libvirt-gconfig-xml-doc.h"

#define GVIR_CONFIG_OBJECT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_OBJECT, GVirConfigObjectPrivate))

struct _GVirConfigObjectPrivate
{
    gchar *schema;

    GVirConfigXmlDoc *doc;
    xmlNodePtr node;
};

G_DEFINE_TYPE(GVirConfigObject, gvir_config_object, G_TYPE_OBJECT);

enum {
    PROP_0,
    PROP_SCHEMA,
    PROP_NODE,
    PROP_DOC
};


static void gvir_xml_generic_error_nop(void *userData G_GNUC_UNUSED,
                                       const char *msg G_GNUC_UNUSED,
                                       ...)
{
}

static void gvir_xml_structured_error_nop(void *userData G_GNUC_UNUSED,
                                          xmlErrorPtr error G_GNUC_UNUSED)
{
}


static void gvir_config_object_get_property(GObject *object,
                                            guint prop_id,
                                            GValue *value,
                                            GParamSpec *pspec)
{
    GVirConfigObject *obj = GVIR_CONFIG_OBJECT(object);
    GVirConfigObjectPrivate *priv = obj->priv;

    switch (prop_id) {
    case PROP_SCHEMA:
        g_value_set_string(value, priv->schema);
        break;

    case PROP_NODE:
        g_value_set_pointer(value, gvir_config_object_get_xml_node(obj));
        break;

    case PROP_DOC:
        g_value_set_object(value, obj->priv->doc);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void gvir_config_object_set_property(GObject *object,
                                            guint prop_id,
                                            const GValue *value,
                                            GParamSpec *pspec)
{
    GVirConfigObject *obj = GVIR_CONFIG_OBJECT(object);
    GVirConfigObjectPrivate *priv = obj->priv;

    switch (prop_id) {
    case PROP_SCHEMA:
        g_free(priv->schema);
        priv->schema = g_value_dup_string(value);
        break;

    case PROP_NODE:
        priv->node =g_value_get_pointer(value);
        break;

    case PROP_DOC:
        obj->priv->doc = g_value_dup_object(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_config_object_finalize(GObject *object)
{
    GVirConfigObject *conn = GVIR_CONFIG_OBJECT(object);
    GVirConfigObjectPrivate *priv = conn->priv;

    g_debug("Finalize GVirConfigObject=%p", conn);

    g_free(priv->schema);

    if (priv->doc != NULL) {
        g_object_unref(G_OBJECT(priv->doc));
        priv->node = NULL; /* node belongs to doc, make sure not to free it */
    }
    if (priv->node != NULL) {
        g_assert(priv->node->doc == NULL);
        xmlFreeNode(priv->node);
    }

    G_OBJECT_CLASS(gvir_config_object_parent_class)->finalize(object);
}


static void gvir_config_object_class_init(GVirConfigObjectClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = gvir_config_object_finalize;
    object_class->get_property = gvir_config_object_get_property;
    object_class->set_property = gvir_config_object_set_property;

    g_object_class_install_property(object_class,
                                    PROP_SCHEMA,
                                    g_param_spec_string("schema",
                                                        "Schema",
                                                        "The doc RNG schema",
                                                        NULL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));

    g_object_class_install_property(object_class,
                                    PROP_NODE,
                                    g_param_spec_pointer("node",
                                                        "XML Node",
                                                        "The XML node this config object corresponds to",
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));

    g_object_class_install_property(object_class,
                                    PROP_DOC,
                                    g_param_spec_object("doc",
                                                        "XML Doc",
                                                        "The XML doc this config object corresponds to",
                                                        GVIR_TYPE_CONFIG_XML_DOC,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));

    g_type_class_add_private(klass, sizeof(GVirConfigObjectPrivate));
}


static void gvir_config_object_init(GVirConfigObject *conn)
{
    g_debug("Init GVirConfigObject=%p", conn);

    conn->priv = GVIR_CONFIG_OBJECT_GET_PRIVATE(conn);
}

void gvir_config_object_validate(GVirConfigObject *config,
                                 GError **err)
{
    GVirConfigObjectPrivate *priv = config->priv;
    xmlRelaxNGParserCtxtPtr rngParser = NULL;
    xmlRelaxNGPtr rng = NULL;
    xmlRelaxNGValidCtxtPtr rngValid = NULL;

    xmlSetGenericErrorFunc(NULL, gvir_xml_generic_error_nop);
    xmlSetStructuredErrorFunc(NULL, gvir_xml_structured_error_nop);

    if (!priv->node) {
        *err = gvir_xml_error_new(GVIR_CONFIG_OBJECT_ERROR,
                                  0,
                                  "%s",
                                  "No XML document associated with this config object");
        return;
    }

    rngParser = xmlRelaxNGNewParserCtxt(priv->schema);
    if (!rngParser) {
        *err = gvir_xml_error_new(GVIR_CONFIG_OBJECT_ERROR,
                                  0,
                                  "Unable to create RNG parser for %s",
                                  priv->schema);
        return;
    }

    rng = xmlRelaxNGParse(rngParser);
    if (!rng) {
        *err = gvir_xml_error_new(GVIR_CONFIG_OBJECT_ERROR,
                                  0,
                                  "Unable to parse RNG %s",
                                  priv->schema);
        xmlRelaxNGFreeParserCtxt(rngParser);
        return;
    }
    xmlRelaxNGFreeParserCtxt(rngParser);

    rngValid = xmlRelaxNGNewValidCtxt(rng);
    if (!rngValid) {
        *err = gvir_xml_error_new(GVIR_CONFIG_OBJECT_ERROR,
                                  0,
                                  "Unable to create RNG validation context %s",
                                  priv->schema);
        xmlRelaxNGFree(rng);
        return;
    }

    if (xmlRelaxNGValidateDoc(rngValid, priv->node->doc) != 0) {
        *err = gvir_xml_error_new(GVIR_CONFIG_OBJECT_ERROR,
                                  0,
                                  "%s",
                                  "Unable to validate doc");
        xmlRelaxNGFreeValidCtxt(rngValid);
        xmlRelaxNGFree(rng);
        return;
    }

    xmlRelaxNGFreeValidCtxt(rngValid);
    xmlRelaxNGFree(rng);
}

gchar *gvir_config_object_to_xml(GVirConfigObject *config)
{
    xmlChar *doc;
    int size;
    xmlNodePtr node;
    gchar *output_doc;

    node = gvir_config_object_get_xml_node(config);
    if (node == NULL)
        return NULL;

    xmlDocDumpMemory(node->doc, &doc, &size);

    output_doc = g_strdup((gchar *)doc);
    xmlFree(doc);

    return output_doc;
}

const gchar *gvir_config_object_get_schema(GVirConfigObject *config)
{
    GVirConfigObjectPrivate *priv = config->priv;
    return priv->schema;
}

/* FIXME: will we always have one xmlNode per GConfig object? */
/* FIXME: need to return the right node from subclasses */
/* NB: the xmlNodePtr must not be freed by the caller */
G_GNUC_INTERNAL xmlNodePtr
gvir_config_object_get_xml_node(GVirConfigObject *config)
{
    return config->priv->node;
}

G_GNUC_INTERNAL char *
gvir_config_object_get_node_content(GVirConfigObject *object,
                                    const char *node_name)
{
    xmlNodePtr node;

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(object));
    if (node == NULL)
        return NULL;

    return gvir_config_xml_get_child_element_content_glib(node, node_name);
}

static xmlNodePtr
gvir_config_object_set_child_internal(GVirConfigObject *object,
                                      xmlNodePtr child,
                                      gboolean overwrite)
{
    xmlNodePtr parent_node;
    xmlNodePtr old_node;

    parent_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(object));
    g_return_val_if_fail (parent_node != NULL, NULL);

    old_node = gvir_config_xml_get_element(parent_node, child->name, NULL);
    /* FIXME: should we make sure there are no multiple occurrences
     * of this node?
     */
    if (old_node) {
        if (overwrite) {
            old_node = xmlReplaceNode(old_node, child);
            xmlFreeNode(old_node);
        } else {
            return old_node;
        }
    } else {
        xmlAddChild(parent_node, child);
    }

    return NULL;
}

G_GNUC_INTERNAL void
gvir_config_object_set_child(GVirConfigObject *object, xmlNodePtr child)
{
    gvir_config_object_set_child_internal(object, child, TRUE);
}

G_GNUC_INTERNAL xmlNodePtr
gvir_config_object_add_child(GVirConfigObject *object,
                             const char *child_name)
{
    xmlNodePtr new_node;
    xmlNodePtr old_node;

    g_return_val_if_fail(GVIR_IS_CONFIG_OBJECT(object), NULL);

    new_node = xmlNewDocNode(NULL, NULL, (xmlChar *)child_name, NULL);
    old_node = gvir_config_object_set_child_internal(object, new_node,
                                                     FALSE);
    if (old_node != NULL) {
        xmlFreeNode(new_node);
        return old_node;
    }

    return new_node;
}

G_GNUC_INTERNAL xmlNodePtr
gvir_config_object_replace_child(GVirConfigObject *object,
                                 const char *child_name)
{
    xmlNodePtr new_node;

    g_return_val_if_fail(GVIR_IS_CONFIG_OBJECT(object), NULL);
    g_return_val_if_fail(child_name != NULL, NULL);

    new_node = xmlNewDocNode(NULL, NULL, (xmlChar *)child_name, NULL);
    gvir_config_object_set_child_internal(object, new_node, TRUE);

    return new_node;
}

G_GNUC_INTERNAL void
gvir_config_object_set_node_content(GVirConfigObject *object,
                                    const char *node_name,
                                    const char *value)
{
    xmlNodePtr node;
    xmlChar *encoded_data;

    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(object));
    g_return_if_fail(node_name != NULL);
    g_return_if_fail(value != NULL);

    node = gvir_config_object_replace_child(object, node_name);
    g_return_if_fail(node != NULL);
    encoded_data = xmlEncodeEntitiesReentrant(node->doc,
                                              (xmlChar *)value);
    xmlNodeSetContent(node, encoded_data);
    xmlFree(encoded_data);
}

G_GNUC_INTERNAL void
gvir_config_object_set_node_content_uint64(GVirConfigObject *object,
                                           const char *node_name,
                                           guint64 value)
{
    char *str;

    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(object));
    g_return_if_fail(node_name != NULL);

    str = g_strdup_printf("%"G_GUINT64_FORMAT, value);
    gvir_config_object_set_node_content(object, node_name, str);
    g_free(str);
}

/* FIXME: how to notify of errors/node not found? */
G_GNUC_INTERNAL guint64
gvir_config_object_get_node_content_uint64(GVirConfigObject *object,
                                           const char *node_name)
{
    xmlNodePtr node;
    xmlChar *str;
    guint64 value;

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(object));
    if (node == NULL)
        return 0;

    str = gvir_config_xml_get_child_element_content(node, node_name);
    if (!str)
        return 0;

    value = g_ascii_strtoull((char *)str, NULL, 0);
    xmlFree(str);

    return value;
}

GVirConfigObject *gvir_config_object_new_from_xml(GType type,
                                                  const char *root_name,
                                                  const char *schema,
                                                  const gchar *xml,
                                                  GError **error)
{
    GVirConfigObject *object;
    GVirConfigXmlDoc *doc;
    xmlNodePtr node;

    node = gvir_config_xml_parse(xml, root_name, error);
    if ((error != NULL) && (*error != NULL))
        return NULL;
    doc = gvir_config_xml_doc_new(node->doc);
    object = GVIR_CONFIG_OBJECT(g_object_new(type,
                                             "doc", doc,
                                             "node", node,
                                             "schema", schema,
                                             NULL));
    g_object_unref(G_OBJECT(doc));

    return object;
}

GVirConfigObject *gvir_config_object_new(GType type,
                                         const char *root_name,
                                         const char *schema)
{
    GVirConfigObject *object;
    GVirConfigXmlDoc *doc;
    xmlDocPtr xml_doc;
    xmlNodePtr node;

    doc = gvir_config_xml_doc_new(NULL);
    g_object_get(G_OBJECT(doc), "doc", &xml_doc, NULL);
    g_assert(xml_doc != NULL);
    node = xmlNewDocNode(xml_doc, NULL, (xmlChar *)root_name, NULL);
    xmlDocSetRootElement(xml_doc, node);
    object = GVIR_CONFIG_OBJECT(g_object_new(type,
                                             "doc", doc,
                                             "node", node,
                                             "schema", schema,
                                             NULL));

    g_object_unref(G_OBJECT(doc));

    return object;
}
