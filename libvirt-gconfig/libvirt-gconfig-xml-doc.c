/*
 * libvirt-gconfig-xml-doc.c: libvirt XML document management
 *
 * Copyright (C) 2011 Red Hat, Inc.
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
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include <string.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-xml-doc.h"

extern gboolean debugFlag;

#define GVIR_CONFIG_XML_DOC_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_XML_DOC, GVirConfigXmlDocPrivate))

struct _GVirConfigXmlDocPrivate
{
    xmlDocPtr doc;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigXmlDoc, gvir_config_xml_doc, G_TYPE_OBJECT);

enum {
    PROP_0,
    PROP_DOC
};

static void gvir_config_xml_doc_get_property(GObject *object,
                                              guint prop_id,
                                              GValue *value,
                                              GParamSpec *pspec)
{
    GVirConfigXmlDoc *xml_doc = GVIR_CONFIG_XML_DOC(object);

    switch (prop_id) {
    case PROP_DOC:
        g_value_set_pointer(value, xml_doc->priv->doc);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void gvir_config_xml_doc_set_property(GObject *object,
                                              guint prop_id,
                                              const GValue *value,
                                              GParamSpec *pspec)
{
    GVirConfigXmlDoc *doc = GVIR_CONFIG_XML_DOC(object);

    switch (prop_id) {
    case PROP_DOC:
        doc->priv->doc = g_value_get_pointer(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void gvir_config_xml_doc_finalize(GObject *object)
{
    GVirConfigXmlDoc *doc = GVIR_CONFIG_XML_DOC(object);
    GVirConfigXmlDocPrivate *priv = doc->priv;

    xmlFreeDoc(priv->doc);

    G_OBJECT_CLASS(gvir_config_xml_doc_parent_class)->finalize(object);
}

static void gvir_config_xml_doc_class_init(GVirConfigXmlDocClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->finalize = gvir_config_xml_doc_finalize;
    object_class->get_property = gvir_config_xml_doc_get_property;
    object_class->set_property = gvir_config_xml_doc_set_property;

    g_object_class_install_property(object_class,
                                    PROP_DOC,
                                    g_param_spec_pointer("doc",
                                                        "XML Doc",
                                                        "The XML doc this config object corresponds to",
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));
}


static void gvir_config_xml_doc_init(GVirConfigXmlDoc *xml_doc)
{
    GVirConfigXmlDocPrivate *priv;

    priv = xml_doc->priv = GVIR_CONFIG_XML_DOC_GET_PRIVATE(xml_doc);

    memset(priv, 0, sizeof(*priv));
}


GVirConfigXmlDoc *gvir_config_xml_doc_new(xmlDocPtr doc)
{
    if (doc == NULL) {
        doc = xmlNewDoc((xmlChar *)"1.0");
    }
    return GVIR_CONFIG_XML_DOC(g_object_new(GVIR_CONFIG_TYPE_XML_DOC,
                                             "doc", doc,
                                             NULL));
}
