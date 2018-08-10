/*
 * libvirt-gconfig-storage-vol.c: libvirt storage vol configuration
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"


#define GVIR_CONFIG_STORAGE_VOL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_STORAGE_VOL, GVirConfigStorageVolPrivate))

struct _GVirConfigStorageVolPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigStorageVol, gvir_config_storage_vol, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_storage_vol_class_init(GVirConfigStorageVolClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_storage_vol_init(GVirConfigStorageVol *vol)
{
    vol->priv = GVIR_CONFIG_STORAGE_VOL_GET_PRIVATE(vol);
}


GVirConfigStorageVol *gvir_config_storage_vol_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_STORAGE_VOL,
                                    "volume",
                                    DATADIR "/libvirt/schemas/storage_vol.rng");
    return GVIR_CONFIG_STORAGE_VOL(object);
}

GVirConfigStorageVol *gvir_config_storage_vol_new_from_xml(const gchar *xml,
                                                           GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_STORAGE_VOL,
                                             "volume",
                                             DATADIR "/libvirt/schemas/storage_vol.rng",
                                             xml, error);
    return GVIR_CONFIG_STORAGE_VOL(object);
}

/**
 * gvir_config_storage_vol_set_name:
 * @name: (allow-none):
 */
void gvir_config_storage_vol_set_name(GVirConfigStorageVol *vol,
                                      const char *name)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_VOL(vol));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(vol),
                                        "name", name);
}

void gvir_config_storage_vol_set_capacity(GVirConfigStorageVol *vol,
                                          guint64 capacity)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_VOL(vol));

    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(vol),
                                               "capacity", capacity);
}

void gvir_config_storage_vol_set_allocation(GVirConfigStorageVol *vol,
                                            guint64 allocation)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_VOL(vol));

    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(vol),
                                               "allocation", allocation);
}

/**
 * gvir_config_storage_vol_set_target:
 * @target: (allow-none):
 */
void gvir_config_storage_vol_set_target(GVirConfigStorageVol *vol,
                                        GVirConfigStorageVolTarget *target)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_VOL(vol));
    g_return_if_fail(target == NULL ||
                     GVIR_CONFIG_IS_STORAGE_VOL_TARGET(target));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(vol),
                                      "target",
                                      GVIR_CONFIG_OBJECT(target));
}

/**
 * gvir_config_storage_vol_set_backing_store:
 * @backing_store: (allow-none):
 */
void gvir_config_storage_vol_set_backing_store(GVirConfigStorageVol *vol,
                                               GVirConfigStorageVolBackingStore *backing_store)
{
    g_return_if_fail(GVIR_CONFIG_IS_STORAGE_VOL(vol));
    g_return_if_fail(backing_store == NULL ||
                     GVIR_CONFIG_IS_STORAGE_VOL_BACKING_STORE(backing_store));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(vol),
                                      "backingStore",
                                      GVIR_CONFIG_OBJECT(backing_store));
}
