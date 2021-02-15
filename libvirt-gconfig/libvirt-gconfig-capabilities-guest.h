/*
 * libvirt-gconfig-capabilities-guest.h: libvirt guest capabilities
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

#include "libvirt-gconfig-domain-os.h"
#include "libvirt-gconfig-capabilities-guest-arch.h"

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_CAPABILITIES_GUEST            (gvir_config_capabilities_guest_get_type ())
#define GVIR_CONFIG_CAPABILITIES_GUEST(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST, GVirConfigCapabilitiesGuest))
#define GVIR_CONFIG_CAPABILITIES_GUEST_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST, GVirConfigCapabilitiesGuestClass))
#define GVIR_CONFIG_IS_CAPABILITIES_GUEST(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST))
#define GVIR_CONFIG_IS_CAPABILITIES_GUEST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST))
#define GVIR_CONFIG_CAPABILITIES_GUEST_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST, GVirConfigCapabilitiesGuestClass))

typedef struct _GVirConfigCapabilitiesGuest GVirConfigCapabilitiesGuest;
typedef struct _GVirConfigCapabilitiesGuestPrivate GVirConfigCapabilitiesGuestPrivate;
typedef struct _GVirConfigCapabilitiesGuestClass GVirConfigCapabilitiesGuestClass;

struct _GVirConfigCapabilitiesGuest
{
    GVirConfigObject parent;

    GVirConfigCapabilitiesGuestPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigCapabilitiesGuestClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_capabilities_guest_get_type(void);

GVirConfigDomainOsType
gvir_config_capabilities_guest_get_os_type(GVirConfigCapabilitiesGuest *guest);

GVirConfigCapabilitiesGuestArch *
gvir_config_capabilities_guest_get_arch(GVirConfigCapabilitiesGuest *guest);
GList *
gvir_config_capabilities_guest_get_features(GVirConfigCapabilitiesGuest *guest);

G_END_DECLS
