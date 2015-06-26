/*
 * libvirt-gobject-network-dhcp-lease-private.h: libvirt gobject integration
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

#ifndef __LIBVIRT_GOBJECT_NETWORK_DHCP_LEASE_PRIVATE_H__
#define __LIBVIRT_GOBJECT_NETWORK_DHCP_LEASE_PRIVATE_H__

G_BEGIN_DECLS

GVirNetworkDHCPLease *gvir_network_dhcp_lease_new(virNetworkDHCPLeasePtr handle);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_NETWORK_DHCP_LEASE_PRIVATE_H__ */
