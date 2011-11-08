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


//extern gboolean debugFlag;
gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_CONFIG_OBJECT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_OBJECT, GVirConfigObjectPrivate))

struct _GVirConfigObjectPrivate
{
    gchar *schema;

    xmlNodePtr node;
};

G_DEFINE_ABSTRACT_TYPE(GVirConfigObject, gvir_config_object, G_TYPE_OBJECT);

enum {
    PROP_0,
    PROP_SCHEMA,
    PROP_NODE
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
    GVirConfigObject *conn = GVIR_CONFIG_OBJECT(object);
    GVirConfigObjectPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_SCHEMA:
        g_value_set_string(value, priv->schema);
        break;

    case PROP_NODE:
        g_value_set_pointer(value, gvir_config_object_get_xml_node(conn));
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
    GVirConfigObject *conn = GVIR_CONFIG_OBJECT(object);
    GVirConfigObjectPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_SCHEMA:
        g_free(priv->schema);
        priv->schema = g_value_dup_string(value);
        break;

    case PROP_NODE: {
        xmlNodePtr node;
        node = g_value_get_pointer(value);
        if ((priv->node != NULL)
             && (priv->node->doc != NULL)
             && (priv->node->doc != node->doc)) {
            xmlFreeDoc(priv->node->doc);
        }
        priv->node = node;
        break;
    }

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_config_object_finalize(GObject *object)
{
    GVirConfigObject *conn = GVIR_CONFIG_OBJECT(object);
    GVirConfigObjectPrivate *priv = conn->priv;

    DEBUG("Finalize GVirConfigObject=%p", conn);

    g_free(priv->schema);

    /* FIXME: all objects describing a given XML document will share the
     * same document so we can't destroy it here like this, we need some
     * refcounting to know when to destroy it.
     */
    if (priv->node)
        xmlFreeDoc(priv->node->doc);

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
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));

    g_object_class_install_property(object_class,
                                    PROP_NODE,
                                    g_param_spec_pointer("node",
                                                        "XML Node",
                                                        "The XML node this config object corresponds to",
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));

    g_type_class_add_private(klass, sizeof(GVirConfigObjectPrivate));
}


static void gvir_config_object_init(GVirConfigObject *conn)
{
    GVirConfigObjectPrivate *priv;

    DEBUG("Init GVirConfigObject=%p", conn);

    priv = conn->priv = GVIR_CONFIG_OBJECT_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
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
xmlNodePtr gvir_config_object_get_xml_node(GVirConfigObject *config)
{
    return config->priv->node;
}

char *gvir_config_object_get_node_content(GVirConfigObject *object,
                                          const char *node_name)
{
    xmlNodePtr node;

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(object));
    if (node == NULL)
        return NULL;

    return gvir_config_xml_get_child_element_content_glib(node, node_name);
}

/* FIXME: if there are multiple nodes with the same name, this function
 * won't behave as expected. Should we get rid of the duplicated node names
 * here?
 */
void gvir_config_object_set_node_content(GVirConfigObject *object,
                                         const char *node_name,
                                         const char *value)
{
    xmlNodePtr parent_node;
    xmlNodePtr old_node;
    xmlNodePtr new_node;
    xmlChar *encoded_name;

    parent_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(object));
    encoded_name = xmlEncodeEntitiesReentrant(parent_node->doc,
                                              (xmlChar *)value);
    new_node = xmlNewDocNode(parent_node->doc, NULL,
                             (xmlChar *)node_name, encoded_name);
    xmlFree(encoded_name);

    old_node = gvir_config_xml_get_element(parent_node, node_name, NULL);
    if (old_node) {
        old_node = xmlReplaceNode(old_node, new_node);
        xmlFreeNode(old_node);
    } else {
        xmlAddChild(parent_node, new_node);
    }
}

/* FIXME: how to notify of errors/node not found? */
guint64 gvir_config_object_get_node_content_uint64(GVirConfigObject *object,
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


void gvir_config_object_set_node_content_uint64(GVirConfigObject *object,
                                                const char *node_name,
                                                guint64 value)
{
    char *str;
    str = g_strdup_printf("%"G_GUINT64_FORMAT, value);
    gvir_config_object_set_node_content(object, node_name, str);
    g_free(str);
}

GVirConfigObject *gvir_config_object_new_from_xml(GType type,
                                                  const char *root_name,
                                                  const char *schema,
                                                  const gchar *xml,
                                                  GError **error)
{
    xmlNodePtr node;

    node = gvir_config_xml_parse(xml, root_name, error);
    if ((error != NULL) && (*error != NULL))
        return NULL;
    return GVIR_CONFIG_OBJECT(g_object_new(type,
                                           "node", node,
                                           "schema", schema,
                                           NULL));
}

GVirConfigObject *gvir_config_object_new(GType type,
                                         const char *root_name,
                                         const char *schema)
{
    xmlDocPtr doc;
    xmlNodePtr node;

    doc = xmlNewDoc((xmlChar *)"1.0");
    node = xmlNewDocNode(doc, NULL, (xmlChar *)root_name, NULL);
    xmlDocSetRootElement(doc, node);
    return GVIR_CONFIG_OBJECT(g_object_new(type,
                                           "node", node,
                                           "schema", schema,
                                           NULL));
}
