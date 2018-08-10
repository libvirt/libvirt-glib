/*
 * libvirt-gconfig-capabilities-guest.c: libvirt guest capabilities
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

#define GVIR_CONFIG_CAPABILITIES_GUEST_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST, GVirConfigCapabilitiesGuestPrivate))

struct _GVirConfigCapabilitiesGuestPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigCapabilitiesGuest, gvir_config_capabilities_guest, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_capabilities_guest_class_init(GVirConfigCapabilitiesGuestClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_capabilities_guest_init(GVirConfigCapabilitiesGuest *guest)
{
    guest->priv = GVIR_CONFIG_CAPABILITIES_GUEST_GET_PRIVATE(guest);
}

GVirConfigDomainOsType
gvir_config_capabilities_guest_get_os_type(GVirConfigCapabilitiesGuest *guest)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_GUEST(guest),
                         GVIR_CONFIG_DOMAIN_OS_TYPE_LINUX);

    return gvir_config_object_get_node_content_genum(GVIR_CONFIG_OBJECT(guest),
                                                     "os_type",
                                                     GVIR_CONFIG_TYPE_DOMAIN_OS_TYPE,
                                                     GVIR_CONFIG_DOMAIN_OS_TYPE_LINUX);
}

/**
 * gvir_config_capabilities_guest_get_arch:
 *
 * Gets the CPU architecture capabilities of the guest.
 *
 * Returns: (transfer full): a new #GVirConfigCapabilitiesGuestArch.
 */
GVirConfigCapabilitiesGuestArch *
gvir_config_capabilities_guest_get_arch(GVirConfigCapabilitiesGuest *guest)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_GUEST(guest), NULL);

    object = gvir_config_object_get_child_with_type
                                (GVIR_CONFIG_OBJECT(guest),
                                 "arch",
                                 GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_ARCH);

    return GVIR_CONFIG_CAPABILITIES_GUEST_ARCH(object);
}

struct GetFeatureData {
    GVirConfigXmlDoc *doc;
    const gchar *schema;
    GList *features;
};

static gboolean add_feature(xmlNodePtr node, gpointer opaque)
{
    struct GetFeatureData* data = (struct GetFeatureData*)opaque;
    GVirConfigObject *object;

    object = gvir_config_object_new_from_tree
                                (GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_FEATURE,
                                 data->doc,
                                 data->schema,
                                 node);
    if (object != NULL)
        data->features = g_list_append(data->features, object);
    else
        g_debug("Failed to parse %s node", node->name);

    return TRUE;
}

/**
 * gvir_config_capabilities_guest_get_features:
 *
 * Gets the CPU features for this guest.
 *
 * Returns: (element-type LibvirtGConfig.CapabilitiesGuestFeature) (transfer full):
 * a newly allocated #GList of #GVirConfigCapabilitiesGuestFeature.
 */
GList *
gvir_config_capabilities_guest_get_features(GVirConfigCapabilitiesGuest *guest)
{
    struct GetFeatureData data;

    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_GUEST(guest), NULL);

    g_object_get(G_OBJECT(guest), "doc", &data.doc, NULL);
    g_return_val_if_fail(data.doc != NULL, NULL);
    data.schema = gvir_config_object_get_schema(GVIR_CONFIG_OBJECT(guest));
    data.features = NULL;

    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(guest),
                                     "features",
                                     add_feature,
                                     &data);

    g_clear_object(&data.doc);

    return data.features;
}
