/*
 * libvirt-gconfig-storage-permissions.h: libvirt storage permissions configuration
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
 * Author: Christophe Fergeau
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_STORAGE_PERMISSIONS            (gvir_config_storage_permissions_get_type ())
#define GVIR_CONFIG_STORAGE_PERMISSIONS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_STORAGE_PERMISSIONS, GVirConfigStoragePermissions))
#define GVIR_CONFIG_STORAGE_PERMISSIONS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_STORAGE_PERMISSIONS, GVirConfigStoragePermissionsClass))
#define GVIR_CONFIG_IS_STORAGE_PERMISSIONS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_STORAGE_PERMISSIONS))
#define GVIR_CONFIG_IS_STORAGE_PERMISSIONS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_STORAGE_PERMISSIONS))
#define GVIR_CONFIG_STORAGE_PERMISSIONS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_STORAGE_PERMISSIONS, GVirConfigStoragePermissionsClass))

typedef struct _GVirConfigStoragePermissions GVirConfigStoragePermissions;
typedef struct _GVirConfigStoragePermissionsPrivate GVirConfigStoragePermissionsPrivate;
typedef struct _GVirConfigStoragePermissionsClass GVirConfigStoragePermissionsClass;

struct _GVirConfigStoragePermissions
{
    GVirConfigObject parent;

    GVirConfigStoragePermissionsPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigStoragePermissionsClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_storage_permissions_get_type(void);

GVirConfigStoragePermissions *gvir_config_storage_permissions_new(void);
GVirConfigStoragePermissions *gvir_config_storage_permissions_new_from_xml(const gchar *xml,
                                                                           GError **error);

guint gvir_config_storage_permissions_get_group(GVirConfigStoragePermissions *perms);
void gvir_config_storage_permissions_set_group(GVirConfigStoragePermissions *perms,
                                               guint group);
const char *gvir_config_storage_permissions_get_label(GVirConfigStoragePermissions *perms);
void gvir_config_storage_permissions_set_label(GVirConfigStoragePermissions *perms,
                                               const char *label);
guint gvir_config_storage_permissions_get_mode(GVirConfigStoragePermissions *perms);
void gvir_config_storage_permissions_set_mode(GVirConfigStoragePermissions *perms,
                                              guint mode);
guint gvir_config_storage_permissions_get_owner(GVirConfigStoragePermissions *perms);
void gvir_config_storage_permissions_set_owner(GVirConfigStoragePermissions *perms,
                                               guint owner);

G_END_DECLS
