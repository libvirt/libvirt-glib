/*
 * libvirt-gconfig-storage-pool.h: libvirt storage pool configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_STORAGE_POOL_H__
#define __LIBVIRT_GCONFIG_STORAGE_POOL_H__

#include <libvirt-gconfig/libvirt-gconfig-storage-pool-source.h>
#include <libvirt-gconfig/libvirt-gconfig-storage-pool-target.h>

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_STORAGE_POOL            (gvir_config_storage_pool_get_type ())
#define GVIR_CONFIG_STORAGE_POOL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_STORAGE_POOL, GVirConfigStoragePool))
#define GVIR_CONFIG_STORAGE_POOL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_STORAGE_POOL, GVirConfigStoragePoolClass))
#define GVIR_CONFIG_IS_STORAGE_POOL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_STORAGE_POOL))
#define GVIR_CONFIG_IS_STORAGE_POOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_STORAGE_POOL))
#define GVIR_CONFIG_STORAGE_POOL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_STORAGE_POOL, GVirConfigStoragePoolClass))

typedef struct _GVirConfigStoragePool GVirConfigStoragePool;
typedef struct _GVirConfigStoragePoolPrivate GVirConfigStoragePoolPrivate;
typedef struct _GVirConfigStoragePoolClass GVirConfigStoragePoolClass;

struct _GVirConfigStoragePool
{
    GVirConfigObject parent;

    GVirConfigStoragePoolPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigStoragePoolClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_STORAGE_POOL_TYPE_DIR,
    GVIR_CONFIG_STORAGE_POOL_TYPE_FS,
    GVIR_CONFIG_STORAGE_POOL_TYPE_NETFS,
    GVIR_CONFIG_STORAGE_POOL_TYPE_LOGICAL,
    GVIR_CONFIG_STORAGE_POOL_TYPE_DISK,
    GVIR_CONFIG_STORAGE_POOL_TYPE_ISCSI,
    GVIR_CONFIG_STORAGE_POOL_TYPE_SCSI,
    GVIR_CONFIG_STORAGE_POOL_TYPE_MPATH
} GVirConfigStoragePoolType;

GType gvir_config_storage_pool_get_type(void);

GVirConfigStoragePool *gvir_config_storage_pool_new(void);
GVirConfigStoragePool *gvir_config_storage_pool_new_from_xml(const gchar *xml,
                                                             GError **error);

guint64 gvir_config_storage_pool_get_allocation(GVirConfigStoragePool *pool);
void gvir_config_storage_pool_set_allocation(GVirConfigStoragePool *pool,
                                             guint64 allocation);
guint64 gvir_config_storage_pool_get_available(GVirConfigStoragePool *pool);
void gvir_config_storage_pool_set_available(GVirConfigStoragePool *pool,
                                            guint64 available);
guint64 gvir_config_storage_pool_get_capacity(GVirConfigStoragePool *pool);
void gvir_config_storage_pool_set_capacity(GVirConfigStoragePool *pool,
                                           guint64 capacity);
const char *gvir_config_storage_pool_get_name(GVirConfigStoragePool *pool);
void gvir_config_storage_pool_set_name(GVirConfigStoragePool *pool,
                                       const char *name);
GVirConfigStoragePoolType gvir_config_storage_pool_get_pool_type(GVirConfigStoragePool *pool);
void gvir_config_storage_pool_set_pool_type(GVirConfigStoragePool *pool,
                                            GVirConfigStoragePoolType type);
GVirConfigStoragePoolSource *gvir_config_storage_pool_get_source(GVirConfigStoragePool *pool);
void gvir_config_storage_pool_set_source(GVirConfigStoragePool *pool,
                                         GVirConfigStoragePoolSource *source);
GVirConfigStoragePoolTarget *gvir_config_storage_pool_get_target(GVirConfigStoragePool *pool);
void gvir_config_storage_pool_set_target(GVirConfigStoragePool *pool,
                                         GVirConfigStoragePoolTarget *target);
const char *gvir_config_storage_pool_get_uuid(GVirConfigStoragePool *pool);
void gvir_config_storage_pool_set_uuid(GVirConfigStoragePool *pool,
                                       const char *uuid);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_STORAGE_POOL_H__ */
