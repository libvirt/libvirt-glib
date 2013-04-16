/*
 * libvirt-gconfig-domain-snapshot-private.h: libvirt domain snapshot private methods
 *
 * Copyright (C) 2010-2013 Red Hat, Inc.
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_SNAPSHOT_PRIVATE_H__
#define __LIBVIRT_GCONFIG_DOMAIN_SNAPSHOT_PRIVATE_H__

#include <libvirt-gconfig/libvirt-gconfig-domain.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-snapshot-disk.h>

G_BEGIN_DECLS
GVirConfigDomainSnapshotDisk *gvir_config_domain_snapshot_disk_new_from_tree(GVirConfigXmlDoc *doc,
                                                                             xmlNodePtr tree);
G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_SNAPSHOT_PRIVATE_H__ */
