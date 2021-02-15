/*
 * libvirt-gconfig-domain-graphics-rdp.h: libvirt domain RDP graphics configuration
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

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_RDP            (gvir_config_domain_graphics_rdp_get_type ())
#define GVIR_CONFIG_DOMAIN_GRAPHICS_RDP(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_RDP, GVirConfigDomainGraphicsRdp))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_RDP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_RDP, GVirConfigDomainGraphicsRdpClass))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_RDP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_RDP))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_RDP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_RDP))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_RDP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_RDP, GVirConfigDomainGraphicsRdpClass))

typedef struct _GVirConfigDomainGraphicsRdp GVirConfigDomainGraphicsRdp;
typedef struct _GVirConfigDomainGraphicsRdpPrivate GVirConfigDomainGraphicsRdpPrivate;
typedef struct _GVirConfigDomainGraphicsRdpClass GVirConfigDomainGraphicsRdpClass;

struct _GVirConfigDomainGraphicsRdp
{
    GVirConfigDomainGraphics parent;

    GVirConfigDomainGraphicsRdpPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainGraphicsRdpClass
{
    GVirConfigDomainGraphicsClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_graphics_rdp_get_type(void);

GVirConfigDomainGraphicsRdp *gvir_config_domain_graphics_rdp_new(void);
GVirConfigDomainGraphicsRdp *gvir_config_domain_graphics_rdp_new_from_xml(const gchar *xml,
                                                                          GError **error);

void gvir_config_domain_graphics_rdp_set_autoport(GVirConfigDomainGraphicsRdp *graphics,
                                                  gboolean autoport);

int gvir_config_domain_graphics_rdp_get_port(GVirConfigDomainGraphicsRdp *graphics);
void gvir_config_domain_graphics_rdp_set_port(GVirConfigDomainGraphicsRdp *graphics,
                                              int port);

gboolean gvir_config_domain_graphics_rdp_get_multi_user(GVirConfigDomainGraphicsRdp *graphics);
void gvir_config_domain_graphics_rdp_set_multi_user(GVirConfigDomainGraphicsRdp *graphics,
                                                    gboolean multi_user);
gboolean gvir_config_domain_graphics_rdp_get_replace_user(GVirConfigDomainGraphicsRdp *graphics);
void gvir_config_domain_graphics_rdp_set_replace_user(GVirConfigDomainGraphicsRdp *graphics,
                                                      gboolean replace_user);

G_END_DECLS
