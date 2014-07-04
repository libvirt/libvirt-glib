/*
 * libvirt-gconfig-domain-cpu.h: libvirt Domain CPU
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

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CPU_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CPU_H__

#include <libvirt-gconfig/libvirt-gconfig-domain-cpu-model.h>

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CPU            (gvir_config_domain_cpu_get_type ())
#define GVIR_CONFIG_DOMAIN_CPU(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU, GVirConfigDomainCpu))
#define GVIR_CONFIG_DOMAIN_CPU_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CPU, GVirConfigDomainCpuClass))
#define GVIR_CONFIG_IS_DOMAIN_CPU(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU))
#define GVIR_CONFIG_IS_DOMAIN_CPU_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CPU))
#define GVIR_CONFIG_DOMAIN_CPU_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU, GVirConfigDomainCpuClass))

typedef struct _GVirConfigDomainCpu GVirConfigDomainCpu;
typedef struct _GVirConfigDomainCpuPrivate GVirConfigDomainCpuPrivate;
typedef struct _GVirConfigDomainCpuClass GVirConfigDomainCpuClass;

struct _GVirConfigDomainCpu
{
    GVirConfigCapabilitiesCpu parent;

    GVirConfigDomainCpuPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainCpuClass
{
    GVirConfigCapabilitiesCpuClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_CPU_MATCH_POLICY_MINIMUM,
    GVIR_CONFIG_DOMAIN_CPU_MATCH_POLICY_EXACT,
    GVIR_CONFIG_DOMAIN_CPU_MATCH_POLICY_STRICT
} GVirConfigDomainCpuMatchPolicy;

typedef enum {
    GVIR_CONFIG_DOMAIN_CPU_MODE_CUSTOM,
    GVIR_CONFIG_DOMAIN_CPU_MODE_HOST_MODEL,
    GVIR_CONFIG_DOMAIN_CPU_MODE_HOST_PASSTHROUGH
} GVirConfigDomainCpuMode;

GType gvir_config_domain_cpu_get_type(void);
GVirConfigDomainCpu *gvir_config_domain_cpu_new(void);
GVirConfigDomainCpu *gvir_config_domain_cpu_new_from_xml(const gchar *xml,
                                                         GError **error);
void
gvir_config_domain_cpu_set_match_policy(GVirConfigDomainCpu *cpu,
                                        GVirConfigDomainCpuMatchPolicy policy);
GVirConfigDomainCpuMatchPolicy
gvir_config_domain_cpu_get_match_policy(GVirConfigDomainCpu *cpu);
void gvir_config_domain_cpu_set_mode(GVirConfigDomainCpu *cpu,
                                     GVirConfigDomainCpuMode mode);
void gvir_config_domain_cpu_set_model(GVirConfigDomainCpu *cpu,
                                      GVirConfigDomainCpuModel *model);
GVirConfigDomainCpuMode
gvir_config_domain_cpu_get_mode(GVirConfigDomainCpu *cpu);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CPU_H__ */
