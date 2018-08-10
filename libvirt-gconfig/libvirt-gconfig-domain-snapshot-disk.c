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

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainSnapshotDisk, gvir_config_domain_snapshot_disk, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_snapshot_disk_class_init(GVirConfigDomainSnapshotDiskClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_snapshot_disk_init(GVirConfigDomainSnapshotDisk *disk)
{
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


const char *gvir_config_domain_snapshot_disk_get_name(GVirConfigDomainSnapshotDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT_DISK(disk), NULL);

    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(disk),
                                               "name");
}


void gvir_config_domain_snapshot_disk_set_name(GVirConfigDomainSnapshotDisk *disk,
                                               const char *name)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT_DISK(disk));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(disk),
                                        "name", name);
}


GVirConfigDomainDiskSnapshotType gvir_config_domain_snapshot_disk_get_snapshot_type(GVirConfigDomainSnapshotDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT_DISK(disk),
                         GVIR_CONFIG_DOMAIN_DISK_SNAPSHOT_NO);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(disk),
                                                  NULL,
                                                  "snapshot",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_SNAPSHOT_TYPE,
                                                  GVIR_CONFIG_DOMAIN_DISK_SNAPSHOT_NO);
}


void gvir_config_domain_snapshot_disk_set_snapshot_type(GVirConfigDomainSnapshotDisk *disk,
                                                        GVirConfigDomainDiskSnapshotType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT_DISK(disk));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(disk), "snapshot",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_SNAPSHOT_TYPE,
                                               type, NULL);
}


const char *gvir_config_domain_snapshot_disk_get_source_file(GVirConfigDomainSnapshotDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT_DISK(disk), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(disk),
                                            "source", "file");
}


void gvir_config_domain_snapshot_disk_set_source_file(GVirConfigDomainSnapshotDisk *disk,
                                                      const char *filename)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT_DISK(disk));

    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                "source", "file", filename);
}

GVirConfigDomainDiskFormat gvir_config_domain_snapshot_disk_get_driver_format(GVirConfigDomainSnapshotDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT_DISK(disk),
                         GVIR_CONFIG_DOMAIN_DISK_FORMAT_RAW);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(disk),
                                                  "driver", "type",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_FORMAT,
                                                  GVIR_CONFIG_DOMAIN_DISK_FORMAT_RAW);
}


void gvir_config_domain_snapshot_disk_set_driver_format(GVirConfigDomainSnapshotDisk *disk,
                                                        GVirConfigDomainDiskFormat format)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT_DISK(disk));

    gvir_config_object_add_child_with_attribute_enum(GVIR_CONFIG_OBJECT(disk),
                                                     "driver", "type",
                                                     GVIR_CONFIG_TYPE_DOMAIN_DISK_FORMAT,
                                                     format);
}
