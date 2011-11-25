/*
 * libvirt-gobject-config-domain-graphics-spice.c: libvirt glib integration
 *
 * Copyright (C) 2011 Red Hat
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
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include <string.h>

#include <libxml/tree.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"

#define GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_SPICE, GVirConfigDomainGraphicsSpicePrivate))

struct _GVirConfigDomainGraphicsSpicePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainGraphicsSpice, gvir_config_domain_graphics_spice, GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS);


static void gvir_config_domain_graphics_spice_class_init(GVirConfigDomainGraphicsSpiceClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainGraphicsSpicePrivate));
}


static void gvir_config_domain_graphics_spice_init(GVirConfigDomainGraphicsSpice *graphics_spice)
{
    g_debug("Init GVirConfigDomainGraphicsSpice=%p", graphics_spice);

    graphics_spice->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE_GET_PRIVATE(graphics_spice);
}


GVirConfigDomainGraphicsSpice *gvir_config_domain_graphics_spice_new(void)
{
    xmlDocPtr doc;
    xmlNodePtr node;

    doc = xmlNewDoc((xmlChar *)"1.0");
    node= xmlNewDocNode(doc, NULL, (xmlChar *)"graphics", NULL);
    xmlNewProp(doc->children, (xmlChar*)"type", (xmlChar*)"spice");
    xmlDocSetRootElement(doc, node);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE(g_object_new(GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_SPICE,
                                             "node", node,
                                             NULL));
}

GVirConfigDomainGraphicsSpice *gvir_config_domain_graphics_spice_new_from_xml(const gchar *xml,
                                                                 GError **error)
{
    xmlNodePtr node;

    node = gvir_config_xml_parse(xml, "graphics", error);
    if ((error != NULL) && (*error != NULL))
        return NULL;
    xmlNewProp(node, (xmlChar*)"type", (xmlChar*)"spice");
    return GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE(g_object_new(GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_SPICE,
                                                          "node", node,
                                                          NULL));
}

void gvir_config_domain_graphics_spice_set_port(GVirConfigDomainGraphicsSpice *graphics,
                                         unsigned int port)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_GRAPHICS_SPICE(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "port", G_TYPE_UINT, port,
                                               NULL);
}
