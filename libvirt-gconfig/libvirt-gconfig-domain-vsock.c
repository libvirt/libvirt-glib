/*
 * libvirt-gconfig-domain-vsock.c: libvirt domain Vsock configuration
 *
 * Copyright (C) 2024 Red Hat, Inc.
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
 * Author: Felipe Borges <felipeborges@gnome.org>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_VSOCK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_VSOCK, GVirConfigDomainVsockPrivate))

struct _GVirConfigDomainVsockPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainVsock, gvir_config_domain_vsock, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_vsock_class_init(GVirConfigDomainVsockClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_domain_vsock_init(GVirConfigDomainVsock *vsock)
{
    vsock->priv = GVIR_CONFIG_DOMAIN_VSOCK_GET_PRIVATE(vsock);
}

GVirConfigDomainVsock *gvir_config_domain_vsock_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_VSOCK,
                                    "vsock", NULL);
    gvir_config_object_set_attribute(object, "model", "virtio", NULL);
    return GVIR_CONFIG_DOMAIN_VSOCK(object);
}

void
gvir_config_domain_vsock_set_cid_address(GVirConfigDomainVsock *vsock,
                                         unsigned int address)
{
    GVirConfigObject *child;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_VSOCK(vsock));

    child = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(vsock), "cid");

    gvir_config_object_set_attribute_with_type(child,
                                               "auto", G_TYPE_STRING, "no",
                                               "address", G_TYPE_UINT, address,
                                               NULL);
    g_object_unref(G_OBJECT(child));
}
