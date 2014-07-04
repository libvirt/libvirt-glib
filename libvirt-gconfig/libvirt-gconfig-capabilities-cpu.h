/*
 * libvirt-gconfig-capabilities-cpu.h: libvirt CPU capabilities
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

#ifndef __LIBVIRT_GCONFIG_CAPABILITIES_CPU_H__
#define __LIBVIRT_GCONFIG_CAPABILITIES_CPU_H__

#include "libvirt-gconfig-capabilities-cpu-topology.h"
#include "libvirt-gconfig-capabilities-cpu-feature.h"
#include "libvirt-gconfig-capabilities-cpu-model.h"

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_CAPABILITIES_CPU            (gvir_config_capabilities_cpu_get_type ())
#define GVIR_CONFIG_CAPABILITIES_CPU(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU, GVirConfigCapabilitiesCpu))
#define GVIR_CONFIG_CAPABILITIES_CPU_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_CPU, GVirConfigCapabilitiesCpuClass))
#define GVIR_CONFIG_IS_CAPABILITIES_CPU(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU))
#define GVIR_CONFIG_IS_CAPABILITIES_CPU_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_CPU))
#define GVIR_CONFIG_CAPABILITIES_CPU_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU, GVirConfigCapabilitiesCpuClass))

typedef struct _GVirConfigCapabilitiesCpu GVirConfigCapabilitiesCpu;
typedef struct _GVirConfigCapabilitiesCpuPrivate GVirConfigCapabilitiesCpuPrivate;
typedef struct _GVirConfigCapabilitiesCpuClass GVirConfigCapabilitiesCpuClass;

struct _GVirConfigCapabilitiesCpu
{
    GVirConfigObject parent;

    GVirConfigCapabilitiesCpuPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigCapabilitiesCpuClass
{
    GVirConfigObjectClass parent_class;

    GList *(*get_features)(GVirConfigCapabilitiesCpu *cpu);

    gpointer padding[19];
};

GType gvir_config_capabilities_cpu_get_type(void);

const gchar *
gvir_config_capabilities_cpu_get_arch(GVirConfigCapabilitiesCpu *cpu);
void gvir_config_capabilities_cpu_add_feature(GVirConfigCapabilitiesCpu *cpu,
                                              GVirConfigCapabilitiesCpuFeature *feature);
GList *
gvir_config_capabilities_cpu_get_features(GVirConfigCapabilitiesCpu *cpu);
GVirConfigCapabilitiesCpuTopology *
gvir_config_capabilities_cpu_get_topology(GVirConfigCapabilitiesCpu *cpu);
void
gvir_config_capabilities_cpu_set_topology(GVirConfigCapabilitiesCpu *cpu,
                                          GVirConfigCapabilitiesCpuTopology *topology);
GVirConfigCapabilitiesCpuModel *
gvir_config_capabilities_cpu_get_model(GVirConfigCapabilitiesCpu *cpu);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_CAPABILITIES_CPU_H__ */
