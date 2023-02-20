/*
 * libvirt-gconfig-domain-graphics-dbus.c: libvirt domain DBUS configuration
 *
 * Copyright (C) 2023 Red Hat, Inc.
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
 * Author: Felipe Borges <felipeborges@gnome.org>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_GRAPHICS_DBUS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DBUS, GVirConfigDomainGraphicsDBusPrivate))

struct _GVirConfigDomainGraphicsDBusPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainGraphicsDBus, gvir_config_domain_graphics_dbus, GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS);


static void gvir_config_domain_graphics_dbus_class_init(GVirConfigDomainGraphicsDBusClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_domain_graphics_dbus_init(GVirConfigDomainGraphicsDBus *graphics_dbus)
{
    graphics_dbus->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_DBUS_GET_PRIVATE(graphics_dbus);
}

GVirConfigDomainGraphicsDBus *gvir_config_domain_graphics_dbus_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DBUS,
                                    "graphics", NULL);
    gvir_config_object_set_attribute(object, "type", "dbus", NULL);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_DBUS(object);
}

GVirConfigDomainGraphicsDBus *
gvir_config_domain_graphics_dbus_new_from_xml(const gchar *xml,
                                              GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DBUS,
                                             "graphics", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "dbus") != 0) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
    return GVIR_CONFIG_DOMAIN_GRAPHICS_DBUS(object);
}

const char *gvir_config_domain_graphics_dbus_get_address(GVirConfigDomainGraphicsDBus *graphics)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_DBUS(graphics), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(graphics),
                                            NULL, "address");
}

void gvir_config_domain_graphics_dbus_set_address(GVirConfigDomainGraphicsDBus *graphics,
                                                  const char *address)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_DBUS(graphics));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(graphics),
                                     "address", address,
                                     NULL);
}

void gvir_config_domain_graphics_dbus_set_gl(GVirConfigDomainGraphicsDBus *graphics,
                                             gboolean gl)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_DBUS(graphics));

    gvir_config_object_replace_child_with_attribute_enum
        (GVIR_CONFIG_OBJECT(graphics), "gl", "enable", G_TYPE_BOOLEAN, gl);
}
