/*
 * libvirt-gconfig-storage-pool-source.c: libvirt storage pool source configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"


#define GVIR_CONFIG_STORAGE_POOL_SOURCE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_STORAGE_POOL_SOURCE, GVirConfigStoragePoolSourcePrivate))

struct _GVirConfigStoragePoolSourcePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigStoragePoolSource, gvir_config_storage_pool_source, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_storage_pool_source_class_init(GVirConfigStoragePoolSourceClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_storage_pool_source_init(GVirConfigStoragePoolSource *source)
{
    source->priv = GVIR_CONFIG_STORAGE_POOL_SOURCE_GET_PRIVATE(source);
}


GVirConfigStoragePoolSource *gvir_config_storage_pool_source_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_STORAGE_POOL_SOURCE,
                                    "source", NULL);
    return GVIR_CONFIG_STORAGE_POOL_SOURCE(object);
}

GVirConfigStoragePoolSource *gvir_config_storage_pool_source_new_from_xml(const gchar *xml,
                                                           GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_STORAGE_POOL_SOURCE,
                                             "source", NULL,
                                             xml, error);
    return GVIR_CONFIG_STORAGE_POOL_SOURCE(object);
}

/**
 * gvir_config_storage_pool_source_get_adapter:
 * @source: a #GVirConfigStoragePoolSource
 *
 * For pools backed by a SCSI adapter, returns the SCSI adapter name
 *
 * Returns: the SCSI adapter name.
 */
const char *gvir_config_storage_pool_source_get_adapter(GVirConfigStoragePoolSource *source)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source), NULL);
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(source),
                                            "adapter",
                                            "name");
}

void gvir_config_storage_pool_source_set_adapter(GVirConfigStoragePoolSource *source,
                                                 const char *adapter)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(source), "adapter");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute(node, "name", adapter, NULL);
    g_object_unref(G_OBJECT(node));
}

/**
 * gvir_config_storage_pool_source_get_device_path:
 * @source: a #GVirConfigStoragePoolSource
 *
 * For pools backed by a physical device, returns the path to the block
 * device node
 *
 * Returns: fully qualified path to the block device node.
 */
const char *gvir_config_storage_pool_source_get_device_path(GVirConfigStoragePoolSource *source)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source), NULL);
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(source),
                                            "device",
                                            "path");
}

void gvir_config_storage_pool_source_set_device_path(GVirConfigStoragePoolSource *source,
                                                     const char *device_path)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source));

    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(source), "device");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute(node, "path", device_path, NULL);
    g_object_unref(G_OBJECT(node));
}

/**
 * gvir_config_storage_pool_source_get_directory:
 * @source: a #GVirConfigStoragePoolSource
 *
 * For pools backed by a directory, returns the path to the backing directory
 *
 * Returns: path to the directory backing directory.
 */
const char *gvir_config_storage_pool_source_get_directory(GVirConfigStoragePoolSource *source)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source), NULL);
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(source),
                                            "directory",
                                            "path");
}

void gvir_config_storage_pool_source_set_directory(GVirConfigStoragePoolSource *source,
                                                   const char *directory)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(source), "directory");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute(node, "path", directory, NULL);
    g_object_unref(G_OBJECT(node));
}

/**
 * gvir_config_storage_pool_source_get_format:
 * @source: a #GVirConfigStoragePoolSource
 *
 * Provides information about the format of the pool. This format is
 * backend-specific but is typically used to indicate filesystem type, or
 * network filesystem type, or partition table type, or LVM metadata type.
 *
 * Returns: the storage pool format.
 */
const char *gvir_config_storage_pool_source_get_format(GVirConfigStoragePoolSource *source)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source), NULL);
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(source),
                                            "format",
                                            "type");
}

void gvir_config_storage_pool_source_set_format(GVirConfigStoragePoolSource *source,
                                                const char *format)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(source), "format");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute(node, "type", format, NULL);
    g_object_unref(G_OBJECT(node));
}

/**
 * gvir_config_storage_pool_source_get_host:
 * @source: a #GVirConfigStoragePoolSource
 *
 * For pools backed by storage from remote server, returns the hostname
 * of the remote server.
 *
 * Returns: hostname or IP address of the remote server.
 */
const char *gvir_config_storage_pool_source_get_host(GVirConfigStoragePoolSource *source)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source), NULL);
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(source),
                                            "host",
                                            "name");
}

void gvir_config_storage_pool_source_set_host(GVirConfigStoragePoolSource *source,
                                              const char *host)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(source), "host");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute(node, "name", host, NULL);
    g_object_unref(G_OBJECT(node));
}

/**
 * gvir_config_storage_pool_source_get_name:
 * @source: a #GVirConfigStoragePoolSource
 *
 * For pools backed by storage from a named element (for example, LV
 * groups), returns the name of the element
 *
 * Returns: name of the element used by @source
 */
const char *gvir_config_storage_pool_source_get_name(GVirConfigStoragePoolSource *source)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source), NULL);
    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(source),
                                               "name");
}

/**
 * gvir_config_storage_pool_source_set_name:
 * @name: (allow-none):
 */
void gvir_config_storage_pool_source_set_name(GVirConfigStoragePoolSource *source,
                                              const char *name)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(source),
                                        "name", name);
}

/**
 * gvir_config_storage_pool_source_get_product:
 * @source: a #GVirConfigStoragePoolSource
 *
 * Gets the product name of the storage device.
 *
 * Returns: product name of the storage device.
 */
const char *gvir_config_storage_pool_source_get_product(GVirConfigStoragePoolSource *source)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source), NULL);
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(source),
                                            "product",
                                            "name");
}

void gvir_config_storage_pool_source_set_product(GVirConfigStoragePoolSource *source,
                                                 const char *product)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(source), "product");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute(node, "name", product, NULL);
    g_object_unref(G_OBJECT(node));
}

/**
 * gvir_config_storage_pool_source_get_vendor:
 * @source: a #GVirConfigStoragePoolSource
 *
 * Gets the vendor name of the storage device.
 *
 * Returns: vendor name of the storage device.
 */
const char *gvir_config_storage_pool_source_get_vendor(GVirConfigStoragePoolSource *source)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source), NULL);
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(source),
                                            "vendor",
                                            "name");
}

void gvir_config_storage_pool_source_set_vendor(GVirConfigStoragePoolSource *source,
                                                const char *vendor)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(source), "vendor");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute(node, "name", vendor, NULL);
    g_object_unref(G_OBJECT(node));
}
