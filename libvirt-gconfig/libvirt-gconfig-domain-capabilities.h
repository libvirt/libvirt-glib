/*
 * libvirt-gconfig-domain-capabilities.c: libvirt domain capabilities configuration
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

#pragma once

#include "libvirt-gconfig/libvirt-gconfig-domain-capabilities-os.h"

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES            (gvir_config_domain_capabilities_get_type ())
#define GVIR_CONFIG_DOMAIN_CAPABILITIES(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES, GVirConfigDomainCapabilities))
#define GVIR_CONFIG_DOMAIN_CAPABILITIES_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES, GVirConfigDomainCapabilitiesClass))
#define GVIR_CONFIG_IS_DOMAIN_CAPABILITIES(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES))
#define GVIR_CONFIG_IS_DOMAIN_CAPABILITIES_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES))
#define GVIR_CONFIG_DOMAIN_CAPABILITIES_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES, GVirConfigDomainCapabilitiesClass))

typedef struct _GVirConfigDomainCapabilities GVirConfigDomainCapabilities;
typedef struct _GVirConfigDomainCapabilitiesPrivate GVirConfigDomainCapabilitiesPrivate;
typedef struct _GVirConfigDomainCapabilitiesClass GVirConfigDomainCapabilitiesClass;

struct _GVirConfigDomainCapabilities
{
    GVirConfigObject parent;

    GVirConfigDomainCapabilitiesPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainCapabilitiesClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_capabilities_get_type(void);

GVirConfigDomainCapabilities *gvir_config_domain_capabilities_new(void);
GVirConfigDomainCapabilities *gvir_config_domain_capabilities_new_from_xml(const gchar *xml,
                                                                           GError **error);
GVirConfigDomainCapabilitiesOs *gvir_config_domain_capabilities_get_os(GVirConfigDomainCapabilities *domain_caps);

G_END_DECLS
