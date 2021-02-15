/*
 * libvirt-gconfig-capabilities-guest-domain.h: libvirt guest domain capabilities
 *
 * Copyright (C) 2010-2012 Red Hat, Inc.
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
 * Authors: Zeeshan Ali <zeenix@redhat.com>
 *          Daniel P. Berrange <berrange@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

#include "libvirt-gconfig-domain.h"

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_DOMAIN            (gvir_config_capabilities_guest_domain_get_type ())
#define GVIR_CONFIG_CAPABILITIES_GUEST_DOMAIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_DOMAIN, GVirConfigCapabilitiesGuestDomain))
#define GVIR_CONFIG_CAPABILITIES_GUEST_DOMAIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_DOMAIN, GVirConfigCapabilitiesGuestDomainClass))
#define GVIR_CONFIG_IS_CAPABILITIES_GUEST_DOMAIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_DOMAIN))
#define GVIR_CONFIG_IS_CAPABILITIES_GUEST_DOMAIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_DOMAIN))
#define GVIR_CONFIG_CAPABILITIES_GUEST_DOMAIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_DOMAIN, GVirConfigCapabilitiesGuestDomainClass))

typedef struct _GVirConfigCapabilitiesGuestDomain GVirConfigCapabilitiesGuestDomain;
typedef struct _GVirConfigCapabilitiesGuestDomainPrivate GVirConfigCapabilitiesGuestDomainPrivate;
typedef struct _GVirConfigCapabilitiesGuestDomainClass GVirConfigCapabilitiesGuestDomainClass;

struct _GVirConfigCapabilitiesGuestDomain
{
    GVirConfigObject parent;

    GVirConfigCapabilitiesGuestDomainPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigCapabilitiesGuestDomainClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_capabilities_guest_domain_get_type(void);

const gchar *
gvir_config_capabilities_guest_domain_get_emulator(GVirConfigCapabilitiesGuestDomain *domain);

GVirConfigDomainVirtType
gvir_config_capabilities_guest_domain_get_virt_type(GVirConfigCapabilitiesGuestDomain *domain);

G_END_DECLS
