/*
 * libvirt-gconfig-network.c: libvirt network configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010-2011 Red Hat, Inc.
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"

#define GVIR_CONFIG_NETWORK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_NETWORK, GVirConfigNetworkPrivate))

struct _GVirConfigNetworkPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigNetwork, gvir_config_network, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_network_class_init(GVirConfigNetworkClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_network_init(GVirConfigNetwork *network)
{
    network->priv = GVIR_CONFIG_NETWORK_GET_PRIVATE(network);
}


GVirConfigNetwork *gvir_config_network_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_NETWORK,
                                    "network",
                                    DATADIR "/libvirt/schemas/network.rng");
    return GVIR_CONFIG_NETWORK(object);
}

GVirConfigNetwork *gvir_config_network_new_from_xml(const gchar *xml,
                                                    GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_NETWORK,
                                             "network",
                                             DATADIR "/libvirt/schemas/network.rng",
                                             xml, error);
    return GVIR_CONFIG_NETWORK(object);
}
