/*
 * libvirt-gconfig-object.c: base object for XML configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010-2011 Red Hat, Inc.
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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <string.h>

#include <libxml/relaxng.h>
#include <glib/gi18n-lib.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_OBJECT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_OBJECT, GVirConfigObjectPrivate))

struct _GVirConfigObjectPrivate
{
    gchar *schema;

    GVirConfigXmlDoc *doc;
    xmlNodePtr node;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigObject, gvir_config_object, G_TYPE_OBJECT);

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
    GVirConfigObject *gvir_object = GVIR_CONFIG_OBJECT(object);
    GVirConfigObjectPrivate *priv = gvir_object->priv;

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
                                                        GVIR_CONFIG_TYPE_XML_DOC,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));
}


static void gvir_config_object_init(GVirConfigObject *object)
{
    object->priv = GVIR_CONFIG_OBJECT_GET_PRIVATE(object);
}

void gvir_config_object_validate(GVirConfigObject *config,
                                 GError **err)
{
    GVirConfigObjectPrivate *priv;
    xmlRelaxNGParserCtxtPtr rngParser = NULL;
    xmlRelaxNGPtr rng = NULL;
    xmlRelaxNGValidCtxtPtr rngValid = NULL;

    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(config));
    g_return_if_fail(err == NULL || *err == NULL);

    priv = config->priv;

    xmlSetGenericErrorFunc(NULL, gvir_xml_generic_error_nop);
    xmlSetStructuredErrorFunc(NULL, gvir_xml_structured_error_nop);

    if (!priv->node) {
        gvir_config_set_error_literal(err,
                                      GVIR_CONFIG_OBJECT_ERROR,
                                      0,
                                      _("No XML document associated with this config object"));
        return;
    }

    if (!priv->schema) {
        gvir_config_set_error_literal(err,
                                      GVIR_CONFIG_OBJECT_ERROR,
                                      0,
                                      _("No XML schema associated with this config object"));
        return;
    }

    rngParser = xmlRelaxNGNewParserCtxt(priv->schema);
    if (!rngParser) {
        gvir_config_set_error(err,
                              GVIR_CONFIG_OBJECT_ERROR,
                              0,
                              _("Unable to create RNG parser for %s"),
                              priv->schema);
        return;
    }

    rng = xmlRelaxNGParse(rngParser);
    if (!rng) {
        gvir_config_set_error(err,
                              GVIR_CONFIG_OBJECT_ERROR,
                              0,
                              _("Unable to parse RNG %s"),
                              priv->schema);
        xmlRelaxNGFreeParserCtxt(rngParser);
        return;
    }
    xmlRelaxNGFreeParserCtxt(rngParser);

    rngValid = xmlRelaxNGNewValidCtxt(rng);
    if (!rngValid) {
        gvir_config_set_error(err,
                              GVIR_CONFIG_OBJECT_ERROR,
                              0,
                              _("Unable to create RNG validation context %s"),
                              priv->schema);
        xmlRelaxNGFree(rng);
        return;
    }

    if (xmlRelaxNGValidateDoc(rngValid, priv->node->doc) != 0) {
        gvir_config_set_error_literal(err,
                                      GVIR_CONFIG_OBJECT_ERROR,
                                      0,
                                      _("Unable to validate doc"));
        xmlRelaxNGFreeValidCtxt(rngValid);
        xmlRelaxNGFree(rng);
        return;
    }

    xmlRelaxNGFreeValidCtxt(rngValid);
    xmlRelaxNGFree(rng);
}

gchar *gvir_config_object_to_xml(GVirConfigObject *config)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_OBJECT(config), NULL);

    return gvir_config_xml_node_to_string(config->priv->node);
}

const gchar *gvir_config_object_get_schema(GVirConfigObject *config)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_OBJECT(config), NULL);

    return config->priv->schema;
}


G_GNUC_INTERNAL GVirConfigXmlDoc *
gvir_config_object_get_xml_doc(GVirConfigObject *config)
{
    return config->priv->doc;
}


/* FIXME: will we always have one xmlNode per GConfig object? */
/* FIXME: need to return the right node from subclasses */
/* NB: the xmlNodePtr must not be freed by the caller */
G_GNUC_INTERNAL xmlNodePtr
gvir_config_object_get_xml_node(GVirConfigObject *config)
{
    return config->priv->node;
}

G_GNUC_INTERNAL const char *
gvir_config_object_get_node_content(GVirConfigObject *object,
                                    const char *node_name)
{
    xmlNodePtr node;

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(object));
    if (node == NULL)
        return NULL;

    return gvir_config_xml_get_child_element_content(node, node_name);
}

