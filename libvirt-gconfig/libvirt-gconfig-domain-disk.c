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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_DISK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_DISK, GVirConfigDomainDiskPrivate))

struct _GVirConfigDomainDiskPrivate
{
    GVirConfigDomainDiskType type;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainDisk, gvir_config_domain_disk, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_disk_class_init(GVirConfigDomainDiskClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_disk_init(GVirConfigDomainDisk *disk)
{
    disk->priv = GVIR_CONFIG_DOMAIN_DISK_GET_PRIVATE(disk);
}


GVirConfigDomainDisk *gvir_config_domain_disk_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_DISK,
                                    "disk", NULL);
    return GVIR_CONFIG_DOMAIN_DISK(object);
}

GVirConfigDomainDisk *gvir_config_domain_disk_new_from_xml(const gchar *xml,
                                                           GError **error)
{
    GVirConfigObject *object;
    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_DISK,
                                             "disk", NULL, xml, error);

    return GVIR_CONFIG_DOMAIN_DISK(object);
}

GVirConfigDomainDevice *
gvir_config_domain_disk_new_from_tree(GVirConfigXmlDoc *doc,
                                      xmlNodePtr tree)
{
    GVirConfigObject *object;
    GVirConfigDomainDisk *disk;
    GVirConfigDomainDiskType type;
    const char *type_str;

    type_str = gvir_config_xml_get_attribute_content(tree, "type");
    if (type_str == NULL)
        return NULL;

    type = gvir_config_genum_get_value(GVIR_CONFIG_TYPE_DOMAIN_DISK_TYPE,
                                       type_str,
                                       GVIR_CONFIG_DOMAIN_DISK_FILE);

    object = gvir_config_object_new_from_tree(GVIR_CONFIG_TYPE_DOMAIN_DISK,
                                              doc, NULL, tree);
    disk = GVIR_CONFIG_DOMAIN_DISK(object);
    disk->priv->type = type;

    return GVIR_CONFIG_DOMAIN_DEVICE(object);
}

void gvir_config_domain_disk_set_type(GVirConfigDomainDisk *disk,
                                      GVirConfigDomainDiskType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(disk), "type",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_TYPE,
                                               type, NULL);
    disk->priv->type = type;
}

void gvir_config_domain_disk_set_guest_device_type(GVirConfigDomainDisk *disk,
                                                   GVirConfigDomainDiskGuestDeviceType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(disk), "device",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_GUEST_DEVICE_TYPE,
                                               type, NULL);
}

void gvir_config_domain_disk_set_snapshot_type(GVirConfigDomainDisk *disk,
                                               GVirConfigDomainDiskSnapshotType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(disk), "snapshot",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_SNAPSHOT_TYPE,
                                               type, NULL);
}

void gvir_config_domain_disk_set_startup_policy(GVirConfigDomainDisk *disk,
                                                GVirConfigDomainDiskStartupPolicy policy)
{
    const char *str;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    str = gvir_config_genum_get_nick(GVIR_CONFIG_TYPE_DOMAIN_DISK_STARTUP_POLICY, policy);
    g_return_if_fail(str != NULL);
    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                "source", "startupPolicy", str);
}

void gvir_config_domain_disk_set_source(GVirConfigDomainDisk *disk,
                                        const char *source)
{
    const char *attribute_name;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));

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
    gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                   "source",
                                                   attribute_name, source);
}

void gvir_config_domain_disk_set_driver_name(GVirConfigDomainDisk *disk,
                                             const char *driver_name)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                "driver", "name", driver_name);
}

/**
 * gvir_config_domain_disk_set_driver_type:
 *
 * Deprecated: 0.1.7: Use gvir_config_domain_disk_set_driver_format()
 * instead
 */
void gvir_config_domain_disk_set_driver_type(GVirConfigDomainDisk *disk,
                                             const char *driver_type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                "driver", "type", driver_type);
}


void gvir_config_domain_disk_set_driver_format(GVirConfigDomainDisk *disk,
                                               GVirConfigDomainDiskFormat format)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    gvir_config_object_add_child_with_attribute_enum(GVIR_CONFIG_OBJECT(disk),
                                                     "driver", "type",
                                                     GVIR_CONFIG_TYPE_DOMAIN_DISK_FORMAT,
                                                     format);
}


void gvir_config_domain_disk_set_driver_cache(GVirConfigDomainDisk *disk,
                                              GVirConfigDomainDiskCacheType cache_type)
{
    const char *cache_str;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    cache_str = gvir_config_genum_get_nick(GVIR_CONFIG_TYPE_DOMAIN_DISK_CACHE_TYPE, cache_type);
    g_return_if_fail(cache_str != NULL);
    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                "driver", "cache", cache_str);
}

void gvir_config_domain_disk_set_target_bus(GVirConfigDomainDisk *disk,
                                            GVirConfigDomainDiskBus bus)
{
    const char *bus_str;
    GVirConfigObject *child;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    bus_str = gvir_config_genum_get_nick(GVIR_CONFIG_TYPE_DOMAIN_DISK_BUS, bus);
    g_return_if_fail(bus_str != NULL);

    child = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(disk), "target");
    gvir_config_object_set_attribute(child, "bus", bus_str, NULL);

    if (bus == GVIR_CONFIG_DOMAIN_DISK_BUS_USB) {
        gvir_config_object_set_attribute(child, "removable", "on", NULL);
    }
    g_object_unref(G_OBJECT(child));
}

