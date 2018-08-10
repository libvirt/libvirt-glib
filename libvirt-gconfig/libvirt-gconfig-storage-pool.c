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

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigStoragePool, gvir_config_storage_pool, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_storage_pool_class_init(GVirConfigStoragePoolClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_storage_pool_init(GVirConfigStoragePool *pool)
{
    pool->priv = GVIR_CONFIG_STORAGE_POOL_GET_PRIVATE(pool);
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

/**
 * gvir_config_storage_pool_get_pool_type:
 * @pool: a #GVirConfigStoragePool
 *
 * Gets the type of the pool.
 *
 * Returns: #Gname of @pool.
 */
GVirConfigStoragePoolType gvir_config_storage_pool_get_pool_type(GVirConfigStoragePool *pool)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool),
                         GVIR_CONFIG_STORAGE_POOL_TYPE_DIR);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(pool),
                                                  NULL, "type",
                                                  GVIR_CONFIG_TYPE_STORAGE_POOL_TYPE,
                                                  GVIR_CONFIG_STORAGE_POOL_TYPE_DIR);
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
 * gvir_config_storage_pool_get_name:
 * @pool: a #GVirConfigStoragePool
 *
 * Gets the name of the pool.
 *
 * Returns: name of @pool.
 */
const char *gvir_config_storage_pool_get_name(GVirConfigStoragePool *pool)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool), NULL);

    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(pool),
                                               "name");
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
 * gvir_config_storage_pool_get_uuid:
 * @pool: a #GVirConfigStoragePool
 *
 * Gets the unique identifier for @pool.
 *
 * Returns: unique identifier for @pool.
 */
const char *gvir_config_storage_pool_get_uuid(GVirConfigStoragePool *pool)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool), NULL);

    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(pool),
                                               "uuid");
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

/**
 * gvir_config_storage_pool_get_capacity:
 * @pool: a #GVirConfigStoragePool
 *
 * Gets the total storage capacity for the pool.
 *
 * Returns: total storage capacity in bytes.
 */
guint64 gvir_config_storage_pool_get_capacity(GVirConfigStoragePool *pool)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool), 0);

    return gvir_config_object_get_node_content_uint64(GVIR_CONFIG_OBJECT(pool),
                                                      "capacity");
}

void gvir_config_storage_pool_set_capacity(GVirConfigStoragePool *pool,
                                           guint64 capacity)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool));

    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(pool),
                                               "capacity", capacity);
}

/**
 * gvir_config_storage_pool_get_allocation:
 * @pool: a #GVirConfigStoragePool
 *
 * Gets the total storage allocation for the pool.
 *
 * Returns: total storage allocation in bytes.
 */
guint64 gvir_config_storage_pool_get_allocation(GVirConfigStoragePool *pool)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool), 0);

    return gvir_config_object_get_node_content_uint64(GVIR_CONFIG_OBJECT(pool),
                                                      "allocation");
}

void gvir_config_storage_pool_set_allocation(GVirConfigStoragePool *pool,
                                             guint64 allocation)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool));

    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(pool),
                                               "allocation", allocation);
}

/**
 * gvir_config_storage_pool_get_available:
 * @pool: a #GVirConfigStoragePool
 *
 * Gets the free space available for allocating new volumes in the pool.
 *
 * Returns: free space available in bytes.
 */
guint64 gvir_config_storage_pool_get_available(GVirConfigStoragePool *pool)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool), 0);

    return gvir_config_object_get_node_content_uint64(GVIR_CONFIG_OBJECT(pool),
                                                      "available");
}

void gvir_config_storage_pool_set_available(GVirConfigStoragePool *pool,
                                            guint64 available)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool));

    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(pool),
                                               "available", available);
}

/**
 * gvir_config_storage_pool_get_source:
 * @pool: a #GVirConfigStoragePool
 *
 * Gets the source for @pool
 *
 * Returns: (transfer full): a new #GVirConfigStoragePoolSource instance.
 */
GVirConfigStoragePoolSource *gvir_config_storage_pool_get_source(GVirConfigStoragePool *pool)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool), NULL);

    object = gvir_config_object_get_child_with_type
                                (GVIR_CONFIG_OBJECT(pool),
                                 "source",
                                 GVIR_CONFIG_TYPE_STORAGE_POOL_SOURCE);

    return GVIR_CONFIG_STORAGE_POOL_SOURCE(object);
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
 * gvir_config_storage_pool_get_target:
 * @pool: a #GVirConfigStoragePool
 *
 * Gets the target for @pool
 *
 * Returns: (transfer full): a new #GVirConfigStoragePoolTarget instance.
 */
GVirConfigStoragePoolTarget *gvir_config_storage_pool_get_target(GVirConfigStoragePool *pool)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL(pool), NULL);

    object = gvir_config_object_get_child_with_type
                                (GVIR_CONFIG_OBJECT(pool),
                                 "target",
                                 GVIR_CONFIG_TYPE_STORAGE_POOL_TARGET);

    return GVIR_CONFIG_STORAGE_POOL_TARGET(object);
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
