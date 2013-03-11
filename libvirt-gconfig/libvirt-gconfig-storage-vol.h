/*
 * libvirt-gconfig-storage-vol.c: libvirt storage volume configuration
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

#ifndef __LIBVIRT_GCONFIG_STORAGE_VOL_H__
#define __LIBVIRT_GCONFIG_STORAGE_VOL_H__

#include <libvirt-gconfig/libvirt-gconfig-storage-vol-backing-store.h>
#include <libvirt-gconfig/libvirt-gconfig-storage-vol-target.h>

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_STORAGE_VOL            (gvir_config_storage_vol_get_type ())
#define GVIR_CONFIG_STORAGE_VOL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_STORAGE_VOL, GVirConfigStorageVol))
#define GVIR_CONFIG_STORAGE_VOL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_STORAGE_VOL, GVirConfigStorageVolClass))
#define GVIR_CONFIG_IS_STORAGE_VOL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_STORAGE_VOL))
#define GVIR_CONFIG_IS_STORAGE_VOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_STORAGE_VOL))
#define GVIR_CONFIG_STORAGE_VOL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_STORAGE_VOL, GVirConfigStorageVolClass))

typedef struct _GVirConfigStorageVol GVirConfigStorageVol;
typedef struct _GVirConfigStorageVolPrivate GVirConfigStorageVolPrivate;
typedef struct _GVirConfigStorageVolClass GVirConfigStorageVolClass;

struct _GVirConfigStorageVol
{
    GVirConfigObject parent;

    GVirConfigStorageVolPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigStorageVolClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_storage_vol_get_type(void);

GVirConfigStorageVol *gvir_config_storage_vol_new(void);
GVirConfigStorageVol *gvir_config_storage_vol_new_from_xml(const gchar *xml,
                                                           GError **error);

void gvir_config_storage_vol_set_allocation(GVirConfigStorageVol *vol,
                                            guint64 allocation);
void gvir_config_storage_vol_set_backing_store(GVirConfigStorageVol *vol,
                                               GVirConfigStorageVolBackingStore *backing_store);
void gvir_config_storage_vol_set_capacity(GVirConfigStorageVol *vol,
                                          guint64 capacity);
void gvir_config_storage_vol_set_name(GVirConfigStorageVol *vol,
                                      const char *name);
void gvir_config_storage_vol_set_target(GVirConfigStorageVol *vol,
                                        GVirConfigStorageVolTarget *target);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_STORAGE_VOL_H__ */