G_GNUC_INTERNAL const char *
gvir_config_object_get_attribute(GVirConfigObject *object,
                                 const char *node_name,
                                 const char *attr_name)
{
    xmlNodePtr node;

    g_return_val_if_fail(attr_name != NULL, NULL);

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(object));
    if (node == NULL)
        return NULL;

    if (node_name != NULL) {
        node = gvir_config_xml_get_element(node, node_name, NULL);
        if (node == NULL)
            return NULL;
    }

    return gvir_config_xml_get_attribute_content(node, attr_name);
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

G_GNUC_INTERNAL void
gvir_config_object_foreach_child(GVirConfigObject *object,
                                 const char *parent_name,
                                 GVirConfigXmlNodeIterator iter_func,
                                 gpointer opaque)
{
    xmlNodePtr root_node;
    xmlNodePtr node;

    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(object));

    root_node = gvir_config_object_get_xml_node(object);
    g_return_if_fail(root_node != NULL);

    node = gvir_config_xml_get_element(root_node, parent_name, NULL);
    if (node == NULL)
        return;

    gvir_config_xml_foreach_child(node, iter_func, opaque);
}

G_GNUC_INTERNAL GVirConfigObject *
gvir_config_object_add_child(GVirConfigObject *object,
                             const char *child_name)
{
    xmlNodePtr new_node;
    xmlNodePtr old_node;

    g_return_val_if_fail(GVIR_CONFIG_IS_OBJECT(object), NULL);
    g_return_val_if_fail(child_name != NULL, NULL);

    new_node = xmlNewDocNode(NULL, NULL, (xmlChar *)child_name, NULL);
    old_node = gvir_config_object_set_child_internal(object, new_node,
                                                     FALSE);
    if (old_node != NULL) {
        xmlFreeNode(new_node);
        return GVIR_CONFIG_OBJECT(g_object_new(GVIR_CONFIG_TYPE_OBJECT,
                                               "doc", object->priv->doc,
                                               "node", old_node,
                                               NULL));
    }

    return GVIR_CONFIG_OBJECT(g_object_new(GVIR_CONFIG_TYPE_OBJECT,
                                           "doc", object->priv->doc,
                                           "node", new_node,
                                           NULL));
}

G_GNUC_INTERNAL void
gvir_config_object_add_child_with_attribute(GVirConfigObject *object,
                                            const char *child_name,
                                            const char *attr_name,
                                            const char *attr_value)
{
    GVirConfigObject *child;

    child = gvir_config_object_add_child(object, child_name);
    gvir_config_object_set_attribute(child, attr_name, attr_value, NULL);
    g_object_unref(G_OBJECT(child));
}


void gvir_config_object_add_child_with_attribute_enum(GVirConfigObject *object,
                                                      const char *child_name,
                                                      const char *attr_name,
                                                      GType attr_type,
                                                      unsigned int attr_value)
{
    GVirConfigObject *child;

    child = gvir_config_object_add_child(object, child_name);
    gvir_config_object_set_attribute_with_type(child, attr_name, attr_type, attr_value, NULL);
    g_object_unref(G_OBJECT(child));
}


G_GNUC_INTERNAL GVirConfigObject *
gvir_config_object_replace_child(GVirConfigObject *object,
                                 const char *child_name)
{
    xmlNodePtr new_node;

    g_return_val_if_fail(GVIR_CONFIG_IS_OBJECT(object), NULL);
    g_return_val_if_fail(child_name != NULL, NULL);

    new_node = xmlNewDocNode(NULL, NULL, (xmlChar *)child_name, NULL);
    gvir_config_object_set_child_internal(object, new_node, TRUE);

    return GVIR_CONFIG_OBJECT(g_object_new(GVIR_CONFIG_TYPE_OBJECT,
                                           "doc", object->priv->doc,
                                           "node", new_node,
                                           NULL));
}

G_GNUC_INTERNAL void
gvir_config_object_replace_child_with_attribute(GVirConfigObject *object,
                                                const char *child_name,
                                                const char *attr_name,
                                                const char *attr_value)
{
    GVirConfigObject *child;

    child = gvir_config_object_replace_child(object, child_name);
    gvir_config_object_set_attribute(child, attr_name, attr_value, NULL);
    g_object_unref(G_OBJECT(child));
}

