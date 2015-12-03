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
#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
#include "libvirt-gobject/libvirt-gobject-network-dhcp-lease-private.h"
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */

#define GVIR_NETWORK_DHCP_LEASE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_NETWORK_DHCP_LEASE, GVirNetworkDHCPLeasePrivate))

struct _GVirNetworkDHCPLeasePrivate
{
#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    virNetworkDHCPLeasePtr handle;
#else
    void *handle;
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
};

G_DEFINE_TYPE(GVirNetworkDHCPLease, gvir_network_dhcp_lease, G_TYPE_OBJECT);

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
#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
        if (priv->handle)
            virNetworkDHCPLeaseFree(priv->handle);
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
        priv->handle = g_value_get_pointer(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_network_dhcp_lease_finalize(GObject *object)
{
    GVirNetworkDHCPLease *lease = GVIR_NETWORK_DHCP_LEASE(object);
#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    GVirNetworkDHCPLeasePrivate *priv = lease->priv;
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */

#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    virNetworkDHCPLeaseFree(priv->handle);
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */

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
                                                         G_PARAM_PRIVATE |
                                                         G_PARAM_STATIC_STRINGS));

    g_type_class_add_private(klass, sizeof(GVirNetworkDHCPLeasePrivate));
}


static void gvir_network_dhcp_lease_init(GVirNetworkDHCPLease *lease)
{
    lease->priv = GVIR_NETWORK_DHCP_LEASE_GET_PRIVATE(lease);
}

#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
GVirNetworkDHCPLease *gvir_network_dhcp_lease_new(virNetworkDHCPLeasePtr handle)
{
    return g_object_new(GVIR_TYPE_NETWORK_DHCP_LEASE,
                        "handle", handle,
                        NULL);
}
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */

/**
 * gvir_network_dhcp_lease_get_iface:
 * @lease: the lease
 *
 * Returns: (transfer none): The network interface name.
 */
const gchar *gvir_network_dhcp_lease_get_iface(GVirNetworkDHCPLease *lease)
{
    g_return_val_if_fail(GVIR_IS_NETWORK_DHCP_LEASE(lease), NULL);

#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    return lease->priv->handle->iface;
#else /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
    return NULL;
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
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

#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    return lease->priv->handle->expirytime;
#else /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
    return -1;
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
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

#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    return lease->priv->handle->type;
#else /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
    return -1;
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
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

#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    return lease->priv->handle->mac;
#else /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
    return NULL;
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
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

#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    return lease->priv->handle->iaid;
#else /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
    return NULL;
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
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

#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    return lease->priv->handle->ipaddr;
#else /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
    return NULL;
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
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

#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    return lease->priv->handle->prefix;
#else /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
    return 0;
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
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

#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    return lease->priv->handle->hostname;
#else /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
    return NULL;
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
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

#ifdef HAVE_VIR_NETWORK_GET_DHCP_LEASES
    return lease->priv->handle->clientid;
#else /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
    return NULL;
#endif /* HAVE_VIR_NETWORK_GET_DHCP_LEASES */
}
