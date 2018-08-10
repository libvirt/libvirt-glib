/*
 * libvirt-gconfig-domain-graphics-rdp.c: libvirt domain RDP configuration
 *
 * Copyright (C) 2011, 2014 Red Hat, Inc.
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

#define GVIR_CONFIG_DOMAIN_GRAPHICS_RDP_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_RDP, GVirConfigDomainGraphicsRdpPrivate))

struct _GVirConfigDomainGraphicsRdpPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainGraphicsRdp, gvir_config_domain_graphics_rdp, GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS);


static void gvir_config_domain_graphics_rdp_class_init(GVirConfigDomainGraphicsRdpClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_graphics_rdp_init(GVirConfigDomainGraphicsRdp *graphics_rdp)
{
    graphics_rdp->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_RDP_GET_PRIVATE(graphics_rdp);
}


GVirConfigDomainGraphicsRdp *gvir_config_domain_graphics_rdp_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_RDP,
                                    "graphics", NULL);
    gvir_config_object_set_attribute(object, "type", "rdp", NULL);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_RDP(object);
}

GVirConfigDomainGraphicsRdp *
gvir_config_domain_graphics_rdp_new_from_xml(const gchar *xml,
                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_RDP,
                                             "graphics", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "rdp") != 0) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
    return GVIR_CONFIG_DOMAIN_GRAPHICS_RDP(object);
}

void gvir_config_domain_graphics_rdp_set_autoport(GVirConfigDomainGraphicsRdp *graphics,
                                                  gboolean autoport)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_RDP(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "autoport", G_TYPE_BOOLEAN, autoport,
                                               NULL);
}

int gvir_config_domain_graphics_rdp_get_port(GVirConfigDomainGraphicsRdp *graphics)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_RDP(graphics), 0);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(graphics),
                                                   NULL, "port", 0);
}

void gvir_config_domain_graphics_rdp_set_port(GVirConfigDomainGraphicsRdp *graphics,
                                              int port)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_RDP(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "port", G_TYPE_INT, port,
                                               NULL);
}


void gvir_config_domain_graphics_rdp_set_replace_user(GVirConfigDomainGraphicsRdp *graphics,
                                                      gboolean replace_user)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_RDP(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "replaceUser",
                                               G_TYPE_BOOLEAN,
                                               replace_user,
                                               NULL);
}


gboolean gvir_config_domain_graphics_rdp_get_replace_user(GVirConfigDomainGraphicsRdp *graphics)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_RDP(graphics), FALSE);

    return gvir_config_object_get_attribute_boolean(GVIR_CONFIG_OBJECT(graphics),
                                                    NULL,
                                                    "replaceUser",
                                                    FALSE);
}


void gvir_config_domain_graphics_rdp_set_multi_user(GVirConfigDomainGraphicsRdp *graphics,
                                                    gboolean multi_user)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_RDP(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "multiUser",
                                               G_TYPE_BOOLEAN,
                                               multi_user,
                                               NULL);
}


gboolean gvir_config_domain_graphics_rdp_get_multi_user(GVirConfigDomainGraphicsRdp *graphics)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_RDP(graphics), FALSE);

    return gvir_config_object_get_attribute_boolean(GVIR_CONFIG_OBJECT(graphics),
                                                    NULL,
                                                    "multiUser",
                                                    FALSE);
}
