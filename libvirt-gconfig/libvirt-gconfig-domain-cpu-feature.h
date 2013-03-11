/*
 * libvirt-gconfig-domain-cpu-feature.h: libvirt domain CPU feature
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

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CPU_FEATURE_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CPU_FEATURE_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE            (gvir_config_domain_cpu_feature_get_type ())
#define GVIR_CONFIG_DOMAIN_CPU_FEATURE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE, GVirConfigDomainCpuFeature))
#define GVIR_CONFIG_DOMAIN_CPU_FEATURE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE, GVirConfigDomainCpuFeatureClass))
#define GVIR_CONFIG_IS_DOMAIN_CPU_FEATURE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE))
#define GVIR_CONFIG_IS_DOMAIN_CPU_FEATURE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE))
#define GVIR_CONFIG_DOMAIN_CPU_FEATURE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU_FEATURE, GVirConfigDomainCpuFeatureClass))

typedef struct _GVirConfigDomainCpuFeature GVirConfigDomainCpuFeature;
typedef struct _GVirConfigDomainCpuFeaturePrivate GVirConfigDomainCpuFeaturePrivate;
typedef struct _GVirConfigDomainCpuFeatureClass GVirConfigDomainCpuFeatureClass;

struct _GVirConfigDomainCpuFeature
{
    GVirConfigCapabilitiesCpu parent;

    GVirConfigDomainCpuFeaturePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainCpuFeatureClass
{
    GVirConfigCapabilitiesCpuClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_CPU_FEATURE_POLICY_FORCE,
    GVIR_CONFIG_DOMAIN_CPU_FEATURE_POLICY_REQUIRE,
    GVIR_CONFIG_DOMAIN_CPU_FEATURE_POLICY_OPTIONAL,
    GVIR_CONFIG_DOMAIN_CPU_FEATURE_POLICY_DISABLE,
    GVIR_CONFIG_DOMAIN_CPU_FEATURE_POLICY_FORBID
} GVirConfigDomainCpuFeaturePolicy;

GType gvir_config_domain_cpu_feature_get_type(void);
GVirConfigDomainCpuFeature *gvir_config_domain_cpu_feature_new(void);
GVirConfigDomainCpuFeature *
gvir_config_domain_cpu_feature_new_from_xml(const gchar *xml, GError **error);

void
gvir_config_domain_cpu_feature_set_policy(GVirConfigDomainCpuFeature *feature,
                                          GVirConfigDomainCpuFeaturePolicy policy);
GVirConfigDomainCpuFeaturePolicy
gvir_config_domain_cpu_feature_get_policy(GVirConfigDomainCpuFeature *feature);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CPU_FEATURE_H__ */
