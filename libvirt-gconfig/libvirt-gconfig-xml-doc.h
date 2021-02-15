/*
 * libvirt-gobject-xml-doc.h: libvirt XML document management
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_XML_DOC            (gvir_config_xml_doc_get_type ())
#define GVIR_CONFIG_XML_DOC(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_XML_DOC, GVirConfigXmlDoc))
#define GVIR_CONFIG_XML_DOC_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_XML_DOC, GVirConfigXmlDocClass))
#define GVIR_CONFIG_IS_XML_DOC(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_XML_DOC))
#define GVIR_CONFIG_IS_XML_DOC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_XML_DOC))
#define GVIR_CONFIG_XML_DOC_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_XML_DOC, GVirConfigXmlDocClass))

typedef struct _GVirConfigXmlDoc GVirConfigXmlDoc;
typedef struct _GVirConfigXmlDocPrivate GVirConfigXmlDocPrivate;
typedef struct _GVirConfigXmlDocClass GVirConfigXmlDocClass;

struct _GVirConfigXmlDoc
{
    GObject parent;

    GVirConfigXmlDocPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigXmlDocClass
{
    GObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_xml_doc_get_type(void);

GVirConfigXmlDoc *gvir_config_xml_doc_new(xmlDocPtr doc);

G_END_DECLS
