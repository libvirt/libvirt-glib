/*
 * libvirt-gconfig-capabilities-cpu-feature.c: libvirt CPU feature capabilities
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

#define GVIR_CONFIG_CAPABILITIES_CPU_FEATURE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_FEATURE, GVirConfigCapabilitiesCpuFeaturePrivate))

struct _GVirConfigCapabilitiesCpuFeaturePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigCapabilitiesCpuFeature, gvir_config_capabilities_cpu_feature, GVIR_CONFIG_TYPE_OBJECT);

static void gvir_config_capabilities_cpu_feature_class_init(GVirConfigCapabilitiesCpuFeatureClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_capabilities_cpu_feature_init(GVirConfigCapabilitiesCpuFeature *feature)
{
    feature->priv = GVIR_CONFIG_CAPABILITIES_CPU_FEATURE_GET_PRIVATE(feature);
}

const gchar *
gvir_config_capabilities_cpu_feature_get_name(GVirConfigCapabilitiesCpuFeature *feature)
{
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(feature),
                                            NULL,
                                            "name");
}

void
gvir_config_capabilities_cpu_feature_set_name(GVirConfigCapabilitiesCpuFeature *feature,
                                              const gchar *name)
{
    g_return_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU_FEATURE(feature));
    g_return_if_fail(name != NULL);

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(feature),
                                     "name", name,
                                     NULL);
}
