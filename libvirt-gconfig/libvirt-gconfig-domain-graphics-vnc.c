/*
 * libvirt-gobject-config-domain-graphics-vnc.c: libvirt glib integration
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <string.h>

#include <libxml/tree.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"

#define GVIR_CONFIG_DOMAIN_GRAPHICS_VNC_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_VNC, GVirConfigDomainGraphicsVncPrivate))

struct _GVirConfigDomainGraphicsVncPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainGraphicsVnc, gvir_config_domain_graphics_vnc, GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS);


static void gvir_config_domain_graphics_vnc_class_init(GVirConfigDomainGraphicsVncClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainGraphicsVncPrivate));
}


static void gvir_config_domain_graphics_vnc_init(GVirConfigDomainGraphicsVnc *graphics_vnc)
{
    g_debug("Init GVirConfigDomainGraphicsVnc=%p", graphics_vnc);

    graphics_vnc->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_VNC_GET_PRIVATE(graphics_vnc);
}


GVirConfigDomainGraphicsVnc *gvir_config_domain_graphics_vnc_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_VNC,
                                    "graphics", NULL);
    gvir_config_object_set_attribute(object, "type", "vnc", NULL);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_VNC(object);
}

GVirConfigDomainGraphicsVnc *
gvir_config_domain_graphics_vnc_new_from_xml(const gchar *xml,
                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS_VNC,
                                             "graphics", NULL, xml, error);
    if (object == NULL)
        return NULL;
    gvir_config_object_set_attribute(object, "type", "vnc", NULL);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_VNC(object);
}


void gvir_config_domain_graphics_vnc_set_autoport(GVirConfigDomainGraphicsVnc *graphics,
                                                  gboolean autoport)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_GRAPHICS_VNC(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "autoport", G_TYPE_BOOLEAN, autoport,
                                               NULL);
}

void gvir_config_domain_graphics_vnc_set_port(GVirConfigDomainGraphicsVnc *graphics,
                                              int port)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_GRAPHICS_VNC(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "port", G_TYPE_INT, port,
                                               NULL);
}
