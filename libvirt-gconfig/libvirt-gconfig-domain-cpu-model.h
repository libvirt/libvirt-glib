/*
 * libvirt-gconfig-domain-cpu-model.h: libvirt domain CPU model
 *
 * Copyright (C) 2014 Red Hat, Inc.
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
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CPU_MODEL_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CPU_MODEL_H__

#include "libvirt-gconfig-capabilities-cpu-model.h"

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CPU_MODEL            (gvir_config_domain_cpu_model_get_type ())
#define GVIR_CONFIG_DOMAIN_CPU_MODEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU_MODEL, GVirConfigDomainCpuModel))
#define GVIR_CONFIG_DOMAIN_CPU_MODEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CPU_MODEL, GVirConfigDomainCpuModelClass))
#define GVIR_CONFIG_IS_DOMAIN_CPU_MODEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU_MODEL))
#define GVIR_CONFIG_IS_DOMAIN_CPU_MODEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CPU_MODEL))
#define GVIR_CONFIG_DOMAIN_CPU_MODEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU_MODEL, GVirConfigDomainCpuModelClass))

typedef struct _GVirConfigDomainCpuModel GVirConfigDomainCpuModel;
typedef struct _GVirConfigDomainCpuModelPrivate GVirConfigDomainCpuModelPrivate;
typedef struct _GVirConfigDomainCpuModelClass GVirConfigDomainCpuModelClass;

struct _GVirConfigDomainCpuModel
{
    GVirConfigCapabilitiesCpuModel parent;

    GVirConfigDomainCpuModelPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainCpuModelClass
{
    GVirConfigCapabilitiesCpuModelClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_cpu_model_get_type(void);
GVirConfigDomainCpuModel *gvir_config_domain_cpu_model_new(void);
GVirConfigDomainCpuModel *
gvir_config_domain_cpu_model_new_from_xml(const gchar *xml, GError **error);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CPU_MODEL_H__ */
