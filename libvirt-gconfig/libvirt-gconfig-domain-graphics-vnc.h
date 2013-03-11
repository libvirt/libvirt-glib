/*
 * libvirt-gconfig-domain-graphics-vnc.h: libvirt domain VNC graphics configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_GRAPHICS_VNC_H__
#define __LIBVIRT_GCONFIG_DOMAIN_GRAPHICS_VNC_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_VNC            (gvir_config_domain_graphics_vnc_get_type ())
#define GVIR_CONFIG_DOMAIN_GRAPHICS_VNC(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_VNC, GVirConfigDomainGraphicsVnc))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_VNC_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_VNC, GVirConfigDomainGraphicsVncClass))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_VNC(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_VNC))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_VNC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_VNC))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_VNC_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_VNC, GVirConfigDomainGraphicsVncClass))

typedef struct _GVirConfigDomainGraphicsVnc GVirConfigDomainGraphicsVnc;
typedef struct _GVirConfigDomainGraphicsVncPrivate GVirConfigDomainGraphicsVncPrivate;
typedef struct _GVirConfigDomainGraphicsVncClass GVirConfigDomainGraphicsVncClass;

struct _GVirConfigDomainGraphicsVnc
{
    GVirConfigDomainGraphics parent;

    GVirConfigDomainGraphicsVncPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainGraphicsVncClass
{
    GVirConfigDomainGraphicsClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_graphics_vnc_get_type(void);

GVirConfigDomainGraphicsVnc *gvir_config_domain_graphics_vnc_new(void);
GVirConfigDomainGraphicsVnc *gvir_config_domain_graphics_vnc_new_from_xml(const gchar *xml,
                                                                          GError **error);

const char *gvir_config_domain_graphics_vnc_get_socket(GVirConfigDomainGraphicsVnc *graphics);
void gvir_config_domain_graphics_vnc_set_socket(GVirConfigDomainGraphicsVnc *graphics,
                                                const char *socket);

void gvir_config_domain_graphics_vnc_set_autoport(GVirConfigDomainGraphicsVnc *graphics,
                                                  gboolean autoport);

int gvir_config_domain_graphics_vnc_get_port(GVirConfigDomainGraphicsVnc *graphics);
void gvir_config_domain_graphics_vnc_set_port(GVirConfigDomainGraphicsVnc *graphics,
                                              int port);

void gvir_config_domain_graphics_vnc_set_password(GVirConfigDomainGraphicsVnc *graphics,
                                                  const char *password);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_GRAPHICS_VNC_H__ */
