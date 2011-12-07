/*
 * libvirt-gconfig-domain-disk.c: libvirt domain disk configuration
 *
 * Copyright (C) 2011 Red Hat, Inc.
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"

#define GVIR_CONFIG_DOMAIN_DISK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_DISK, GVirConfigDomainDiskPrivate))

struct _GVirConfigDomainDiskPrivate
{
    GVirConfigDomainDiskType type;
};

G_DEFINE_TYPE(GVirConfigDomainDisk, gvir_config_domain_disk, GVIR_TYPE_CONFIG_DOMAIN_DEVICE);


static void gvir_config_domain_disk_class_init(GVirConfigDomainDiskClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainDiskPrivate));
}


static void gvir_config_domain_disk_init(GVirConfigDomainDisk *disk)
{
    g_debug("Init GVirConfigDomainDisk=%p", disk);

    disk->priv = GVIR_CONFIG_DOMAIN_DISK_GET_PRIVATE(disk);
}


GVirConfigDomainDisk *gvir_config_domain_disk_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_DISK,
                                    "disk", NULL);
    return GVIR_CONFIG_DOMAIN_DISK(object);
}

GVirConfigDomainDisk *gvir_config_domain_disk_new_from_xml(const gchar *xml,
                                                           GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_DISK,
                                             "disk", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_DISK(object);
}

void gvir_config_domain_disk_set_type(GVirConfigDomainDisk *disk,
                                      GVirConfigDomainDiskType type)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_DISK(disk));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(disk), "type",
                                               GVIR_TYPE_CONFIG_DOMAIN_DISK_TYPE,
                                               type, NULL);
    disk->priv->type = type;
}

void gvir_config_domain_disk_set_guest_device_type(GVirConfigDomainDisk *disk,
                                                   GVirConfigDomainDiskGuestDeviceType type)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_DISK(disk));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(disk), "device",
                                               GVIR_TYPE_CONFIG_DOMAIN_DISK_GUEST_DEVICE_TYPE,
                                               type, NULL);
}

void gvir_config_domain_disk_set_snapshot_type(GVirConfigDomainDisk *disk,
                                               GVirConfigDomainDiskSnapshotType type)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_DISK(disk));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(disk), "snapshot",
                                               GVIR_TYPE_CONFIG_DOMAIN_DISK_SNAPSHOT_TYPE,
                                               type, NULL);
}

void gvir_config_domain_disk_set_source(GVirConfigDomainDisk *disk,
                                        const char *source)
{
    GVirConfigObject *source_node;
    const char *attribute_name;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_DISK(disk));
    source_node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(disk),
                                                   "source");
    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(source_node));

    switch (disk->priv->type) {
        case GVIR_CONFIG_DOMAIN_DISK_FILE:
            attribute_name = "file";
            break;
        case GVIR_CONFIG_DOMAIN_DISK_BLOCK:
            attribute_name = "dev";
            break;
        case GVIR_CONFIG_DOMAIN_DISK_DIR:
            attribute_name = "dir";
            break;
        case GVIR_CONFIG_DOMAIN_DISK_NETWORK:
            attribute_name = "protocol";
            break;
        default:
            g_return_if_reached();
    }
    gvir_config_object_set_attribute(source_node,
                                     attribute_name, source,
                                     NULL);
    g_object_unref(G_OBJECT(source_node));
}

void gvir_config_domain_disk_set_driver_name(GVirConfigDomainDisk *disk,
                                             const char *driver_name)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_DISK(disk));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(disk), "driver");
    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(node));
    gvir_config_object_set_attribute(node, "name", driver_name, NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_disk_set_driver_type(GVirConfigDomainDisk *disk,
                                             const char *driver_type)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_DISK(disk));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(disk), "driver");
    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(node));
    gvir_config_object_set_attribute(node, "type", driver_type, NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_disk_set_target_bus(GVirConfigDomainDisk *disk,
                                            GVirConfigDomainDiskBus bus)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_DISK(disk));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(disk), "target");
    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(node));
    gvir_config_object_set_attribute_with_type(node,
                                               "bus",
                                               GVIR_TYPE_CONFIG_DOMAIN_DISK_BUS,
                                               bus,
                                               NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_disk_set_target_dev(GVirConfigDomainDisk *disk,
                                            const char *dev)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_DISK(disk));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(disk), "target");
    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(node));
    gvir_config_object_set_attribute(node, "dev", dev, NULL);
    g_object_unref(G_OBJECT(node));
}
