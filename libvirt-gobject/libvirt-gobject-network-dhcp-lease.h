/*
 * libvirt-gobject-network-dhcp-lease.h: libvirt gobject integration
 *
 * Copyright (C) 2010-2011 Red Hat, Inc.
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

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

/**
 * GVirIPAddrType:
 * @GVIR_IP_ADDR_TYPE_IPV4: IPv4 Address.
 * @GVIR_IP_ADDR_TYPE_IPV6: IPv6 Address.
 */
typedef enum {
    GVIR_IP_ADDR_TYPE_IPV4 =   0,
    GVIR_IP_ADDR_TYPE_IPV6 =   1
} GVirIPAddrType;

#define GVIR_TYPE_NETWORK_DHCP_LEASE            (gvir_network_dhcp_lease_get_type ())
#define GVIR_NETWORK_DHCP_LEASE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_NETWORK_DHCP_LEASE, GVirNetworkDHCPLease))
#define GVIR_NETWORK_DHCP_LEASE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_NETWORK_DHCP_LEASE, GVirNetworkDHCPLeaseClass))
#define GVIR_IS_NETWORK_DHCP_LEASE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_NETWORK_DHCP_LEASE))
#define GVIR_IS_NETWORK_DHCP_LEASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_NETWORK_DHCP_LEASE))
#define GVIR_NETWORK_DHCP_LEASE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_NETWORK_DHCP_LEASE, GVirNetworkDHCPLeaseClass))

typedef struct _GVirNetworkDHCPLease GVirNetworkDHCPLease;
typedef struct _GVirNetworkDHCPLeasePrivate GVirNetworkDHCPLeasePrivate;
typedef struct _GVirNetworkDHCPLeaseClass GVirNetworkDHCPLeaseClass;

struct _GVirNetworkDHCPLease
{
    GObject parent;

    GVirNetworkDHCPLeasePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirNetworkDHCPLeaseClass
{
    GObjectClass parent_class;

    gpointer padding[7];
};

GType gvir_network_dhcp_lease_get_type(void);

const gchar *gvir_network_dhcp_lease_get_iface(GVirNetworkDHCPLease *lease);
gint64 gvir_network_dhcp_lease_get_expiry_time(GVirNetworkDHCPLease *lease);
gint gvir_network_dhcp_lease_get_ip_type(GVirNetworkDHCPLease *lease);
const gchar *gvir_network_dhcp_lease_get_mac(GVirNetworkDHCPLease *lease);
const gchar *gvir_network_dhcp_lease_get_iaid(GVirNetworkDHCPLease *lease);
const gchar *gvir_network_dhcp_lease_get_ip(GVirNetworkDHCPLease *lease);
guint gvir_network_dhcp_lease_get_prefix(GVirNetworkDHCPLease *lease);
const gchar *gvir_network_dhcp_lease_get_hostname(GVirNetworkDHCPLease *lease);
const gchar *gvir_network_dhcp_lease_get_client_id(GVirNetworkDHCPLease *lease);

G_END_DECLS
