/*
 * libvirt-gconfig-domain-cpu-model.c: libvirt domain CPU model
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

#define GVIR_CONFIG_DOMAIN_CPU_MODEL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CPU_MODEL, GVirConfigDomainCpuModelPrivate))

struct _GVirConfigDomainCpuModelPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainCpuModel,
              gvir_config_domain_cpu_model,
              GVIR_CONFIG_TYPE_CAPABILITIES_CPU_MODEL);

static void gvir_config_domain_cpu_model_class_init(GVirConfigDomainCpuModelClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_domain_cpu_model_init(GVirConfigDomainCpuModel *model)
{
    model->priv = GVIR_CONFIG_DOMAIN_CPU_MODEL_GET_PRIVATE(model);
}

GVirConfigDomainCpuModel *gvir_config_domain_cpu_model_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CPU_MODEL,
                                    "model",
                                    NULL);

    return GVIR_CONFIG_DOMAIN_CPU_MODEL(object);
}

GVirConfigDomainCpuModel *
gvir_config_domain_cpu_model_new_from_xml(const gchar *xml, GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CPU_MODEL,
                                             "model",
                                             NULL,
                                             xml,
                                             error);

    return GVIR_CONFIG_DOMAIN_CPU_MODEL(object);
}
