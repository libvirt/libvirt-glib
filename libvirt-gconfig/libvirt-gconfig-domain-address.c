/*
 * libvirt-gconfig-domain-address.c: libvirt domain address configuration
 *
 * Copyright (C) 2012 Red Hat, Inc.
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_ADDRESS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS, GVirConfigDomainAddressPrivate))

struct _GVirConfigDomainAddressPrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GVirConfigDomainAddress, gvir_config_domain_address, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_address_class_init(GVirConfigDomainAddressClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_address_init(GVirConfigDomainAddress *address)
{
    address->priv = GVIR_CONFIG_DOMAIN_ADDRESS_GET_PRIVATE(address);
}
