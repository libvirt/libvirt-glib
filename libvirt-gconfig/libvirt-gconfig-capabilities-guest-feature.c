/*
 * libvirt-gconfig-capabilities-guest-feature.c: libvirt guest feature capabilities
 *
 * Copyright (C) 2008 Daniel P. Berrange
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_CAPABILITIES_GUEST_FEATURE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_FEATURE, GVirConfigCapabilitiesGuestFeaturePrivate))

struct _GVirConfigCapabilitiesGuestFeaturePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigCapabilitiesGuestFeature, gvir_config_capabilities_guest_feature, GVIR_CONFIG_TYPE_OBJECT);

static void gvir_config_capabilities_guest_feature_class_init(GVirConfigCapabilitiesGuestFeatureClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_capabilities_guest_feature_init(GVirConfigCapabilitiesGuestFeature *feature)
{
    feature->priv = GVIR_CONFIG_CAPABILITIES_GUEST_FEATURE_GET_PRIVATE(feature);
}

const gchar *
gvir_config_capabilities_guest_feature_get_name(GVirConfigCapabilitiesGuestFeature *feature)
{
    xmlNodePtr node;

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(feature));
    g_return_val_if_fail(node != NULL, NULL);

    return (const gchar *)node->name;
}
