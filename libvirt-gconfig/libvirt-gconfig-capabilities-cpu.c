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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
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

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigCapabilitiesCpu, gvir_config_capabilities_cpu, GVIR_CONFIG_TYPE_OBJECT);

static GList *
_gvir_config_capabilities_cpu_get_features(GVirConfigCapabilitiesCpu *cpu);

static void gvir_config_capabilities_cpu_class_init(GVirConfigCapabilitiesCpuClass *klass)
{
    klass->get_features = _gvir_config_capabilities_cpu_get_features;
}

static void gvir_config_capabilities_cpu_init(GVirConfigCapabilitiesCpu *cpu)
{
    cpu->priv = GVIR_CONFIG_CAPABILITIES_CPU_GET_PRIVATE(cpu);
}

const gchar *
gvir_config_capabilities_cpu_get_arch(GVirConfigCapabilitiesCpu *cpu)
{
    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(cpu), "arch");
}

/**
 * gvir_config_capabilities_cpu_add_feature:
 *
 * Adds a new feature to CPU.
 */
void gvir_config_capabilities_cpu_add_feature(GVirConfigCapabilitiesCpu *cpu,
                                              GVirConfigCapabilitiesCpuFeature *feature)
{
    g_return_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU(cpu));
    g_return_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU_FEATURE(feature));

    gvir_config_object_attach_add(GVIR_CONFIG_OBJECT(cpu),
                                  GVIR_CONFIG_OBJECT(feature));
}

struct GetFeatureData {
    GVirConfigXmlDoc *doc;
    const gchar *schema;
    GList *features;
    GType type;
};

static gboolean add_feature(xmlNodePtr node, gpointer opaque)
{
    struct GetFeatureData* data = (struct GetFeatureData*)opaque;
    GVirConfigObject *feature;

    if (g_strcmp0((const gchar *)node->name, "feature") != 0)
        return TRUE;

    feature = gvir_config_object_new_from_tree
                                (data->type,
                                 data->doc,
                                 data->schema,
                                 node);
    if (feature != NULL)
        data->features = g_list_append(data->features, feature);
    else
        g_debug("Failed to parse %s node", node->name);

    return TRUE;
}

G_GNUC_INTERNAL GList *
gvir_config_capabilities_cpu_get_features_with_type(GVirConfigCapabilitiesCpu *cpu,
                                                    GType type)
{
    struct GetFeatureData data;

    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU(cpu), NULL);

    data.schema = gvir_config_object_get_schema(GVIR_CONFIG_OBJECT(cpu));
    g_object_get(G_OBJECT(cpu), "doc", &data.doc, NULL);
    g_return_val_if_fail(data.doc != NULL, NULL);
    data.features = NULL;
    data.type = type;

    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(cpu),
                                     NULL,
                                     add_feature,
                                     &data);
    g_clear_object(&data.doc);

    return data.features;
}

static GList *
_gvir_config_capabilities_cpu_get_features(GVirConfigCapabilitiesCpu *cpu)
{
    return gvir_config_capabilities_cpu_get_features_with_type(
                        cpu, GVIR_CONFIG_TYPE_CAPABILITIES_CPU_FEATURE);
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
    return GVIR_CONFIG_CAPABILITIES_CPU_GET_CLASS(cpu)->get_features(cpu);
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

/**
 * gvir_config_capabilities_cpu_set_topology:
 *
 * Sets the topology of the cpu.
 */
void
gvir_config_capabilities_cpu_set_topology(GVirConfigCapabilitiesCpu *cpu,
                                          GVirConfigCapabilitiesCpuTopology *topology)
{
    g_return_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU(cpu));
    g_return_if_fail(topology == NULL || GVIR_CONFIG_IS_CAPABILITIES_CPU_TOPOLOGY(topology));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(cpu),
                                      "topology",
                                      GVIR_CONFIG_OBJECT(topology));
}

/**
 * gvir_config_capabilities_cpu_get_model:
 *
 * Gets the model of the cpu.
 *
 * Returns: (transfer full): a new #GVirConfigCapabilitiesCpuModel.
 */
GVirConfigCapabilitiesCpuModel *
gvir_config_capabilities_cpu_get_model(GVirConfigCapabilitiesCpu *cpu)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU(cpu), NULL);

    object = gvir_config_object_get_child_with_type
                                (GVIR_CONFIG_OBJECT(cpu),
                                 "model",
                                 GVIR_CONFIG_TYPE_CAPABILITIES_CPU_MODEL);

    return GVIR_CONFIG_CAPABILITIES_CPU_MODEL(object);
}
