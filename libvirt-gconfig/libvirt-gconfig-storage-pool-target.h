/*
 * libvirt-gconfig-storage-pool-target.h: libvirt storage pool target configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_STORAGE_POOL_TARGET            (gvir_config_storage_pool_target_get_type ())
#define GVIR_CONFIG_STORAGE_POOL_TARGET(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_STORAGE_POOL_TARGET, GVirConfigStoragePoolTarget))
#define GVIR_CONFIG_STORAGE_POOL_TARGET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_STORAGE_POOL_TARGET, GVirConfigStoragePoolTargetClass))
#define GVIR_CONFIG_IS_STORAGE_POOL_TARGET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_STORAGE_POOL_TARGET))
#define GVIR_CONFIG_IS_STORAGE_POOL_TARGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_STORAGE_POOL_TARGET))
#define GVIR_CONFIG_STORAGE_POOL_TARGET_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_STORAGE_POOL_TARGET, GVirConfigStoragePoolTargetClass))

typedef struct _GVirConfigStoragePoolTarget GVirConfigStoragePoolTarget;
typedef struct _GVirConfigStoragePoolTargetPrivate GVirConfigStoragePoolTargetPrivate;
typedef struct _GVirConfigStoragePoolTargetClass GVirConfigStoragePoolTargetClass;

struct _GVirConfigStoragePoolTarget
{
    GVirConfigObject parent;

    GVirConfigStoragePoolTargetPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigStoragePoolTargetClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_storage_pool_target_get_type(void);

GVirConfigStoragePoolTarget *gvir_config_storage_pool_target_new(void);
GVirConfigStoragePoolTarget *gvir_config_storage_pool_target_new_from_xml(const gchar *xml,
                                                                          GError **error);

const char *gvir_config_storage_pool_target_get_path(GVirConfigStoragePoolTarget *target);
void gvir_config_storage_pool_target_set_path(GVirConfigStoragePoolTarget *target,
                                              const char *path);
GVirConfigStoragePermissions *gvir_config_storage_pool_target_get_permissions(GVirConfigStoragePoolTarget *target);
void gvir_config_storage_pool_target_set_permissions(GVirConfigStoragePoolTarget *target,
                                                     GVirConfigStoragePermissions *perms);

G_END_DECLS
