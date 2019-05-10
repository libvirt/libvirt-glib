/*
 * libvirt-gconfig-domain-capabilities-os.c: libvirt OS domain capabilities
 *
 * Copyright (C) 2019 Red Hat, Inc.
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
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_CAPABILITIES_OS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES_OS, GVirConfigDomainCapabilitiesOsPrivate))

struct _GVirConfigDomainCapabilitiesOsPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainCapabilitiesOs, gvir_config_domain_capabilities_os, GVIR_CONFIG_TYPE_OBJECT);

static void gvir_config_domain_capabilities_os_class_init(GVirConfigDomainCapabilitiesOsClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_domain_capabilities_os_init(GVirConfigDomainCapabilitiesOs *os)
{
    os->priv = GVIR_CONFIG_DOMAIN_CAPABILITIES_OS_GET_PRIVATE(os);
}
