/*
 * libvirt-gconfig-domain-chardev-source-spiceport.h: libvirt domain chardev spiceport configuration
 *
 * Copyright (C) 2014 Red Hat, Inc.
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
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_PORT_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_PORT_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_PORT            (gvir_config_domain_chardev_source_spiceport_get_type ())
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_PORT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_PORT, GVirConfigDomainChardevSourceSpicePort))
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_PORT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_PORT, GVirConfigDomainChardevSourceSpicePortClass))
#define GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE_SPICE_PORT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_PORT))
#define GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE_SPICE_PORT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_PORT))
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_PORT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_PORT, GVirConfigDomainChardevSourceSpicePortClass))

typedef struct _GVirConfigDomainChardevSourceSpicePort GVirConfigDomainChardevSourceSpicePort;
typedef struct _GVirConfigDomainChardevSourceSpicePortPrivate GVirConfigDomainChardevSourceSpicePortPrivate;
typedef struct _GVirConfigDomainChardevSourceSpicePortClass GVirConfigDomainChardevSourceSpicePortClass;

struct _GVirConfigDomainChardevSourceSpicePort
{
    GVirConfigDomainChardevSource parent;

    GVirConfigDomainChardevSourceSpicePortPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainChardevSourceSpicePortClass
{
    GVirConfigDomainChardevSourceClass parent_class;

    gpointer padding[20];
};


GType gvir_config_domain_chardev_source_spiceport_get_type(void);

GVirConfigDomainChardevSourceSpicePort *gvir_config_domain_chardev_source_spiceport_new(void);
GVirConfigDomainChardevSourceSpicePort *gvir_config_domain_chardev_source_spiceport_new_from_xml(const gchar *xml,
                                                                              GError **error);

void gvir_config_domain_chardev_source_spiceport_set_channel(GVirConfigDomainChardevSourceSpicePort *port,
                                                    const char *channel);

const gchar * gvir_config_domain_chardev_source_spiceport_get_channel(GVirConfigDomainChardevSourceSpicePort *port);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_PORT_H__ */
