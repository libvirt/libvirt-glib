/*
 * libvirt-gconfig-capabilities-host.c: libvirt host capabilities
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

#define GVIR_CONFIG_CAPABILITIES_HOST_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES_HOST, GVirConfigCapabilitiesHostPrivate))

struct _GVirConfigCapabilitiesHostPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigCapabilitiesHost, gvir_config_capabilities_host, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_capabilities_host_class_init(GVirConfigCapabilitiesHostClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_capabilities_host_init(GVirConfigCapabilitiesHost *host)
{
    host->priv = GVIR_CONFIG_CAPABILITIES_HOST_GET_PRIVATE(host);
}

const gchar *
gvir_config_capabilities_host_get_uuid(GVirConfigCapabilitiesHost *host)
{
    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(host), "uuid");
}

/**
 * gvir_config_capabilities_host_get_cpu:
 *
 * Gets the CPU capabilities of the host.
 *
 * Returns: (transfer full): a new #GVirConfigCapabilitiesCpu.
 */
GVirConfigCapabilitiesCpu *
gvir_config_capabilities_host_get_cpu(GVirConfigCapabilitiesHost *host)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_HOST(host), NULL);

    object = gvir_config_object_get_child_with_type
                                (GVIR_CONFIG_OBJECT(host),
                                 "cpu",
                                 GVIR_CONFIG_TYPE_CAPABILITIES_CPU);

    return GVIR_CONFIG_CAPABILITIES_CPU(object);
}

struct GetSecModelData {
    GVirConfigXmlDoc *doc;
    const gchar *schema;
    GList *secmodels;
    GType type;
};

static gboolean add_secmodel(xmlNodePtr node, gpointer opaque)
{
    struct GetSecModelData* data = (struct GetSecModelData*)opaque;
    GVirConfigObject *secmodel;

    if (g_strcmp0((const gchar *)node->name, "secmodel") != 0)
        return TRUE;

    secmodel = gvir_config_object_new_from_tree
                                (data->type,
                                 data->doc,
                                 data->schema,
                                 node);
    if (secmodel != NULL)
        data->secmodels = g_list_append(data->secmodels, secmodel);
    else
        g_debug("Failed to parse %s node", node->name);

    return TRUE;
}

/**
 * gvir_config_capabilities_host_get_secmodels:
 * @host: a #GVirConfigCapabilitiesHost
 *
 * Get the security models listed in @host capabilities.
 *
 * Returns: (element-type LibvirtGConfig.CapabilitiesHostSecModel) (transfer full):
 * a newly allocated #GList of #GVirConfigCapabilitiesHostSecModel.
 */
GList *
gvir_config_capabilities_host_get_secmodels(GVirConfigCapabilitiesHost *host)
{
    struct GetSecModelData data;

    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_HOST(host), NULL);

    data.schema = gvir_config_object_get_schema(GVIR_CONFIG_OBJECT(host));
    g_object_get(G_OBJECT(host), "doc", &data.doc, NULL);
    g_return_val_if_fail(data.doc != NULL, NULL);
    data.secmodels = NULL;
    data.type = GVIR_CONFIG_TYPE_CAPABILITIES_HOST_SECMODEL;

    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(host),
                                     NULL,
                                     add_secmodel,
                                     &data);

    g_clear_object(&data.doc);

    return data.secmodels;
}
