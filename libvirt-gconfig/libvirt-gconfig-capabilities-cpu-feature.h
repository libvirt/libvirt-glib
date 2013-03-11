/*
 * libvirt-gconfig-capabilities-cpu-feature.h: libvirt CPU feature capabilities
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

#ifndef __LIBVIRT_GCONFIG_CAPABILITIES_CPU_FEATURE_H__
#define __LIBVIRT_GCONFIG_CAPABILITIES_CPU_FEATURE_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_CAPABILITIES_CPU_FEATURE            (gvir_config_capabilities_cpu_feature_get_type ())
#define GVIR_CONFIG_CAPABILITIES_CPU_FEATURE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_FEATURE, GVirConfigCapabilitiesCpuFeature))
#define GVIR_CONFIG_CAPABILITIES_CPU_FEATURE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_FEATURE, GVirConfigCapabilitiesCpuFeatureClass))
#define GVIR_CONFIG_IS_CAPABILITIES_CPU_FEATURE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_FEATURE))
#define GVIR_CONFIG_IS_CAPABILITIES_CPU_FEATURE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_FEATURE))
#define GVIR_CONFIG_CAPABILITIES_CPU_FEATURE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_FEATURE, GVirConfigCapabilitiesCpuFeatureClass))

typedef struct _GVirConfigCapabilitiesCpuFeature GVirConfigCapabilitiesCpuFeature;
typedef struct _GVirConfigCapabilitiesCpuFeaturePrivate GVirConfigCapabilitiesCpuFeaturePrivate;
typedef struct _GVirConfigCapabilitiesCpuFeatureClass GVirConfigCapabilitiesCpuFeatureClass;

struct _GVirConfigCapabilitiesCpuFeature
{
    GVirConfigObject parent;

    GVirConfigCapabilitiesCpuFeaturePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigCapabilitiesCpuFeatureClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_capabilities_cpu_feature_get_type(void);

const gchar *
gvir_config_capabilities_cpu_feature_get_name(GVirConfigCapabilitiesCpuFeature *feature);
void
gvir_config_capabilities_cpu_feature_set_name(GVirConfigCapabilitiesCpuFeature *feature,
                                              const gchar *name);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_CAPABILITIES_CPU_FEATURE_H__ */
