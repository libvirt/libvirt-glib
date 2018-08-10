/*
 * libvirt-gconfig-capabilities-cpu-model.c: libvirt CPU model capabilities
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_CAPABILITIES_CPU_MODEL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES_CPU_MODEL, GVirConfigCapabilitiesCpuModelPrivate))

struct _GVirConfigCapabilitiesCpuModelPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigCapabilitiesCpuModel, gvir_config_capabilities_cpu_model, GVIR_CONFIG_TYPE_OBJECT);

static void gvir_config_capabilities_cpu_model_class_init(GVirConfigCapabilitiesCpuModelClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_capabilities_cpu_model_init(GVirConfigCapabilitiesCpuModel *model)
{
    model->priv = GVIR_CONFIG_CAPABILITIES_CPU_MODEL_GET_PRIVATE(model);
}

GVirConfigCapabilitiesCpuModel *gvir_config_capabilities_cpu_model_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_CAPABILITIES_CPU_MODEL,
                                    "model",
                                    NULL);

    return GVIR_CONFIG_CAPABILITIES_CPU_MODEL(object);
}

GVirConfigCapabilitiesCpuModel *
gvir_config_capabilities_cpu_model_new_from_xml(const gchar *xml, GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_CAPABILITIES_CPU_MODEL,
                                             "model",
                                             NULL,
                                             xml,
                                             error);

    return GVIR_CONFIG_CAPABILITIES_CPU_MODEL(object);
}

void
gvir_config_capabilities_cpu_model_set_name(GVirConfigCapabilitiesCpuModel *model,
                                            const gchar *name)
{
    g_return_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU_MODEL(model));

    gvir_config_object_set_node_content
                (GVIR_CONFIG_OBJECT(model),
                 NULL,
                 name);
}

const gchar *
gvir_config_capabilities_cpu_model_get_name(GVirConfigCapabilitiesCpuModel *model)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_CPU_MODEL(model), NULL);

    return gvir_config_object_get_node_content (GVIR_CONFIG_OBJECT(model),
                                                NULL);
}