G_GNUC_INTERNAL void
gvir_config_object_replace_child_with_attribute_enum(GVirConfigObject *object,
                                                     const char *child_name,
                                                     const char *attr_name,
                                                     GType attr_type,
                                                     unsigned int attr_value)
{
    GVirConfigObject *child;

    child = gvir_config_object_replace_child(object, child_name);
    gvir_config_object_set_attribute_with_type(child, attr_name, attr_type, attr_value, NULL);
    g_object_unref(G_OBJECT(child));
}

struct NodeMatch {
    const char *name;
    const char *ns;
};

static gboolean maybe_unlink_node(xmlNodePtr node, void *opaque)
{
    gboolean dounlink = TRUE;
    struct NodeMatch *match = (struct NodeMatch *)opaque;

    if (match->ns != NULL) {
        dounlink = dounlink && (g_strcmp0(match->ns, (char *)node->ns->href) == 0);
    }

    if (match->name != NULL) {
        dounlink = dounlink && (g_strcmp0(match->name, (char *)node->name) == 0);
    }
    if (dounlink) {
        xmlUnlinkNode(node);
        xmlFreeNode(node);
    }

    return dounlink;
}

static gboolean remove_oneshot(xmlNodePtr node, gpointer opaque)
{
    return !maybe_unlink_node(node, opaque);
}

G_GNUC_INTERNAL void
gvir_config_object_delete_child(GVirConfigObject *object,
                                const char *child_name,
                                const char *ns_href)
{
    struct NodeMatch match;

    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(object));

    match.name = child_name;
    match.ns = ns_href;
    gvir_config_object_foreach_child(object, NULL, remove_oneshot, &match);
}

static gboolean remove_always(xmlNodePtr node, gpointer opaque)
{
    maybe_unlink_node(node, opaque);

    return TRUE;
}

G_GNUC_INTERNAL void
gvir_config_object_delete_children(GVirConfigObject *object,
                                   const char *child_name,
                                   const char *ns_href)
{
    struct NodeMatch match;

    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(object));

    match.name = child_name;
    match.ns = ns_href;

    gvir_config_object_foreach_child(object, NULL, remove_always, &match);
}

G_GNUC_INTERNAL void
gvir_config_object_set_node_content(GVirConfigObject *object,
                                    const char *node_name,
                                    const char *value)
{
    xmlChar *encoded_data;
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(object));

    if (value == NULL) {
        gvir_config_object_delete_child(object, node_name, NULL);

        return;
    }

    if (node_name != NULL) {
        node = gvir_config_object_replace_child(object, node_name);
        g_return_if_fail(node != NULL);
    } else {
        node = g_object_ref(object);
    }
    encoded_data = xmlEncodeEntitiesReentrant(node->priv->node->doc,
                                              (xmlChar *)value);
    xmlNodeSetContent(node->priv->node, encoded_data);
    xmlFree(encoded_data);
    g_object_unref(G_OBJECT(node));
}

G_GNUC_INTERNAL void
gvir_config_object_set_node_content_uint64(GVirConfigObject *object,
                                           const char *node_name,
                                           guint64 value)
{
    char *str;

    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(object));

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
    const char *str;
    guint64 value;

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(object));
    if (node == NULL)
        return 0;

    str = gvir_config_xml_get_child_element_content(node, node_name);
    if (!str)
        return 0;

    value = g_ascii_strtoull(str, NULL, 0);

    return value;
}

G_GNUC_INTERNAL gint
gvir_config_object_get_node_content_genum(GVirConfigObject *object,
                                          const char *node_name,
                                          GType enum_type,
                                          gint default_value)
{
    xmlNodePtr node;
    const char *str;
    gint value;

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(object));
    if (node == NULL)
        return default_value;

    str = gvir_config_xml_get_child_element_content(node, node_name);
    if (!str)
        return default_value;

    value = gvir_config_genum_get_value(enum_type, str, default_value);

    return value;
}

G_GNUC_INTERNAL gint
gvir_config_object_get_attribute_genum(GVirConfigObject *object,
                                       const char *node_name,
                                       const char *attr_name,
                                       GType enum_type,
                                       gint default_value)
{
    xmlNodePtr node;
    const char *attr_val;
    gint value;

    g_return_val_if_fail(attr_name != NULL, default_value);

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(object));
    if (node == NULL)
        return default_value;

    if (node_name != NULL) {
        node = gvir_config_xml_get_element(node, node_name, NULL);
        if (node == NULL)
            return default_value;
    }

    attr_val = gvir_config_xml_get_attribute_content(node, attr_name);
    if (attr_val == NULL)
        return default_value;

    value = gvir_config_genum_get_value(enum_type, attr_val,
                                        default_value);

    return value;
}

