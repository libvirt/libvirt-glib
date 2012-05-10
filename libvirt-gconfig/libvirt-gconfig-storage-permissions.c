/*
 * libvirt-gconfig-storage-permissions.c: libvirt storage permissions configuration
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

#define GVIR_CONFIG_STORAGE_PERMISSIONS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_STORAGE_PERMISSIONS, GVirConfigStoragePermissionsPrivate))

struct _GVirConfigStoragePermissionsPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigStoragePermissions, gvir_config_storage_permissions, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_storage_permissions_class_init(GVirConfigStoragePermissionsClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigStoragePermissionsPrivate));
}


static void gvir_config_storage_permissions_init(GVirConfigStoragePermissions *perms)
{
    g_debug("Init GVirConfigStoragePermissions=%p", perms);

    perms->priv = GVIR_CONFIG_STORAGE_PERMISSIONS_GET_PRIVATE(perms);
}


GVirConfigStoragePermissions *gvir_config_storage_permissions_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_STORAGE_PERMISSIONS,
                                    "permissions", NULL);
    return GVIR_CONFIG_STORAGE_PERMISSIONS(object);
}

GVirConfigStoragePermissions *gvir_config_storage_permissions_new_from_xml(const gchar *xml,
                                                                           GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_STORAGE_PERMISSIONS,
                                             "permissions", NULL,
                                             xml, error);
    return GVIR_CONFIG_STORAGE_PERMISSIONS(object);
}

void gvir_config_storage_permissions_set_group(GVirConfigStoragePermissions *perms,
                                               guint group)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_PERMISSIONS(perms));

    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(perms),
                                               "group", group);
}

/**
 * gvir_config_storage_permissions_set_label:
 * @label: (allow-none):
 */
void gvir_config_storage_permissions_set_label(GVirConfigStoragePermissions *perms,
                                               const char *label)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_PERMISSIONS(perms));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(perms),
                                        "label", label);
}

void gvir_config_storage_permissions_set_mode(GVirConfigStoragePermissions *perms,
                                              guint mode)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_PERMISSIONS(perms));

    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(perms),
                                               "mode", mode);
}

void gvir_config_storage_permissions_set_owner(GVirConfigStoragePermissions *perms,
                                               guint owner)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_PERMISSIONS(perms));

    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(perms),
                                               "owner", owner);
}
