/*
 * libvirt-gconfig-capabilities-cpu-model.h: libvirt CPU model capabilities
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

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_CAPABILITIES_CPU_MODEL            (gvir_config_capabilities_cpu_model_get_type ())
#define GVIR_CONFIG_CAPABILITIES_CPU_MODEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_MODEL, GVirConfigCapabilitiesCpuModel))
#define GVIR_CONFIG_CAPABILITIES_CPU_MODEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_MODEL, GVirConfigCapabilitiesCpuModelClass))
#define GVIR_CONFIG_IS_CAPABILITIES_CPU_MODEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_MODEL))
#define GVIR_CONFIG_IS_CAPABILITIES_CPU_MODEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_MODEL))
#define GVIR_CONFIG_CAPABILITIES_CPU_MODEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_MODEL, GVirConfigCapabilitiesCpuModelClass))

typedef struct _GVirConfigCapabilitiesCpuModel GVirConfigCapabilitiesCpuModel;
typedef struct _GVirConfigCapabilitiesCpuModelPrivate GVirConfigCapabilitiesCpuModelPrivate;
typedef struct _GVirConfigCapabilitiesCpuModelClass GVirConfigCapabilitiesCpuModelClass;

struct _GVirConfigCapabilitiesCpuModel
{
    GVirConfigObject parent;

    GVirConfigCapabilitiesCpuModelPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigCapabilitiesCpuModelClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_capabilities_cpu_model_get_type(void);
GVirConfigCapabilitiesCpuModel *gvir_config_capabilities_cpu_model_new(void);
GVirConfigCapabilitiesCpuModel *
gvir_config_capabilities_cpu_model_new_from_xml(const gchar *xml, GError **error);

void
gvir_config_capabilities_cpu_model_set_name(GVirConfigCapabilitiesCpuModel *model,
                                            const gchar *name);
const gchar *
gvir_config_capabilities_cpu_model_get_name(GVirConfigCapabilitiesCpuModel *model);

G_END_DECLS
