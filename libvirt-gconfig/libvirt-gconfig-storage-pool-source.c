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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
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

G_DEFINE_TYPE(GVirConfigStoragePoolSource, gvir_config_storage_pool_source, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_storage_pool_source_class_init(GVirConfigStoragePoolSourceClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigStoragePoolSourcePrivate));
}


static void gvir_config_storage_pool_source_init(GVirConfigStoragePoolSource *source)
{
    g_debug("Init GVirConfigStoragePoolSource=%p", source);

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
