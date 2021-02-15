/*
 * libvirt-gconfig-storage-vol-target.c: libvirt storage volume target configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_STORAGE_VOL_TARGET            (gvir_config_storage_vol_target_get_type ())
#define GVIR_CONFIG_STORAGE_VOL_TARGET(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_STORAGE_VOL_TARGET, GVirConfigStorageVolTarget))
#define GVIR_CONFIG_STORAGE_VOL_TARGET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_STORAGE_VOL_TARGET, GVirConfigStorageVolTargetClass))
#define GVIR_CONFIG_IS_STORAGE_VOL_TARGET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_STORAGE_VOL_TARGET))
#define GVIR_CONFIG_IS_STORAGE_VOL_TARGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_STORAGE_VOL_TARGET))
#define GVIR_CONFIG_STORAGE_VOL_TARGET_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_STORAGE_VOL_TARGET, GVirConfigStorageVolTargetClass))

typedef struct _GVirConfigStorageVolTarget GVirConfigStorageVolTarget;
typedef struct _GVirConfigStorageVolTargetPrivate GVirConfigStorageVolTargetPrivate;
typedef struct _GVirConfigStorageVolTargetClass GVirConfigStorageVolTargetClass;

struct _GVirConfigStorageVolTarget
{
    GVirConfigObject parent;

    GVirConfigStorageVolTargetPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigStorageVolTargetClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_storage_vol_target_get_type(void);

GVirConfigStorageVolTarget *gvir_config_storage_vol_target_new(void);
GVirConfigStorageVolTarget *gvir_config_storage_vol_target_new_from_xml(const gchar *xml,
                                                                        GError **error);

void gvir_config_storage_vol_target_set_compat(GVirConfigStorageVolTarget *target,
                                               const char *compat);
void gvir_config_storage_vol_target_set_format(GVirConfigStorageVolTarget *target,
                                               const char *format);
void gvir_config_storage_vol_target_set_permissions(GVirConfigStorageVolTarget *target,
                                                    GVirConfigStoragePermissions *perms);
typedef enum {
    GVIR_CONFIG_STORAGE_VOL_TARGET_FEATURE_LAZY_REFCOUNT = 1 << 0
} GVirConfigStorageVolTargetFeatures;

void gvir_config_storage_vol_target_set_features(GVirConfigStorageVolTarget *target,
                                                 guint64 features);

G_END_DECLS
