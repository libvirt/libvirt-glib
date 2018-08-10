/*
 * libvirt-gconfig-capabilities-host-secmodel.c: libvirt security model capabilities
 *
 * Copyright (C) 2014 SUSE LINUX Products GmbH, Nuernberg, Germany.
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
 * Authors: Cédric Bosdonnat <cbosdonnat@suse.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_CAPABILITIES_HOST_SECMODEL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES_HOST_SECMODEL, GVirConfigCapabilitiesHostSecModelPrivate))

struct _GVirConfigCapabilitiesHostSecModelPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigCapabilitiesHostSecModel, gvir_config_capabilities_host_secmodel, GVIR_CONFIG_TYPE_OBJECT);

static void gvir_config_capabilities_host_secmodel_class_init(GVirConfigCapabilitiesHostSecModelClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_capabilities_host_secmodel_init(GVirConfigCapabilitiesHostSecModel *secmodel)
{
    secmodel->priv = GVIR_CONFIG_CAPABILITIES_HOST_SECMODEL_GET_PRIVATE(secmodel);
}

const gchar *
gvir_config_capabilities_host_secmodel_get_model(GVirConfigCapabilitiesHostSecModel *secmodel)
{
    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(secmodel),
                                               "model");
}

const gchar *
gvir_config_capabilities_host_secmodel_get_doi(GVirConfigCapabilitiesHostSecModel *secmodel)
{
    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(secmodel),
                                               "doi");
}