G_GNUC_INTERNAL guint64
gvir_config_object_get_attribute_uint64(GVirConfigObject *object,
                                        const char *node_name,
                                        const char *attr_name,
                                        guint64 default_value)
{
    const char *str;

    str = gvir_config_object_get_attribute(object, node_name, attr_name);
    if (str == NULL)
        return default_value;

    return g_ascii_strtoull(str, NULL, 0);
}


G_GNUC_INTERNAL gboolean
gvir_config_object_get_attribute_boolean(GVirConfigObject *object,
                                         const char *node_name,
                                         const char *attr_name,
                                         gboolean default_value)
{
    const char *str;

    str = gvir_config_object_get_attribute(object, node_name, attr_name);
    if (g_strcmp0(str, "yes") == 0) {
        return TRUE;
    } else if (g_strcmp0(str, "no") == 0) {
        return FALSE;
    } else {
        return default_value;
    }
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
    GError *tmp_error = NULL;

    node = gvir_config_xml_parse(xml, root_name, &tmp_error);
    if (tmp_error != NULL) {
        g_propagate_error(error, tmp_error);
        return NULL;
    }
    doc = gvir_config_xml_doc_new(node->doc);
    object = GVIR_CONFIG_OBJECT(g_object_new(type,
                                             "doc", doc,
                                             "node", node,
                                             "schema", schema,
                                             NULL));
    g_object_unref(G_OBJECT(doc));

    return object;
}

G_GNUC_INTERNAL GVirConfigObject *
gvir_config_object_new_from_tree(GType type, GVirConfigXmlDoc *doc,
                                 const char *schema, xmlNodePtr tree)
{
    g_return_val_if_fail(g_type_is_a(type, GVIR_CONFIG_TYPE_OBJECT), NULL);
    g_return_val_if_fail(GVIR_CONFIG_IS_XML_DOC(doc), NULL);
    g_return_val_if_fail(tree != NULL, NULL);

    return GVIR_CONFIG_OBJECT(g_object_new(type,
                                           "doc", doc,
                                           "node", tree,
                                           "schema", schema,
                                           NULL));
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

G_GNUC_INTERNAL void
gvir_config_object_set_attribute(GVirConfigObject *object, ...)
{
    xmlDocPtr doc;
    va_list args;

    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(object));

    g_object_get(G_OBJECT(object->priv->doc), "doc", &doc, NULL);
    va_start(args, object);
    while (TRUE) {
        const char *name;
        const char *value;

        name = va_arg(args, const char *);
        if (name == NULL) {
            break;
        }
        gvir_config_object_remove_attribute(object, name);
        value = va_arg(args, const char *);
        if (value == NULL) {
            g_warn_if_reached();
            break;
        }
        xmlNewProp(object->priv->node, (xmlChar *)name, (xmlChar *)value);
    }
    va_end(args);
}

G_GNUC_INTERNAL void
gvir_config_object_set_attribute_with_type(GVirConfigObject *object, ...)
{
    va_list args;

    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(object));

    va_start(args, object);
    while (TRUE) {
        const char *name;
        GType attr_type;
        char *str;


        name = va_arg(args, const char *);
        if (name == NULL) {
            break;
        }
        gvir_config_object_remove_attribute(object, name);

        attr_type = va_arg(args, GType);
        if (G_TYPE_IS_ENUM(attr_type)) {
            int val;
            const char *enum_str;
            val = va_arg(args, int);
            enum_str = gvir_config_genum_get_nick(attr_type, val);
            if (enum_str != NULL) {
                str = g_strdup(enum_str);
            } else {
                str = NULL;
            }
        } else switch (attr_type) {
            case G_TYPE_UINT64: {
                guint64 val;
                val = va_arg(args, guint64);
                str = g_strdup_printf("%"G_GUINT64_FORMAT, val);
                break;
            }
            case G_TYPE_UINT: {
                guint val;
                val = va_arg(args, guint);
                str = g_strdup_printf("%u", val);
                break;
            }
            case G_TYPE_INT: {
                gint val;
                val = va_arg(args, gint);
                str = g_strdup_printf("%d", val);
                break;
            }
            case G_TYPE_STRING:
                str = va_arg(args, char *);
                xmlNewProp(object->priv->node, (xmlChar *)name, (xmlChar *)str);
                str = NULL;
                break;
            case G_TYPE_BOOLEAN: {
                gboolean val;
                val = va_arg(args, gboolean);
                str = g_strdup_printf("%s", val?"yes":"no");
                break;
            }
            default:
                g_warning("Unhandled type: %s", g_type_name(attr_type));
                g_assert_not_reached();
        }

        if (str != NULL) {
            xmlNewProp(object->priv->node, (xmlChar *)name, (xmlChar *)str);
            g_free(str);
        }
    }
    va_end(args);
}

