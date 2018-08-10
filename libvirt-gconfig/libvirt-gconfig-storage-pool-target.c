/*
 * libvirt-gconfig-storage-pool-target.c: libvirt storage pool target configuration
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


#define GVIR_CONFIG_STORAGE_POOL_TARGET_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_STORAGE_POOL_TARGET, GVirConfigStoragePoolTargetPrivate))

struct _GVirConfigStoragePoolTargetPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigStoragePoolTarget, gvir_config_storage_pool_target, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_storage_pool_target_class_init(GVirConfigStoragePoolTargetClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_storage_pool_target_init(GVirConfigStoragePoolTarget *target)
{
    target->priv = GVIR_CONFIG_STORAGE_POOL_TARGET_GET_PRIVATE(target);
}


GVirConfigStoragePoolTarget *gvir_config_storage_pool_target_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_STORAGE_POOL_TARGET,
                                    "target", NULL);
    return GVIR_CONFIG_STORAGE_POOL_TARGET(object);
}

GVirConfigStoragePoolTarget *gvir_config_storage_pool_target_new_from_xml(const gchar *xml,
                                                                          GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_STORAGE_POOL_TARGET,
                                             "target", NULL,
                                             xml, error);
    return GVIR_CONFIG_STORAGE_POOL_TARGET(object);
}

/**
 * gvir_config_storage_pool_target_get_path:
 * @target: a #GVirConfigStoragePoolTarget
 *
 * Provides the location at which the storage pool associated with @target
 * will be mapped into the local filesystem namespace.
 *
 * Returns: local filesystem path the storage pool is mapped at.
 */
const char *gvir_config_storage_pool_target_get_path(GVirConfigStoragePoolTarget *target)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_TARGET(target), NULL);

    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(target),
                                               "path");
}

/**
 * gvir_config_storage_pool_target_set_path:
 * @path: (allow-none):
 */
void gvir_config_storage_pool_target_set_path(GVirConfigStoragePoolTarget *target,
                                              const char *path)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_TARGET(target));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(target),
                                        "path", path);
}

/**
 * gvir_config_storage_pool_target_get_permissions:
 * @target: a #GVirConfigStoragePoolTarget
 *
 * Gets the permissions associated with @target
 *
 * Returns: (transfer full): a new #GVirConfigStoragePoolPermissions instance.
 */
GVirConfigStoragePermissions *gvir_config_storage_pool_target_get_permissions(GVirConfigStoragePoolTarget *target)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_TARGET(target), NULL);

    object = gvir_config_object_get_child_with_type
                                (GVIR_CONFIG_OBJECT(target),
                                 "permissions",
                                 GVIR_CONFIG_TYPE_STORAGE_PERMISSIONS);

    return GVIR_CONFIG_STORAGE_PERMISSIONS(object);
}

/**
 * gvir_config_storage_pool_perms_set_permissions:
 * @perms: (allow-none):
 */
void gvir_config_storage_pool_target_set_permissions(GVirConfigStoragePoolTarget *target,
                                                     GVirConfigStoragePermissions *perms)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_POOL_TARGET(target));
    g_return_if_fail(perms == NULL ||
                     GVIR_CONFIG_IS_STORAGE_PERMISSIONS(perms));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(target),
                                      "permissions",
                                      GVIR_CONFIG_OBJECT(perms));
}
