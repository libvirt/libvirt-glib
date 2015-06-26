/*
 * libvirt-gobject.h: libvirt gobject integration
 *
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

#ifndef __LIBVIRT_GOBJECT_H__
#define __LIBVIRT_GOBJECT_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <libvirt-gconfig/libvirt-gconfig.h>

#include <libvirt-gobject/libvirt-gobject-main.h>
#include <libvirt-gobject/libvirt-gobject-enums.h>
#include <libvirt-gobject/libvirt-gobject-stream.h>
#include <libvirt-gobject/libvirt-gobject-domain-device.h>
#include <libvirt-gobject/libvirt-gobject-domain-disk.h>
#include <libvirt-gobject/libvirt-gobject-domain-interface.h>
#include <libvirt-gobject/libvirt-gobject-domain-snapshot.h>
#include <libvirt-gobject/libvirt-gobject-domain.h>
#include <libvirt-gobject/libvirt-gobject-interface.h>
#include <libvirt-gobject/libvirt-gobject-network.h>
#include <libvirt-gobject/libvirt-gobject-network-dhcp-lease.h>
#include <libvirt-gobject/libvirt-gobject-network-filter.h>
#include <libvirt-gobject/libvirt-gobject-node-device.h>
#include <libvirt-gobject/libvirt-gobject-secret.h>
#include <libvirt-gobject/libvirt-gobject-storage-vol.h>
#include <libvirt-gobject/libvirt-gobject-storage-pool.h>
#include <libvirt-gobject/libvirt-gobject-connection.h>
#include <libvirt-gobject/libvirt-gobject-manager.h>
#include <libvirt-gobject/libvirt-gobject-stream.h>

#endif /* __LIBVIRT_GOBJECT_H__ */
