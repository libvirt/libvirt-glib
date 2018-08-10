/*
 * libvirt-gconfig-interface.c: libvirt interface configuration
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

#define GVIR_CONFIG_INTERFACE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_INTERFACE, GVirConfigInterfacePrivate))

struct _GVirConfigInterfacePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigInterface, gvir_config_interface, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_interface_class_init(GVirConfigInterfaceClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_interface_init(GVirConfigInterface *iface)
{
    iface->priv = GVIR_CONFIG_INTERFACE_GET_PRIVATE(iface);
}


GVirConfigInterface *gvir_config_interface_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_INTERFACE,
                                    "interface",
                                    DATADIR "/libvirt/schemas/interface.rng");
    return GVIR_CONFIG_INTERFACE(object);
}

GVirConfigInterface *gvir_config_interface_new_from_xml(const gchar *xml,
                                                        GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_INTERFACE,
                                             "interface",
                                             DATADIR "/libvirt/schemas/interface.rng",
                                             xml, error);
    return GVIR_CONFIG_INTERFACE(object);
}
