/*
 * libvirt-gconfig-capabilities-guest-feature.h: libvirt guest feature capabilities
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

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_FEATURE            (gvir_config_capabilities_guest_feature_get_type ())
#define GVIR_CONFIG_CAPABILITIES_GUEST_FEATURE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_FEATURE, GVirConfigCapabilitiesGuestFeature))
#define GVIR_CONFIG_CAPABILITIES_GUEST_FEATURE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_FEATURE, GVirConfigCapabilitiesGuestFeatureClass))
#define GVIR_CONFIG_IS_CAPABILITIES_GUEST_FEATURE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_FEATURE))
#define GVIR_CONFIG_IS_CAPABILITIES_GUEST_FEATURE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_FEATURE))
#define GVIR_CONFIG_CAPABILITIES_GUEST_FEATURE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_FEATURE, GVirConfigCapabilitiesGuestFeatureClass))

typedef struct _GVirConfigCapabilitiesGuestFeature GVirConfigCapabilitiesGuestFeature;
typedef struct _GVirConfigCapabilitiesGuestFeaturePrivate GVirConfigCapabilitiesGuestFeaturePrivate;
typedef struct _GVirConfigCapabilitiesGuestFeatureClass GVirConfigCapabilitiesGuestFeatureClass;

struct _GVirConfigCapabilitiesGuestFeature
{
    GVirConfigObject parent;

    GVirConfigCapabilitiesGuestFeaturePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigCapabilitiesGuestFeatureClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_capabilities_guest_feature_get_type(void);

const gchar *
gvir_config_capabilities_guest_feature_get_name(GVirConfigCapabilitiesGuestFeature *feature);

G_END_DECLS
