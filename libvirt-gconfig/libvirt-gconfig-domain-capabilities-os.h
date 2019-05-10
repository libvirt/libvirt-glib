/*
 * libvirt-gconfig-domain-capabilities-os.h: libvirt domain capabilities OS
 *
 * Copyright (C) 2019 Red Hat, Inc.
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

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CAPABILITIES_OS_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CAPABILITIES_OS_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES_OS            (gvir_config_domain_capabilities_os_get_type ())
#define GVIR_CONFIG_DOMAIN_CAPABILITIES_OS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES_OS, GVirConfigDomainCapabilitiesOs))
#define GVIR_CONFIG_DOMAIN_CAPABILITIES_OS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES_OS, GVirConfigDomainCapabilitiesOsClass))
#define GVIR_CONFIG_IS_DOMAIN_CAPABILITIES_OS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES_OS))
#define GVIR_CONFIG_IS_DOMAIN_CAPABILITIES_OS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES_OS))
#define GVIR_CONFIG_DOMAIN_CAPABILITIES_OS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES_OS, GVirConfigDomainCapabilitiesOsClass))

typedef struct _GVirConfigDomainCapabilitiesOs GVirConfigDomainCapabilitiesOs;
typedef struct _GVirConfigDomainCapabilitiesOsPrivate GVirConfigDomainCapabilitiesOsPrivate;
typedef struct _GVirConfigDomainCapabilitiesOsClass GVirConfigDomainCapabilitiesOsClass;

struct _GVirConfigDomainCapabilitiesOs
{
    GVirConfigObject parent;

    GVirConfigDomainCapabilitiesOsPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainCapabilitiesOsClass
{
    GVirConfigObjectClass parent_class;

    GList *(*get_firmwares)(GVirConfigDomainCapabilitiesOs *os);

    gpointer padding[19];
};

GType gvir_config_domain_capabilities_os_get_type(void);

GList *
gvir_config_domain_capabilities_os_get_firmwares(GVirConfigDomainCapabilitiesOs *os);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CAPABILITIES_OS_H__ */
