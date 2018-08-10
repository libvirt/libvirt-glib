/*
 * libvirt-gconfig-domain-cpu-feature.c: libvirt domain CPU feature
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

#define GVIR_CONFIG_DOMAIN_CPU_FEATURE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE, GVirConfigDomainCpuFeaturePrivate))

struct _GVirConfigDomainCpuFeaturePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainCpuFeature, gvir_config_domain_cpu_feature, GVIR_CONFIG_TYPE_CAPABILITIES_CPU_FEATURE);

static void gvir_config_domain_cpu_feature_class_init(GVirConfigDomainCpuFeatureClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_domain_cpu_feature_init(GVirConfigDomainCpuFeature *feature)
{
    feature->priv = GVIR_CONFIG_DOMAIN_CPU_FEATURE_GET_PRIVATE(feature);
}

GVirConfigDomainCpuFeature *gvir_config_domain_cpu_feature_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE,
                                    "feature",
                                    NULL);

    return GVIR_CONFIG_DOMAIN_CPU_FEATURE(object);
}

GVirConfigDomainCpuFeature *
gvir_config_domain_cpu_feature_new_from_xml(const gchar *xml, GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE,
                                             "feature",
                                             NULL,
                                             xml,
                                             error);

    return GVIR_CONFIG_DOMAIN_CPU_FEATURE(object);
}

GVirConfigDomainCpuFeaturePolicy
gvir_config_domain_cpu_feature_get_policy(GVirConfigDomainCpuFeature *feature)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_CPU_FEATURE(feature),
                         GVIR_CONFIG_DOMAIN_CPU_FEATURE_POLICY_REQUIRE);

    return gvir_config_object_get_attribute_genum
                (GVIR_CONFIG_OBJECT(feature),
                 NULL,
                 "policy",
                 GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE_POLICY,
                 GVIR_CONFIG_DOMAIN_CPU_FEATURE_POLICY_REQUIRE);
}

void gvir_config_domain_cpu_feature_set_policy(GVirConfigDomainCpuFeature *feature,
                                               GVirConfigDomainCpuFeaturePolicy policy)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CPU_FEATURE(feature));

    gvir_config_object_set_attribute_with_type
                (GVIR_CONFIG_OBJECT(feature),
                 "policy", GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE_POLICY, policy,
                 NULL);
}
