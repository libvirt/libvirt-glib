/*
 * libvirt-gconfig-capabilities-cpu-topology.c: libvirt CPU topology capabilities
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

#define GVIR_CONFIG_CAPABILITIES_CPU_TOPOLOGY_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_TOPOLOGY, GVirConfigCapabilitiesCpuTopologyPrivate))

struct _GVirConfigCapabilitiesCpuTopologyPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigCapabilitiesCpuTopology, gvir_config_capabilities_cpu_topology, GVIR_CONFIG_TYPE_OBJECT);

static void gvir_config_capabilities_cpu_topology_class_init(GVirConfigCapabilitiesCpuTopologyClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_capabilities_cpu_topology_init(GVirConfigCapabilitiesCpuTopology *topology)
{
    topology->priv = GVIR_CONFIG_CAPABILITIES_CPU_TOPOLOGY_GET_PRIVATE(topology);
}

GVirConfigCapabilitiesCpuTopology *gvir_config_capabilities_cpu_topology_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_CAPABILITIES_CPU_TOPOLOGY,
                                    "topology",
                                    NULL);

    return GVIR_CONFIG_CAPABILITIES_CPU_TOPOLOGY(object);
}

GVirConfigCapabilitiesCpuTopology *
gvir_config_capabilities_cpu_topology_new_from_xml(const gchar *xml, GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_CAPABILITIES_CPU_TOPOLOGY,
                                             "topology",
                                             NULL,
                                             xml,
                                             error);

    return GVIR_CONFIG_CAPABILITIES_CPU_TOPOLOGY(object);
}

guint64
gvir_config_capabilities_cpu_topology_get_sockets(GVirConfigCapabilitiesCpuTopology *topology)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU_TOPOLOGY(topology), 0);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(topology),
                                                   NULL,
                                                   "sockets",
                                                   0);
}

void
gvir_config_capabilities_cpu_topology_set_sockets(GVirConfigCapabilitiesCpuTopology *topology,
                                                  guint64 sockets)
{
    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(topology),
                                               "sockets", G_TYPE_UINT64, sockets,
                                               NULL);
}

guint64
gvir_config_capabilities_cpu_topology_get_cores(GVirConfigCapabilitiesCpuTopology *topology)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU_TOPOLOGY(topology), 0);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(topology),
                                                   NULL,
                                                   "cores",
                                                   0);
}

void
gvir_config_capabilities_cpu_topology_set_cores(GVirConfigCapabilitiesCpuTopology *topology,
                                                guint64 cores)
{
    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(topology),
                                               "cores", G_TYPE_UINT64, cores,
                                               NULL);
}

guint64
gvir_config_capabilities_cpu_topology_get_threads(GVirConfigCapabilitiesCpuTopology *topology)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU_TOPOLOGY(topology), 0);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(topology),
                                                   NULL,
                                                   "threads",
                                                   0);
}

void
gvir_config_capabilities_cpu_topology_set_threads(GVirConfigCapabilitiesCpuTopology *topology,
                                                  guint64 threads)
{
    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(topology),
                                               "threads", G_TYPE_UINT64, threads,
                                               NULL);
}