void gvir_config_domain_disk_set_target_dev(GVirConfigDomainDisk *disk,
                                            const char *dev)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(disk),
                                                "target", "dev", dev);
}

GVirConfigDomainDiskType
gvir_config_domain_disk_get_disk_type(GVirConfigDomainDisk *disk)
{
    return disk->priv->type;
}

GVirConfigDomainDiskGuestDeviceType
gvir_config_domain_disk_get_guest_device_type(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk),
                         GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_DISK);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(disk),
                                                  NULL,
                                                  "device",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_GUEST_DEVICE_TYPE,
                                                  GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_DISK);
}

GVirConfigDomainDiskSnapshotType
gvir_config_domain_disk_get_snapshot_type(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk),
                         GVIR_CONFIG_DOMAIN_DISK_SNAPSHOT_NO);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(disk),
                                                  NULL,
                                                  "snapshot",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_SNAPSHOT_TYPE,
                                                  GVIR_CONFIG_DOMAIN_DISK_SNAPSHOT_NO);
}

GVirConfigDomainDiskStartupPolicy
gvir_config_domain_disk_get_startup_policy(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk),
                         GVIR_CONFIG_DOMAIN_DISK_STARTUP_POLICY_MANDATORY);

    return gvir_config_object_get_attribute_genum
                (GVIR_CONFIG_OBJECT(disk),
                 "source", "startupPolicy",
                 GVIR_CONFIG_TYPE_DOMAIN_DISK_STARTUP_POLICY,
                 GVIR_CONFIG_DOMAIN_DISK_STARTUP_POLICY_MANDATORY);
}

const char *
gvir_config_domain_disk_get_source(GVirConfigDomainDisk *disk)
{
    const char *attribute_name;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk), NULL);

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
            g_return_val_if_reached(NULL);
    }
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(disk),
                                            "source", attribute_name);
}

const char *
gvir_config_domain_disk_get_driver_name(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(disk),
                                            "driver", "name");
}

/**
 * gvir_config_domain_disk_get_driver_type:
 *
 * Deprecated: 0.1.7: Use gvir_config_domain_disk_get_driver_format()
 * instead
 */
const char *
gvir_config_domain_disk_get_driver_type(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(disk),
                                            "driver", "type");
}


GVirConfigDomainDiskFormat
gvir_config_domain_disk_get_driver_format(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk),
                         GVIR_CONFIG_DOMAIN_DISK_FORMAT_RAW);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(disk),
                                                  "driver", "type",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_FORMAT,
                                                  GVIR_CONFIG_DOMAIN_DISK_FORMAT_RAW);
}


GVirConfigDomainDiskCacheType
gvir_config_domain_disk_get_driver_cache(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk),
                         GVIR_CONFIG_DOMAIN_DISK_CACHE_DEFAULT);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(disk),
                                                  "driver", "cache",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_CACHE_TYPE,
                                                  GVIR_CONFIG_DOMAIN_DISK_CACHE_DEFAULT);
}


GVirConfigDomainDiskBus
gvir_config_domain_disk_get_target_bus(GVirConfigDomainDisk *disk)
{
    /* FIXME: the default value depends on the "name" attribute, should we
     * copy what libvirt is doing here?
     */
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk),
                         GVIR_CONFIG_DOMAIN_DISK_BUS_IDE);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(disk),
                                                  "target", "snapshot",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_BUS,
                                                  GVIR_CONFIG_DOMAIN_DISK_BUS_IDE);
}


const char *
gvir_config_domain_disk_get_target_dev(GVirConfigDomainDisk *disk)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(disk),
                                            "target", "dev");
}


void
gvir_config_domain_disk_set_readonly(GVirConfigDomainDisk *disk,
                                     gboolean readonly)
{
    if (readonly) {
        GVirConfigObject *node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(disk), "readonly");
        g_object_unref(node);
    } else
        gvir_config_object_delete_child(GVIR_CONFIG_OBJECT(disk), "readonly", NULL);
}


/**
 * gvir_config_domain_disk_set_driver:
 * @disk: a #GVirConfigDomainDisk
 * @driver: (allow-none): a #GVirConfigDomainDiskDriver
 *
 * Uses @driver as the driver configuration for @disk.
 */
void gvir_config_domain_disk_set_driver(GVirConfigDomainDisk *disk,
                                        GVirConfigDomainDiskDriver *driver)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk));
    g_return_if_fail(driver == NULL || GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(disk),
                                      "driver",
                                      GVIR_CONFIG_OBJECT(driver));
}


/**
 * gvir_config_domain_disk_get_driver:
 * @disk: a #GVirConfigDomainDisk
 *
 * Gets the driver configuration for @disk.
 *
 * Returns: (transfer full): A #GVirConfigDomainDiskDriver. The returned
 * object should be unreffed with g_object_unref() when no longer needed.
 */
GVirConfigDomainDiskDriver *gvir_config_domain_disk_get_driver(GVirConfigDomainDisk *disk)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK(disk), NULL);

    object = gvir_config_object_get_child_with_type(GVIR_CONFIG_OBJECT(disk),
                                                    "driver",
                                                    GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER);

    return GVIR_CONFIG_DOMAIN_DISK_DRIVER(object);
}
