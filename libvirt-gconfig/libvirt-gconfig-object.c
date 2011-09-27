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
#include <libxml/xmlerror.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"

//extern gboolean debugFlag;
gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_CONFIG_OBJECT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_OBJECT, GVirConfigObjectPrivate))

struct _GVirConfigObjectPrivate
{
    gchar *doc;
    gchar *schema;

    /* FIXME: docHandle is node->doc, can probably be removed to avoid the
     * 2 getting out of sync
     */
    xmlDocPtr docHandle;
    xmlNodePtr node;
};

G_DEFINE_ABSTRACT_TYPE(GVirConfigObject, gvir_config_object, G_TYPE_OBJECT);

enum {
    PROP_0,
    PROP_DOC,
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
    case PROP_DOC:
        g_value_set_string(value, priv->doc);
        break;

    case PROP_SCHEMA:
        g_value_set_string(value, priv->schema);
        break;

    case PROP_NODE:
        g_value_set_pointer(value, gvir_config_object_get_xml_node(conn, NULL));
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
    case PROP_DOC:
        g_free(priv->doc);
        priv->doc = g_value_dup_string(value);
        break;

    case PROP_SCHEMA:
        g_free(priv->schema);
        priv->schema = g_value_dup_string(value);
        break;

    case PROP_NODE:
        priv->node = g_value_get_pointer(value);
        if ((priv->docHandle != NULL) && (priv->docHandle != priv->node->doc))
            xmlFreeDoc(priv->docHandle);
        if (priv->node)
            priv->docHandle = priv->node->doc;
        else
            priv->docHandle = NULL;
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_config_object_finalize(GObject *object)
{
    GVirConfigObject *conn = GVIR_CONFIG_OBJECT(object);
    GVirConfigObjectPrivate *priv = conn->priv;

    DEBUG("Finalize GVirConfigObject=%p", conn);

    g_free(priv->doc);
    g_free(priv->schema);

    if (priv->docHandle)
        xmlFreeDoc(priv->docHandle);

    G_OBJECT_CLASS(gvir_config_object_parent_class)->finalize(object);
}


static void gvir_config_object_class_init(GVirConfigObjectClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = gvir_config_object_finalize;
    object_class->get_property = gvir_config_object_get_property;
    object_class->set_property = gvir_config_object_set_property;

    g_object_class_install_property(object_class,
                                    PROP_DOC,
                                    g_param_spec_string("doc",
                                                        "Doc",
                                                        "The XML document",
                                                        NULL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));
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

static void
gvir_config_object_parse(GVirConfigObject *config,
                         GError **err)
{
    GVirConfigObjectPrivate *priv = config->priv;
    if (priv->docHandle)
        return;

    if (!priv->doc) {
        *err = g_error_new(GVIR_CONFIG_OBJECT_ERROR,
                           0,
                           "%s",
                           "No XML document to parse");
        return;
    }

    priv->docHandle = xmlParseMemory(priv->doc, strlen(priv->doc));
    if (!priv->docHandle) {
        *err = gvir_xml_error_new(GVIR_CONFIG_OBJECT_ERROR,
                                  0,
                                  "%s",
                                  "Unable to parse configuration");
    }
    priv->node = priv->docHandle->children;
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

    gvir_config_object_parse(config, err);
    if (*err)
        return;

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

    if (xmlRelaxNGValidateDoc(rngValid, priv->docHandle) != 0) {
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

const gchar *gvir_config_object_get_doc(GVirConfigObject *config)
{
    GVirConfigObjectPrivate *priv = config->priv;
    return priv->doc;
}

const gchar *gvir_config_object_get_schema(GVirConfigObject *config)
{
    GVirConfigObjectPrivate *priv = config->priv;
    return priv->schema;
}

/* NB: the xmlDocPtr must not be freed by the caller */
/* gupnp has wrapped xmlDoc in a gobject */
/* not really useful, can be obtained from xmlNode::doc */
xmlDocPtr gvir_config_object_get_xml_doc(GVirConfigObject *config, GError **error)
{
    gvir_config_object_parse(config, error);
    return config->priv->docHandle;
}

/* FIXME: will we always have one xmlNode per GConfig object? */
/* FIXME: need to return the right node from subclasses */
/* NB: the xmlNodePtr must not be freed by the caller */
xmlNodePtr gvir_config_object_get_xml_node(GVirConfigObject *config,
                                           GError **error)
{
    gvir_config_object_parse(config, error);
    return config->priv->node;
}
