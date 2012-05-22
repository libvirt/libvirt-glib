/*
 * libvirt-gconfig-capabilities-cpu.c: libvirt CPU capabilities
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Authors: Zeeshan Ali <zeenix@redhat.com>
 *          Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_CAPABILITIES_CPU_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU, GVirConfigCapabilitiesCpuPrivate))

struct _GVirConfigCapabilitiesCpuPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigCapabilitiesCpu, gvir_config_capabilities_cpu, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_capabilities_cpu_class_init(GVirConfigCapabilitiesCpuClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigCapabilitiesCpuPrivate));
}

static void gvir_config_capabilities_cpu_init(GVirConfigCapabilitiesCpu *cpu)
{
    g_debug("Init GVirConfigCapabilitiesCpu=%p", cpu);

    cpu->priv = GVIR_CONFIG_CAPABILITIES_CPU_GET_PRIVATE(cpu);
}

const gchar *
gvir_config_capabilities_cpu_get_arch(GVirConfigCapabilitiesCpu *cpu)
{
    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(cpu), "arch");
}

struct GetFeatureData {
    GVirConfigXmlDoc *doc;
    const gchar *schema;
    GList *features;
};

static gboolean add_feature(xmlNodePtr node, gpointer opaque)
{
    struct GetFeatureData* data = (struct GetFeatureData*)opaque;
    GVirConfigObject *feature;

    if (g_strcmp0((const gchar *)node->name, "feature") != 0)
        return TRUE;

    feature = gvir_config_object_new_from_tree
                                (GVIR_CONFIG_TYPE_CAPABILITIES_CPU_FEATURE,
                                 data->doc,
                                 data->schema,
                                 node);
    if (feature != NULL)
        data->features = g_list_append(data->features, feature);
    else
        g_debug("Failed to parse %s node", node->name);

    return TRUE;
}

/**
 * gvir_config_capabilities_cpu_get_features:
 *
 * Gets the features of this CPU.
 *
 * Returns: (element-type LibvirtGConfig.CapabilitiesCpuFeature) (transfer full):
 * a newly allocated #GList of #GVirConfigCapabilitiesCpuFeature.
 */
GList *
gvir_config_capabilities_cpu_get_features(GVirConfigCapabilitiesCpu *cpu)
{
    struct GetFeatureData data;

    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU(cpu), NULL);

    data.schema = gvir_config_object_get_schema(GVIR_CONFIG_OBJECT(cpu));
    g_object_get(G_OBJECT(cpu), "doc", &data.doc, NULL);
    g_return_val_if_fail(data.doc != NULL, NULL);
    data.features = NULL;

    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(cpu),
                                     NULL,
                                     add_feature,
                                     &data);
    g_clear_object(&data.doc);

    return data.features;
}

/**
 * gvir_config_capabilities_cpu_get_topology:
 *
 * Gets the topology of the cpu.
 *
 * Returns: (transfer full): a new #GVirConfigCapabilitiesCpuTopology.
 */
GVirConfigCapabilitiesCpuTopology *
gvir_config_capabilities_cpu_get_topology(GVirConfigCapabilitiesCpu *cpu)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU(cpu), NULL);

    object = gvir_config_object_get_child_with_type
                                (GVIR_CONFIG_OBJECT(cpu),
                                 "topology",
                                 GVIR_CONFIG_TYPE_CAPABILITIES_CPU_TOPOLOGY);

    return GVIR_CONFIG_CAPABILITIES_CPU_TOPOLOGY(object);
}
