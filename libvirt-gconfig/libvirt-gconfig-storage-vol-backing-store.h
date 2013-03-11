/*
 * libvirt-gconfig-storage-vol-backing-store.c: libvirt storage volume backing store configuration
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

#ifndef __LIBVIRT_GCONFIG_STORAGE_VOL_BACKING_STORE_H__
#define __LIBVIRT_GCONFIG_STORAGE_VOL_BACKING_STORE_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_STORAGE_VOL_BACKING_STORE            (gvir_config_storage_vol_backing_store_get_type ())
#define GVIR_CONFIG_STORAGE_VOL_BACKING_STORE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_STORAGE_VOL_BACKING_STORE, GVirConfigStorageVolBackingStore))
#define GVIR_CONFIG_STORAGE_VOL_BACKING_STORE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_STORAGE_VOL_BACKING_STORE, GVirConfigStorageVolBackingStoreClass))
#define GVIR_CONFIG_IS_STORAGE_VOL_BACKING_STORE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_STORAGE_VOL_BACKING_STORE))
#define GVIR_CONFIG_IS_STORAGE_VOL_BACKING_STORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_STORAGE_VOL_BACKING_STORE))
#define GVIR_CONFIG_STORAGE_VOL_BACKING_STORE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_STORAGE_VOL_BACKING_STORE, GVirConfigStorageVolBackingStoreClass))

typedef struct _GVirConfigStorageVolBackingStore GVirConfigStorageVolBackingStore;
typedef struct _GVirConfigStorageVolBackingStorePrivate GVirConfigStorageVolBackingStorePrivate;
typedef struct _GVirConfigStorageVolBackingStoreClass GVirConfigStorageVolBackingStoreClass;

struct _GVirConfigStorageVolBackingStore
{
    GVirConfigObject parent;

    GVirConfigStorageVolBackingStorePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigStorageVolBackingStoreClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_storage_vol_backing_store_get_type(void);

GVirConfigStorageVolBackingStore *gvir_config_storage_vol_backing_store_new(void);
GVirConfigStorageVolBackingStore *gvir_config_storage_vol_backing_store_new_from_xml(const gchar *xml,
                                                                                     GError **error);
void gvir_config_storage_vol_backing_store_set_format(GVirConfigStorageVolBackingStore *backing_store,
                                                      const char *format);
void gvir_config_storage_vol_backing_store_set_path(GVirConfigStorageVolBackingStore *backing_store,
                                                    const char *path);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_STORAGE_VOL_BACKING_STORE_H__ */
