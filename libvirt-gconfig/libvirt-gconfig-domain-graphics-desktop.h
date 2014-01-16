/*
 * libvirt-gconfig-domain-graphics-desktop.h: libvirt domain desktop configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_GRAPHICS_DESKTOP_H__
#define __LIBVIRT_GCONFIG_DOMAIN_GRAPHICS_DESKTOP_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DESKTOP            (gvir_config_domain_graphics_desktop_get_type ())
#define GVIR_CONFIG_DOMAIN_GRAPHICS_DESKTOP(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DESKTOP, GVirConfigDomainGraphicsDesktop))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_DESKTOP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DESKTOP, GVirConfigDomainGraphicsDesktopClass))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_DESKTOP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DESKTOP))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_DESKTOP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DESKTOP))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_DESKTOP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_DESKTOP, GVirConfigDomainGraphicsDesktopClass))

typedef struct _GVirConfigDomainGraphicsDesktop GVirConfigDomainGraphicsDesktop;
typedef struct _GVirConfigDomainGraphicsDesktopPrivate GVirConfigDomainGraphicsDesktopPrivate;
typedef struct _GVirConfigDomainGraphicsDesktopClass GVirConfigDomainGraphicsDesktopClass;

struct _GVirConfigDomainGraphicsDesktop
{
    GVirConfigDomainGraphics parent;

    GVirConfigDomainGraphicsDesktopPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainGraphicsDesktopClass
{
    GVirConfigDomainGraphicsClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_graphics_desktop_get_type(void);

GVirConfigDomainGraphicsDesktop *gvir_config_domain_graphics_desktop_new(void);
GVirConfigDomainGraphicsDesktop *gvir_config_domain_graphics_desktop_new_from_xml(const gchar *xml,
                                                                                  GError **error);
const gchar *gvir_config_domain_graphics_desktop_get_display(GVirConfigDomainGraphicsDesktop *graphics);
void gvir_config_domain_graphics_desktop_set_display(GVirConfigDomainGraphicsDesktop *graphics,
                                                     const gchar *disp);
gboolean gvir_config_domain_graphics_desktop_get_fullscreen(GVirConfigDomainGraphicsDesktop *graphics);
void gvir_config_domain_graphics_desktop_set_fullscreen(GVirConfigDomainGraphicsDesktop *graphics,
                                                        gboolean fullscreen);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_GRAPHICS_DESKTOP_H__ */
