/*
 * libvirt-gconfig-domain-graphics-desktop.c: libvirt domain desktop configuration
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

#define GVIR_CONFIG_DOMAIN_GRAPHICS_DESKTOP_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DESKTOP, GVirConfigDomainGraphicsDesktopPrivate))

struct _GVirConfigDomainGraphicsDesktopPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainGraphicsDesktop, gvir_config_domain_graphics_desktop, GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS);


static void gvir_config_domain_graphics_desktop_class_init(GVirConfigDomainGraphicsDesktopClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_graphics_desktop_init(GVirConfigDomainGraphicsDesktop *graphics_desktop)
{
    graphics_desktop->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_DESKTOP_GET_PRIVATE(graphics_desktop);
}


GVirConfigDomainGraphicsDesktop *gvir_config_domain_graphics_desktop_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DESKTOP,
                                    "graphics", NULL);
    gvir_config_object_set_attribute(object, "type", "desktop", NULL);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_DESKTOP(object);
}

GVirConfigDomainGraphicsDesktop *
gvir_config_domain_graphics_desktop_new_from_xml(const gchar *xml,
                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DESKTOP,
                                             "graphics", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "desktop") != 0) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
    return GVIR_CONFIG_DOMAIN_GRAPHICS_DESKTOP(object);
}

void gvir_config_domain_graphics_desktop_set_display(GVirConfigDomainGraphicsDesktop *graphics,
                                                 const gchar *disp)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_DESKTOP(graphics));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(graphics),
                                     "display", disp,
                                     NULL);
}


const gchar *gvir_config_domain_graphics_desktop_get_display(GVirConfigDomainGraphicsDesktop *graphics)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_DESKTOP(graphics), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(graphics),
                                            NULL,
                                            "display");
}


void gvir_config_domain_graphics_desktop_set_fullscreen(GVirConfigDomainGraphicsDesktop *graphics,
                                                        gboolean fullscreen)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_DESKTOP(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "fullscreen",
                                               G_TYPE_BOOLEAN,
                                               fullscreen,
                                               NULL);
}


gboolean gvir_config_domain_graphics_desktop_get_fullscreen(GVirConfigDomainGraphicsDesktop *graphics)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_DESKTOP(graphics), FALSE);

    return gvir_config_object_get_attribute_boolean(GVIR_CONFIG_OBJECT(graphics),
                                                    NULL,
                                                    "fullscreen",
                                                    FALSE);
}
