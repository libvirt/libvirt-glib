/*
 * libvirt-gobject-storage_vol.c: libvirt glib integration
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

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-compat.h"
#include "libvirt-gobject-storage-pool-private.h"

#define GVIR_STORAGE_VOL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_STORAGE_VOL, GVirStorageVolPrivate))

struct _GVirStorageVolPrivate
{
    virStorageVolPtr handle;

    GVirStoragePool *pool;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirStorageVol, gvir_storage_vol, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
    PROP_POOL,
};


#define GVIR_STORAGE_VOL_ERROR gvir_storage_vol_error_quark()


static GQuark
gvir_storage_vol_error_quark(void)
{
    return g_quark_from_static_string("gvir-storage-vol");
}

static void gvir_storage_vol_get_property(GObject *object,
                                          guint prop_id,
                                          GValue *value,
                                          GParamSpec *pspec)
{
    GVirStorageVol *vol = GVIR_STORAGE_VOL(object);
    GVirStorageVolPrivate *priv = vol->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    case PROP_POOL:
        g_value_set_object(value, priv->pool);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_storage_vol_set_property(GObject *object,
                                          guint prop_id,
                                          const GValue *value,
                                          GParamSpec *pspec)
{
    GVirStorageVol *vol = GVIR_STORAGE_VOL(object);
    GVirStorageVolPrivate *priv = vol->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virStorageVolFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;
    case PROP_POOL:
        priv->pool = g_value_get_object(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_storage_vol_finalize(GObject *object)
{
    GVirStorageVol *vol = GVIR_STORAGE_VOL(object);
    GVirStorageVolPrivate *priv = vol->priv;

    virStorageVolFree(priv->handle);

    G_OBJECT_CLASS(gvir_storage_vol_parent_class)->finalize(object);
}


static void gvir_storage_vol_class_init(GVirStorageVolClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_storage_vol_finalize;
    object_class->get_property = gvir_storage_vol_get_property;
    object_class->set_property = gvir_storage_vol_set_property;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The storage_vol handle",
                                                       GVIR_TYPE_STORAGE_VOL_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));

    g_object_class_install_property(object_class,
                                    PROP_POOL,
                                    g_param_spec_object("pool",
                                                        "Pool",
                                                        "The containing storage pool",
                                                        GVIR_TYPE_STORAGE_POOL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));
}


static void gvir_storage_vol_init(GVirStorageVol *vol)
{
    vol->priv = GVIR_STORAGE_VOL_GET_PRIVATE(vol);
}

typedef struct virStorageVol GVirStorageVolHandle;

static GVirStorageVolHandle*
gvir_storage_vol_handle_copy(GVirStorageVolHandle *src)
{
    virStorageVolRef((virStorageVolPtr)src);
    return src;
}

static void
gvir_storage_vol_handle_free(GVirStorageVolHandle *src)
{
    virStorageVolFree((virStorageVolPtr)src);
}

G_DEFINE_BOXED_TYPE(GVirStorageVolHandle, gvir_storage_vol_handle,
                    gvir_storage_vol_handle_copy, gvir_storage_vol_handle_free)

static GVirStorageVolInfo *
gvir_storage_vol_info_copy(GVirStorageVolInfo *info)
{
    return g_slice_dup(GVirStorageVolInfo, info);
}

static void
gvir_storage_vol_info_free(GVirStorageVolInfo *info)
{
    g_slice_free(GVirStorageVolInfo, info);
}

G_DEFINE_BOXED_TYPE(GVirStorageVolInfo, gvir_storage_vol_info,
                    gvir_storage_vol_info_copy, gvir_storage_vol_info_free)

const gchar *gvir_storage_vol_get_name(GVirStorageVol *vol)
{
    const char *name;

    g_return_val_if_fail(GVIR_IS_STORAGE_VOL(vol), NULL);

    if (!(name = virStorageVolGetName(vol->priv->handle))) {
        gvir_warning("Failed to get storage_vol name on %p", vol->priv->handle);
        return NULL;
    }

    return name;
}

const gchar *gvir_storage_vol_get_path(GVirStorageVol *vol, GError **error)
{
    const char *path;

    g_return_val_if_fail(GVIR_IS_STORAGE_VOL(vol), NULL);
    g_return_val_if_fail(error == NULL || *error == NULL, NULL);

    if (!(path = virStorageVolGetPath(vol->priv->handle))) {
        gvir_set_error(error, GVIR_STORAGE_VOL_ERROR, 0,
                       "Failed to get storage_vol path on %p",
                       vol->priv->handle);
        return NULL;
    }

    return path;
}

/**
 * gvir_storage_vol_get_config:
 * @vol: the storage_vol
 * @flags: the flags
 * @err: Place-holder for possible errors
 *
 * Returns: (transfer full): the config. The returned object should be
 * unreffed with g_object_unref() when no longer needed.
 */