static void
gvir_config_object_attach(GVirConfigObject *parent, GVirConfigObject *child, gboolean replace)
{
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(parent));
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(child));

    if (replace) {
        gvir_config_object_delete_children(parent,
                                           (char *)child->priv->node->name,
                                           NULL);
    }
    xmlUnlinkNode(child->priv->node);
    xmlAddChild(parent->priv->node, child->priv->node);
    if (child->priv->doc != NULL) {
        g_object_unref(G_OBJECT(child->priv->doc));
        child->priv->doc = NULL;
    }
    if (parent->priv->doc != NULL) {
        child->priv->doc = g_object_ref(parent->priv->doc);
    }
}

G_GNUC_INTERNAL void
gvir_config_object_attach_replace(GVirConfigObject *parent,
                                  const char *child_name,
                                  GVirConfigObject *child)
{
    g_return_if_fail(child_name != NULL);

    if (child == NULL)
        gvir_config_object_delete_children(parent, child_name, NULL);
    else
        gvir_config_object_attach(parent, child, TRUE);
}

G_GNUC_INTERNAL void
gvir_config_object_attach_add(GVirConfigObject *parent, GVirConfigObject *child)
{
    gvir_config_object_attach(parent, child, FALSE);
}

G_GNUC_INTERNAL void
gvir_config_object_remove_attribute(GVirConfigObject *object,
                                    const char *attr_name)
{
    int status;

    do {
        status = xmlUnsetProp(object->priv->node, (xmlChar *)attr_name);
    } while (status == 0);
}

static gboolean
gvir_config_object_set_xmlnode_namespace(xmlNodePtr node, const char *ns,
                                         const char *ns_uri)
{
    xmlNsPtr namespace;

    namespace = xmlNewNs(node, (xmlChar *)ns_uri, (xmlChar *)ns);
    if (namespace == NULL)
        return FALSE;

    xmlSetNs(node, namespace);
    return TRUE;
}

static gboolean
gvir_config_object_set_namespace_recursively(xmlNodePtr node,
                                             const char *ns,
                                             const char *ns_uri)
{
    xmlNodePtr n;

    for (n = node; n != NULL; n = n->next) {
        if (n->type == XML_ELEMENT_NODE) {
            if (!gvir_config_object_set_xmlnode_namespace(n, ns, ns_uri))
                return FALSE;
        }

        if (!gvir_config_object_set_namespace_recursively(n->children, ns, NULL))
            return FALSE;
    }

    return TRUE;
}

G_GNUC_INTERNAL gboolean
gvir_config_object_set_namespace(GVirConfigObject *object, const char *ns,
                                 const char *ns_uri, gboolean ns_children)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_OBJECT(object), FALSE);
    g_return_val_if_fail(ns != NULL, FALSE);
    g_return_val_if_fail(ns_uri != NULL, FALSE);

    if (!ns_children) {
        return gvir_config_object_set_xmlnode_namespace(object->priv->node,
                                                        ns, ns_uri);
    }

    return gvir_config_object_set_namespace_recursively(object->priv->node,
                                                        ns, ns_uri);
}

G_GNUC_INTERNAL GVirConfigObject *
gvir_config_object_get_child_with_type(GVirConfigObject *object,
                                       const gchar *child_name,
                                       GType child_type)
{
    xmlNodePtr node;

    g_return_val_if_fail(GVIR_CONFIG_IS_OBJECT(object), NULL);
    g_return_val_if_fail(child_name != NULL, NULL);

    node = gvir_config_xml_get_element(object->priv->node, child_name, NULL);
    if (node == NULL)
        return NULL;

    return gvir_config_object_new_from_tree(child_type,
                                            object->priv->doc,
                                            object->priv->schema,
                                            node);
}

G_GNUC_INTERNAL GVirConfigObject *
gvir_config_object_get_child(GVirConfigObject *object,
                             const gchar *child_name)
{
    return gvir_config_object_get_child_with_type(object,
                                                  child_name,
                                                  GVIR_CONFIG_TYPE_OBJECT);
}

G_GNUC_INTERNAL gboolean
gvir_config_object_has_child(GVirConfigObject *object, const gchar *child_name)
{
    xmlNodePtr node;

    g_return_val_if_fail(GVIR_CONFIG_IS_OBJECT(object), FALSE);
    g_return_val_if_fail(child_name != NULL, FALSE);

    node = gvir_config_xml_get_element(object->priv->node, child_name, NULL);

    return (node != NULL);
}
