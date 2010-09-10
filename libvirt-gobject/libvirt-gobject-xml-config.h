/*
 * libvirt-gobject-xml-config.c: libvirt gobject integration
 *
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

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#ifndef __LIBVIRT_GOBJECT_XML_CONFIG_H__
#define __LIBVIRT_GOBJECT_XML_CONFIG_H__

G_BEGIN_DECLS

#define GVIR_TYPE_XML_CONFIG            (gvir_xml_config_get_type ())
#define GVIR_XML_CONFIG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_XML_CONFIG, GVirXmlConfig))
#define GVIR_XML_CONFIG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_XML_CONFIG, GVirXmlConfigClass))
#define GVIR_IS_XML_CONFIG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_XML_CONFIG))
#define GVIR_IS_XML_CONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_XML_CONFIG))
#define GVIR_XML_CONFIG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_XML_CONFIG, GVirXmlConfigClass))

typedef struct _GVirXmlConfig GVirXmlConfig;
typedef struct _GVirXmlConfigPrivate GVirXmlConfigPrivate;
typedef struct _GVirXmlConfigClass GVirXmlConfigClass;

struct _GVirXmlConfig
{
    GObject parent;

    GVirXmlConfigPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirXmlConfigClass
{
    GObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_xml_config_get_type(void);

GVirXmlConfig *gvir_xml_config_new(const gchar *doc,
                                   const gchar *schema);

void gvir_xml_config_validate(GVirXmlConfig *config,
                              GError **err);

const gchar *gvir_xml_config_get_doc(GVirXmlConfig *config);
const gchar *gvir_xml_config_get_schema(GVirXmlConfig *config);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_XML_CONFIG_H__ */
