/*
 * libvirt-gobject-network-dhcp-lease.c: libvirt glib integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2015 Red Hat, Inc.
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
 * Authors: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *          Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-compat.h"
#include "libvirt-gobject/libvirt-gobject-network-dhcp-lease-private.h"

#define GVIR_NETWORK_DHCP_LEASE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_NETWORK_DHCP_LEASE, GVirNetworkDHCPLeasePrivate))

struct _GVirNetworkDHCPLeasePrivate
{
    virNetworkDHCPLeasePtr handle;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirNetworkDHCPLease, gvir_network_dhcp_lease, G_TYPE_OBJECT);

enum {
    PROP_0,
    PROP_HANDLE,
};

static void gvir_network_dhcp_lease_get_property(GObject *object,
                                                 guint prop_id,
                                                 GValue *value,
                                                 GParamSpec *pspec)
{
    GVirNetworkDHCPLease *lease = GVIR_NETWORK_DHCP_LEASE(object);
    GVirNetworkDHCPLeasePrivate *priv = lease->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_pointer(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void gvir_network_dhcp_lease_set_property(GObject *object,
                                                 guint prop_id,
                                                 const GValue *value,
                                                 GParamSpec *pspec)
{
    GVirNetworkDHCPLease *lease = GVIR_NETWORK_DHCP_LEASE(object);
    GVirNetworkDHCPLeasePrivate *priv = lease->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virNetworkDHCPLeaseFree(priv->handle);
        priv->handle = g_value_get_pointer(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_network_dhcp_lease_finalize(GObject *object)
{
    GVirNetworkDHCPLease *lease = GVIR_NETWORK_DHCP_LEASE(object);
    GVirNetworkDHCPLeasePrivate *priv = lease->priv;

    virNetworkDHCPLeaseFree(priv->handle);

    G_OBJECT_CLASS(gvir_network_dhcp_lease_parent_class)->finalize(object);
}

static void gvir_network_dhcp_lease_class_init(GVirNetworkDHCPLeaseClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_network_dhcp_lease_finalize;
    object_class->get_property = gvir_network_dhcp_lease_get_property;
    object_class->set_property = gvir_network_dhcp_lease_set_property;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_pointer("handle",
                                                         "Handle",
                                                         "The lease handle",
                                                         G_PARAM_READWRITE |
                                                         G_PARAM_CONSTRUCT_ONLY |
                                                         G_PARAM_STATIC_STRINGS));
}


static void gvir_network_dhcp_lease_init(GVirNetworkDHCPLease *lease)
{
    lease->priv = GVIR_NETWORK_DHCP_LEASE_GET_PRIVATE(lease);
}

GVirNetworkDHCPLease *gvir_network_dhcp_lease_new(virNetworkDHCPLeasePtr handle)
{
    return g_object_new(GVIR_TYPE_NETWORK_DHCP_LEASE,
                        "handle", handle,
                        NULL);
}

/**
 * gvir_network_dhcp_lease_get_iface:
 * @lease: the lease
 *
 * Returns: (transfer none): The network interface name.
 */
const gchar *gvir_network_dhcp_lease_get_iface(GVirNetworkDHCPLease *lease)
{
    g_return_val_if_fail(GVIR_IS_NETWORK_DHCP_LEASE(lease), NULL);

    return lease->priv->handle->iface;
}

/**
 * gvir_network_dhcp_lease_get_expiry_time:
 * @lease: the lease
 *
 * Returns: The expiry time of this lease, as seconds since epoch.
 */
gint64 gvir_network_dhcp_lease_get_expiry_time(GVirNetworkDHCPLease *lease)
{
    g_return_val_if_fail(GVIR_IS_NETWORK_DHCP_LEASE(lease), -1);

    return lease->priv->handle->expirytime;
}

/**
 * gvir_network_dhcp_lease_get_ip_type:
 * @lease: the lease
 *
 * Returns: The type of IP, see %GVirIPAddrType for possible values.
 */
gint gvir_network_dhcp_lease_get_ip_type(GVirNetworkDHCPLease *lease)
{
    g_return_val_if_fail(GVIR_IS_NETWORK_DHCP_LEASE(lease), -1);

    return lease->priv->handle->type;
}

/**
 * gvir_network_dhcp_lease_get_mac:
 * @lease: the lease
 *
 * Returns: (transfer none): The MAC address.
 */
const gchar *gvir_network_dhcp_lease_get_mac(GVirNetworkDHCPLease *lease)
{
    g_return_val_if_fail(GVIR_IS_NETWORK_DHCP_LEASE(lease), NULL);

    return lease->priv->handle->mac;
}

/**
 * gvir_network_dhcp_lease_get_iaid:
 * @lease: the lease
 *
 * Returns: (transfer none): The IAID.
 */
const gchar *gvir_network_dhcp_lease_get_iaid(GVirNetworkDHCPLease *lease)
{
    g_return_val_if_fail(GVIR_IS_NETWORK_DHCP_LEASE(lease), NULL);

    return lease->priv->handle->iaid;
}

/**
 * gvir_network_dhcp_lease_get_ip:
 * @lease: the lease
 *
 * Returns: (transfer none): The IP address.
 */
const gchar *gvir_network_dhcp_lease_get_ip(GVirNetworkDHCPLease *lease)
{
    g_return_val_if_fail(GVIR_IS_NETWORK_DHCP_LEASE(lease), NULL);

    return lease->priv->handle->ipaddr;
}

/**
 * gvir_network_dhcp_lease_get_prefix:
 * @lease: the lease
 *
 * Returns: The number of network address bits in the IP address.
 */
guint gvir_network_dhcp_lease_get_prefix(GVirNetworkDHCPLease *lease)
{
    g_return_val_if_fail(GVIR_IS_NETWORK_DHCP_LEASE(lease), 0);

    return lease->priv->handle->prefix;
}

/**
 * gvir_network_dhcp_lease_get_hostname:
 * @lease: the lease
 *
 * Returns: (transfer none): The hostname.
 */
const gchar *gvir_network_dhcp_lease_get_hostname(GVirNetworkDHCPLease *lease)
{
    g_return_val_if_fail(GVIR_IS_NETWORK_DHCP_LEASE(lease), NULL);

    return lease->priv->handle->hostname;
}

/**
 * gvir_network_dhcp_lease_get_client_id:
 * @lease: the lease
 *
 * Returns: (transfer none): The client ID or DUID.
 */
const gchar *gvir_network_dhcp_lease_get_client_id(GVirNetworkDHCPLease *lease)
{
    g_return_val_if_fail(GVIR_IS_NETWORK_DHCP_LEASE(lease), NULL);

    return lease->priv->handle->clientid;
}