GVirConfigStorageVol *gvir_storage_vol_get_config(GVirStorageVol *vol,
                                                  guint flags,
                                                  GError **err)
{
    GVirStorageVolPrivate *priv;
    gchar *xml;

    g_return_val_if_fail(GVIR_IS_STORAGE_VOL(vol), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = vol->priv;
    if (!(xml = virStorageVolGetXMLDesc(priv->handle, flags))) {
        gvir_set_error_literal(err, GVIR_STORAGE_VOL_ERROR,
                               0,
                               "Unable to get storage_vol XML config");
        return NULL;
    }

    GVirConfigStorageVol *conf = gvir_config_storage_vol_new_from_xml(xml, err);

    free(xml);
    return conf;
}

/**
 * gvir_storage_vol_get_info:
 * @vol: the storage_vol
 * @err: Place-holder for possible errors
 *
 * Returns: (transfer full): the info. The returned object should be
 * unreffed with g_object_unref() when no longer needed.
 */
GVirStorageVolInfo *gvir_storage_vol_get_info(GVirStorageVol *vol,
                                              GError **err)
{
    GVirStorageVolPrivate *priv;
    virStorageVolInfo info;
    GVirStorageVolInfo *ret;

    g_return_val_if_fail(GVIR_IS_STORAGE_VOL(vol), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = vol->priv;
    if (virStorageVolGetInfo(priv->handle, &info) < 0) {
        if (err)
            *err = gvir_error_new_literal(GVIR_STORAGE_VOL_ERROR,
                                          0,
                                          "Unable to get storage vol info");
        return NULL;
    }

    ret = g_slice_new(GVirStorageVolInfo);
    ret->type = info.type;
    ret->capacity = info.capacity;
    ret->allocation = info.allocation;

    return ret;
}

/**
 * gvir_storage_vol_delete:
 * @vol: the storage volume to delete
 * @flags: the flags
 * @err: Return location for errors, or NULL
 *
 * Deletes the storage volume @vol.
 *
 * Returns: %TRUE on success, %FALSE otherwise
 */
gboolean gvir_storage_vol_delete(GVirStorageVol *vol,
                                 guint flags,
                                 GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_VOL(vol), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    gvir_storage_pool_delete_vol(vol->priv->pool, vol);

    if (virStorageVolDelete(vol->priv->handle, flags) < 0) {
        gvir_set_error_literal(err,
                               GVIR_STORAGE_VOL_ERROR,
                               0,
                               "Unable to delete storage volume");
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_storage_vol_resize:
 * @vol: the storage volume to resize
 * @capacity: the new capacity of the volume
 * @flags: (type GVirStorageVolResizeFlags): the flags
 * @err: Return location for errors, or NULL
 *
 * Changes the capacity of the storage volume @vol to @capacity.
 *
 * Returns: #TRUE success, #FALSE otherwise
 */
gboolean gvir_storage_vol_resize(GVirStorageVol *vol,
                                 guint64 capacity,
                                 guint flags,
                                 GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_VOL(vol), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virStorageVolResize(vol->priv->handle, capacity, flags) < 0) {
        gvir_set_error_literal(err,
                               GVIR_STORAGE_VOL_ERROR,
                               0,
                               "Unable to resize volume storage");
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_storage_vol_download:
 * @vol: the storage volume to download from
 * @stream: stream to use as output
 * @offset: position in @vol to start reading from
 * @length: limit on amount of data to download, or 0 for downloading all data
 * @flags: extra flags, not used yet, pass 0
 *
 * Returns: #TRUE of success, #FALSE otherwise
 */
gboolean gvir_storage_vol_download(GVirStorageVol *vol,
                                   GVirStream *stream,
                                   guint64 offset,
                                   guint64 length,
                                   guint flags G_GNUC_UNUSED,
                                   GError **err)
{
    virStreamPtr stream_handle = NULL;
    gboolean ret = FALSE;

    g_object_get(stream, "handle", &stream_handle, NULL);

    g_return_val_if_fail(GVIR_IS_STORAGE_VOL(vol), FALSE);
    g_return_val_if_fail(GVIR_IS_STREAM(stream), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virStorageVolDownload(vol->priv->handle,
                              stream_handle,
                              offset,
                              length,
                              0) < 0) {
        gvir_set_error_literal(err,
                               GVIR_STORAGE_VOL_ERROR,
                               0,
                               "Unable to download volume storage");

        goto cleanup;
    }

    ret = TRUE;
cleanup:
    if (stream_handle != NULL)
        virStreamFree(stream_handle);
    return ret;
}

/**
 * gvir_storage_vol_upload:
 * @vol: the storage volume to upload
 * @stream: stream to use as input
 * @offset: position in @vol to start to write to
 * @length: limit on amount of data to upload, or 0 for uploading all data
 * @flags: the flags, not set yet, pass 0
 *
 * Returns: #TRUE of success, #FALSE otherwise
 */
gboolean gvir_storage_vol_upload(GVirStorageVol *vol,
                                 GVirStream *stream,
                                 guint64 offset,
                                 guint64 length,
                                 guint flags G_GNUC_UNUSED,
                                 GError **err)
{
    virStreamPtr stream_handle = NULL;
    gboolean ret = FALSE;

    g_object_get(stream, "handle", &stream_handle, NULL);

    g_return_val_if_fail(GVIR_IS_STORAGE_VOL(vol), FALSE);
    g_return_val_if_fail(GVIR_IS_STREAM(stream), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virStorageVolUpload(vol->priv->handle,
                            stream_handle,
                            offset,
                            length,
                            0) < 0) {
        gvir_set_error_literal(err,
                               GVIR_STORAGE_VOL_ERROR,
                               0,
                               "Unable to upload to stream");

        goto cleanup;
    }

    ret = TRUE;
cleanup:
    if (stream_handle != NULL)
        virStreamFree(stream_handle);
    return ret;
}
