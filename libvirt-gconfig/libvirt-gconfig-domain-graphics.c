/*
 * libvirt-gconfig-domain-graphics.c: libvirt domain graphics configuration
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

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_GRAPHICS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS, GVirConfigDomainGraphicsPrivate))

struct _GVirConfigDomainGraphicsPrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GVirConfigDomainGraphics, gvir_config_domain_graphics, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_graphics_class_init(GVirConfigDomainGraphicsClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_graphics_init(GVirConfigDomainGraphics *graphics)
{
    graphics->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_GET_PRIVATE(graphics);
}

G_GNUC_INTERNAL GVirConfigDomainDevice *
gvir_config_domain_graphics_new_from_tree(GVirConfigXmlDoc *doc,
                                          xmlNodePtr tree)
{
    const char *type;
    GType gtype;

    type = gvir_config_xml_get_attribute_content(tree, "type");
    if (type == NULL)
        return NULL;

    if (g_str_equal(type, "sdl")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SDL;
    } else if (g_str_equal(type, "vnc")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_VNC;
    } else if (g_str_equal(type, "dbus")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DBUS;
    } else if (g_str_equal(type, "spice")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE;
    } else if (g_str_equal(type, "rdp")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_RDP;
    } else if (g_str_equal(type, "desktop")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DESKTOP;
    } else {
        g_debug("Unknown graphics node: %s", type);
        return NULL;
    }

    return GVIR_CONFIG_DOMAIN_DEVICE(gvir_config_object_new_from_tree(gtype, doc, NULL, tree));
}
