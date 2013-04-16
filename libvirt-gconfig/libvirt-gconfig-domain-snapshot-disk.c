/*
 * libvirt-gconfig-secret.c: libvirt secret configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_SNAPSHOT_DISK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT_DISK, GVirConfigDomainSnapshotDiskPrivate))

struct _GVirConfigDomainSnapshotDiskPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainSnapshotDisk, gvir_config_domain_snapshot_disk, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_snapshot_disk_class_init(GVirConfigDomainSnapshotDiskClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainSnapshotDiskPrivate));
}


static void gvir_config_domain_snapshot_disk_init(GVirConfigDomainSnapshotDisk *disk)
{
    g_debug("Init GVirConfigDomainSnapshotDisk=%p", disk);

    disk->priv = GVIR_CONFIG_DOMAIN_SNAPSHOT_DISK_GET_PRIVATE(disk);
}


GVirConfigDomainSnapshotDisk *gvir_config_domain_snapshot_disk_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT_DISK,
                                    "disk",
                                    DATADIR "/libvirt/schemas/domainsnapshot.rng");
    return GVIR_CONFIG_DOMAIN_SNAPSHOT_DISK(object);
}


GVirConfigDomainSnapshotDisk *gvir_config_domain_snapshot_disk_new_from_xml(const gchar *xml,
                                                                            GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT_DISK,
                                             "disk",
                                             DATADIR "/libvirt/schemas/domainsnapshot.rng",
                                             xml, error);
    return GVIR_CONFIG_DOMAIN_SNAPSHOT_DISK(object);
}


GVirConfigDomainSnapshotDisk *
gvir_config_domain_snapshot_disk_new_from_tree(GVirConfigXmlDoc *doc,
                                               xmlNodePtr tree)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_tree(GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT_DISK,
                                              doc, NULL, tree);

    return GVIR_CONFIG_DOMAIN_SNAPSHOT_DISK(object);
}
