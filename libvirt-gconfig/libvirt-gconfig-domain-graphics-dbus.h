/*
 * libvirt-gconfig-domain-graphics-dbus.h: libvirt domain DBUS graphics configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DBUS            (gvir_config_domain_graphics_dbus_get_type ())
#define GVIR_CONFIG_DOMAIN_GRAPHICS_DBUS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DBUS, GVirConfigDomainGraphicsDBus))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_DBUS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DBUS, GVirConfigDomainGraphicsDBusClass))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_DBUS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DBUS))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_DBUS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DBUS))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_DBUS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DBUS, GVirConfigDomainGraphicsDBusClass))

typedef struct _GVirConfigDomainGraphicsDBus GVirConfigDomainGraphicsDBus;
typedef struct _GVirConfigDomainGraphicsDBusPrivate GVirConfigDomainGraphicsDBusPrivate;
typedef struct _GVirConfigDomainGraphicsDBusClass GVirConfigDomainGraphicsDBusClass;

struct _GVirConfigDomainGraphicsDBus
{
    GVirConfigDomainGraphics parent;

    GVirConfigDomainGraphicsDBusPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainGraphicsDBusClass
{
    GVirConfigDomainGraphicsClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_graphics_dbus_get_type(void);

GVirConfigDomainGraphicsDBus *gvir_config_domain_graphics_dbus_new(void);
GVirConfigDomainGraphicsDBus *gvir_config_domain_graphics_dbus_new_from_xml(const gchar *xml,
                                                                            GError **error);

const char *gvir_config_domain_graphics_dbus_get_address(GVirConfigDomainGraphicsDBus *graphics);
void gvir_config_domain_graphics_dbus_set_address(GVirConfigDomainGraphicsDBus *graphics,
                                                  const char *address);

void gvir_config_domain_graphics_dbus_set_gl(GVirConfigDomainGraphicsDBus *graphics,
                                             gboolean gl);

G_END_DECLS
