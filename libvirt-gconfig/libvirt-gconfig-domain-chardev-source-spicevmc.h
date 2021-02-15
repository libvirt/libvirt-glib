/*
 * libvirt-gconfig-domain-chardev-source-spicevmc.h: libvirt domain chardev spicevmc configuration
 *
 * Copyright (C) 2012 Red Hat, Inc.
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_VMC            (gvir_config_domain_chardev_source_spicevmc_get_type ())
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_VMC(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_VMC, GVirConfigDomainChardevSourceSpiceVmc))
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_VMC_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_VMC, GVirConfigDomainChardevSourceSpiceVmcClass))
#define GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE_SPICE_VMC(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_VMC))
#define GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE_SPICE_VMC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_VMC))
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_VMC_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_VMC, GVirConfigDomainChardevSourceSpiceVmcClass))

typedef struct _GVirConfigDomainChardevSourceSpiceVmc GVirConfigDomainChardevSourceSpiceVmc;
typedef struct _GVirConfigDomainChardevSourceSpiceVmcPrivate GVirConfigDomainChardevSourceSpiceVmcPrivate;
typedef struct _GVirConfigDomainChardevSourceSpiceVmcClass GVirConfigDomainChardevSourceSpiceVmcClass;

struct _GVirConfigDomainChardevSourceSpiceVmc
{
    GVirConfigDomainChardevSource parent;

    GVirConfigDomainChardevSourceSpiceVmcPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainChardevSourceSpiceVmcClass
{
    GVirConfigDomainChardevSourceClass parent_class;

    gpointer padding[20];
};


GType gvir_config_domain_chardev_source_spicevmc_get_type(void);

GVirConfigDomainChardevSourceSpiceVmc *gvir_config_domain_chardev_source_spicevmc_new(void);
GVirConfigDomainChardevSourceSpiceVmc *gvir_config_domain_chardev_source_spicevmc_new_from_xml(const gchar *xml,
                                                                              GError **error);
G_END_DECLS
