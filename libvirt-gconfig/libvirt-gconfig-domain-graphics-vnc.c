/*
 * libvirt-gconfig-domain-graphics-vnc.c: libvirt domain VNC configuration
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_GRAPHICS_VNC_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_VNC, GVirConfigDomainGraphicsVncPrivate))

struct _GVirConfigDomainGraphicsVncPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainGraphicsVnc, gvir_config_domain_graphics_vnc, GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS);


static void gvir_config_domain_graphics_vnc_class_init(GVirConfigDomainGraphicsVncClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_graphics_vnc_init(GVirConfigDomainGraphicsVnc *graphics_vnc)
{
    graphics_vnc->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_VNC_GET_PRIVATE(graphics_vnc);
}


GVirConfigDomainGraphicsVnc *gvir_config_domain_graphics_vnc_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_VNC,
                                    "graphics", NULL);
    gvir_config_object_set_attribute(object, "type", "vnc", NULL);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_VNC(object);
}

GVirConfigDomainGraphicsVnc *
gvir_config_domain_graphics_vnc_new_from_xml(const gchar *xml,
                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_VNC,
                                             "graphics", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "vnc") != 0) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
    return GVIR_CONFIG_DOMAIN_GRAPHICS_VNC(object);
}


const char *gvir_config_domain_graphics_vnc_get_socket(GVirConfigDomainGraphicsVnc *graphics)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_VNC(graphics), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(graphics),
                                            NULL, "socket");
}

void gvir_config_domain_graphics_vnc_set_socket(GVirConfigDomainGraphicsVnc *graphics,
                                                const char *socket)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_VNC(graphics));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(graphics),
                                     "socket", socket,
                                     NULL);
}

void gvir_config_domain_graphics_vnc_set_autoport(GVirConfigDomainGraphicsVnc *graphics,
                                                  gboolean autoport)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_VNC(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "autoport", G_TYPE_BOOLEAN, autoport,
                                               NULL);
}

int gvir_config_domain_graphics_vnc_get_port(GVirConfigDomainGraphicsVnc *graphics)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_VNC(graphics), 0);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(graphics),
                                                   NULL, "port", 0);
}

void gvir_config_domain_graphics_vnc_set_port(GVirConfigDomainGraphicsVnc *graphics,
                                              int port)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_VNC(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "port", G_TYPE_INT, port,
                                               NULL);
}

void gvir_config_domain_graphics_vnc_set_password(GVirConfigDomainGraphicsVnc *graphics,
                                                  const char *password)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_VNC(graphics));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(graphics),
                                     "passwd", password,
                                     NULL);
}
