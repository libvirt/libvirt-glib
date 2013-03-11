/*
 * libvirt-gconfig-storage-pool.c: libvirt storage pool configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
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
 * Authors: Daniel P. Berrange <berrange@redhat.com>
 *          Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_STORAGE_POOL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_STORAGE_POOL, GVirConfigStoragePoolPrivate))

struct _GVirConfigStoragePoolPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigStoragePool, gvir_config_storage_pool, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_storage_pool_class_init(GVirConfigStoragePoolClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigStoragePoolPrivate));
}


static void gvir_config_storage_pool_init(GVirConfigStoragePool *conn)
{
    g_debug("Init GVirConfigStoragePool=%p", conn);

    conn->priv = GVIR_CONFIG_STORAGE_POOL_GET_PRIVATE(conn);
}


GVirConfigStoragePool *gvir_config_storage_pool_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_STORAGE_POOL,
                                    "pool",
                                    DATADIR "/libvirt/schemas/storagepool.rng");
    return GVIR_CONFIG_STORAGE_POOL(object);
}

GVirConfigStoragePool *gvir_config_storage_pool_new_from_xml(const gchar *xml,
                                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_STORAGE_POOL,
                                             "pool",
                                             DATADIR "/libvirt/schemas/storagepool.rng",
                                             xml, error);
    return GVIR_CONFIG_STORAGE_POOL(object);
}

void gvir_config_storage_pool_set_pool_type(GVirConfigStoragePool *pool,
                                            GVirConfigStoragePoolType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(pool),
                                               "type",
                                               GVIR_CONFIG_TYPE_STORAGE_POOL_TYPE,
                                               type,
                                               NULL);
}

/**
 * gvir_config_storage_pool_set_name:
 * @name: (allow-none):
 */
void gvir_config_storage_pool_set_name(GVirConfigStoragePool *pool,
                                       const char *name)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(pool),
                                        "name", name);
}

/**
 * gvir_config_storage_pool_set_uuid:
 * @uuid: (allow-none):
 */
void gvir_config_storage_pool_set_uuid(GVirConfigStoragePool *pool,
                                       const char *uuid)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(pool),
                                        "uuid", uuid);
}

void gvir_config_storage_pool_set_capacity(GVirConfigStoragePool *pool,
                                           guint64 capacity)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool));

    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(pool),
                                               "capacity", capacity);
}

void gvir_config_storage_pool_set_allocation(GVirConfigStoragePool *pool,
                                             guint64 allocation)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool));

    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(pool),
                                               "allocation", allocation);
}

void gvir_config_storage_pool_set_available(GVirConfigStoragePool *pool,
                                            guint64 available)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool));

    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(pool),
                                               "available", available);
}

/**
 * gvir_config_storage_pool_set_source:
 * @source: (allow-none):
 */
void gvir_config_storage_pool_set_source(GVirConfigStoragePool *pool,
                                         GVirConfigStoragePoolSource *source)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool));
    g_return_if_fail(source == NULL ||
                     GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(source));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(pool),
                                      "source",
                                      GVIR_CONFIG_OBJECT(source));
}

/**
 * gvir_config_storage_pool_set_target:
 * @target: (allow-none):
 */
void gvir_config_storage_pool_set_target(GVirConfigStoragePool *pool,
                                        GVirConfigStoragePoolTarget *target)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool));
    g_return_if_fail(target == NULL ||
                     GVIR_CONFIG_IS_STORAGE_POOL_TARGET(target));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(pool),
                                      "target",
                                      GVIR_CONFIG_OBJECT(target));
}
