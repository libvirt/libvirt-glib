/*
 * libvirt-gconfig-domain-graphics-spice.h: libvirt domain SPICE confiiguration
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_GRAPHICS_SPICE_H__
#define __LIBVIRT_GCONFIG_DOMAIN_GRAPHICS_SPICE_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE            (gvir_config_domain_graphics_spice_get_type ())
#define GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE, GVirConfigDomainGraphicsSpice))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE, GVirConfigDomainGraphicsSpiceClass))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SPICE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SPICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE, GVirConfigDomainGraphicsSpiceClass))

typedef struct _GVirConfigDomainGraphicsSpice GVirConfigDomainGraphicsSpice;
typedef struct _GVirConfigDomainGraphicsSpicePrivate GVirConfigDomainGraphicsSpicePrivate;
typedef struct _GVirConfigDomainGraphicsSpiceClass GVirConfigDomainGraphicsSpiceClass;

struct _GVirConfigDomainGraphicsSpice
{
    GVirConfigDomainGraphics parent;

    GVirConfigDomainGraphicsSpicePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainGraphicsSpiceClass
{
    GVirConfigDomainGraphicsClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_graphics_spice_get_type(void);

GVirConfigDomainGraphicsSpice *gvir_config_domain_graphics_spice_new(void);
GVirConfigDomainGraphicsSpice *gvir_config_domain_graphics_spice_new_from_xml(const gchar *xml,
                                                                              GError **error);

void gvir_config_domain_graphics_spice_set_autoport(GVirConfigDomainGraphicsSpice *graphics,
                                                    gboolean autoport);

void gvir_config_domain_graphics_spice_set_password(GVirConfigDomainGraphicsSpice *graphics,
                                                    const char *password);

void gvir_config_domain_graphics_spice_set_port(GVirConfigDomainGraphicsSpice *graphics,
                                                int port);

void gvir_config_domain_graphics_spice_set_tls_port(GVirConfigDomainGraphicsSpice *graphics,
                                                    int port);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_GRAPHICS_SPICE_H__ */
