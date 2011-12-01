/*
 * libvirt-gobject-config-domain-device.c: libvirt glib integration
 *
 * Copyright (C) 2011 Red Hat
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"

#define GVIR_CONFIG_DOMAIN_DEVICE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_DEVICE, GVirConfigDomainDevicePrivate))

struct _GVirConfigDomainDevicePrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE(GVirConfigDomainDevice, gvir_config_domain_device, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_domain_device_class_init(GVirConfigDomainDeviceClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainDevicePrivate));
}


static void gvir_config_domain_device_init(GVirConfigDomainDevice *device)
{
    g_debug("Init GVirConfigDomainDevice=%p", device);

    device->priv = GVIR_CONFIG_DOMAIN_DEVICE_GET_PRIVATE(device);
}
