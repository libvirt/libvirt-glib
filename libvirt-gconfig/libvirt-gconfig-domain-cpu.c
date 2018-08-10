/*
 * libvirt-gconfig-domain-cpu.c: libvirt Domain CPU
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

#define GVIR_CONFIG_DOMAIN_CPU_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU, GVirConfigDomainCpuPrivate))

struct _GVirConfigDomainCpuPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainCpu, gvir_config_domain_cpu, GVIR_CONFIG_TYPE_CAPABILITIES_CPU);

static GList *
_gvir_config_domain_cpu_get_features(GVirConfigCapabilitiesCpu *cpu);

static void gvir_config_domain_cpu_class_init(GVirConfigDomainCpuClass *klass)
{
    GVirConfigCapabilitiesCpuClass *capabilities_class;

    capabilities_class = GVIR_CONFIG_CAPABILITIES_CPU_CLASS(klass);
    capabilities_class->get_features = _gvir_config_domain_cpu_get_features;
}

static void gvir_config_domain_cpu_init(GVirConfigDomainCpu *cpu)
{
    cpu->priv = GVIR_CONFIG_DOMAIN_CPU_GET_PRIVATE(cpu);
}

GVirConfigDomainCpu *gvir_config_domain_cpu_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CPU, "cpu", NULL);

    return GVIR_CONFIG_DOMAIN_CPU(object);
}

GVirConfigDomainCpu *gvir_config_domain_cpu_new_from_xml(const gchar *xml,
                                                         GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CPU,
                                             "cpu",
                                             NULL,
                                             xml,
                                             error);

    return GVIR_CONFIG_DOMAIN_CPU(object);
}

static GList *
_gvir_config_domain_cpu_get_features(GVirConfigCapabilitiesCpu *cpu)
{
    return gvir_config_capabilities_cpu_get_features_with_type(
                        cpu, GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE);
}

GVirConfigDomainCpuMatchPolicy
gvir_config_domain_cpu_get_match_policy(GVirConfigDomainCpu *cpu)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_CPU(cpu),
                         GVIR_CONFIG_DOMAIN_CPU_MATCH_POLICY_EXACT);

    return gvir_config_object_get_attribute_genum
                (GVIR_CONFIG_OBJECT(cpu),
                 NULL,
                 "match",
                 GVIR_CONFIG_TYPE_DOMAIN_CPU_MATCH_POLICY,
                 GVIR_CONFIG_DOMAIN_CPU_MATCH_POLICY_EXACT);
}

void gvir_config_domain_cpu_set_match_policy(GVirConfigDomainCpu *cpu,
                                             GVirConfigDomainCpuMatchPolicy policy)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CPU(cpu));

    gvir_config_object_set_attribute_with_type
                (GVIR_CONFIG_OBJECT(cpu),
                 "match", GVIR_CONFIG_TYPE_DOMAIN_CPU_MATCH_POLICY, policy,
                 NULL);
}

GVirConfigDomainCpuMode
gvir_config_domain_cpu_get_mode(GVirConfigDomainCpu *cpu)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_CPU(cpu),
                         GVIR_CONFIG_DOMAIN_CPU_MODE_CUSTOM);

    return gvir_config_object_get_attribute_genum
                (GVIR_CONFIG_OBJECT(cpu),
                 NULL,
                 "mode",
                 GVIR_CONFIG_TYPE_DOMAIN_CPU_MODE,
                 GVIR_CONFIG_DOMAIN_CPU_MODE_CUSTOM);
}

void gvir_config_domain_cpu_set_mode(GVirConfigDomainCpu *cpu,
                                     GVirConfigDomainCpuMode mode)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CPU(cpu));

    gvir_config_object_set_attribute_with_type
                (GVIR_CONFIG_OBJECT(cpu),
                 "mode", GVIR_CONFIG_TYPE_DOMAIN_CPU_MODE, mode,
                 NULL);
}

void gvir_config_domain_cpu_set_model(GVirConfigDomainCpu *cpu,
                                      GVirConfigDomainCpuModel *model)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CPU(cpu));
    g_return_if_fail(model == NULL || GVIR_CONFIG_IS_DOMAIN_CPU_MODEL(model));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(cpu),
                                      "model",
                                      GVIR_CONFIG_OBJECT(model));
}
