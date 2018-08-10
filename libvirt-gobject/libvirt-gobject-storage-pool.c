/*
 * libvirt-gobject-storage_pool.c: libvirt glib integration
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

#define GVIR_STORAGE_POOL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_STORAGE_POOL, GVirStoragePoolPrivate))

struct _GVirStoragePoolPrivate
{
    GMutex *lock;
    virStoragePoolPtr handle;

    GHashTable *volumes;
    gchar uuid[VIR_UUID_STRING_BUFLEN];
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirStoragePool, gvir_storage_pool, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_STORAGE_POOL_ERROR gvir_storage_pool_error_quark()


static GQuark
gvir_storage_pool_error_quark(void)
{
    return g_quark_from_static_string("gvir-storage-pool");
}

static void gvir_storage_pool_get_property(GObject *object,
                                           guint prop_id,
                                           GValue *value,
                                           GParamSpec *pspec)
{
    GVirStoragePool *pool = GVIR_STORAGE_POOL(object);
    GVirStoragePoolPrivate *priv = pool->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_storage_pool_set_property(GObject *object,
                                           guint prop_id,
                                           const GValue *value,
                                           GParamSpec *pspec)
{
    GVirStoragePool *pool = GVIR_STORAGE_POOL(object);
    GVirStoragePoolPrivate *priv = pool->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virStoragePoolFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_storage_pool_finalize(GObject *object)
{
    GVirStoragePool *pool = GVIR_STORAGE_POOL(object);
    GVirStoragePoolPrivate *priv = pool->priv;

    if (priv->volumes) {
        g_hash_table_unref(priv->volumes);
        priv->volumes = NULL;
    }

    virStoragePoolFree(priv->handle);

    g_mutex_free(priv->lock);

    G_OBJECT_CLASS(gvir_storage_pool_parent_class)->finalize(object);
}


static void gvir_storage_pool_constructed(GObject *object)
{
    GVirStoragePool *pool = GVIR_STORAGE_POOL(object);
    GVirStoragePoolPrivate *priv = pool->priv;

    G_OBJECT_CLASS(gvir_storage_pool_parent_class)->constructed(object);

    /* xxx we may want to turn this into an initable */
    if (virStoragePoolGetUUIDString(priv->handle, priv->uuid) < 0)
        gvir_warning("Failed to get storage pool UUID on %p", priv->handle);
}


static void gvir_storage_pool_class_init(GVirStoragePoolClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_storage_pool_finalize;
    object_class->get_property = gvir_storage_pool_get_property;
    object_class->set_property = gvir_storage_pool_set_property;
    object_class->constructed = gvir_storage_pool_constructed;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The storage_pool handle",
                                                       GVIR_TYPE_STORAGE_POOL_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));
}


static void gvir_storage_pool_init(GVirStoragePool *pool)
{
    GVirStoragePoolPrivate *priv;

    priv = pool->priv = GVIR_STORAGE_POOL_GET_PRIVATE(pool);

    priv->lock = g_mutex_new();
}

typedef struct virStoragePool GVirStoragePoolHandle;

static GVirStoragePoolHandle*
gvir_storage_pool_handle_copy(GVirStoragePoolHandle *src)
{
    virStoragePoolRef((virStoragePoolPtr)src);
    return src;
}

static void
gvir_storage_pool_handle_free(GVirStoragePoolHandle *src)
{
    virStoragePoolFree((virStoragePoolPtr)src);
}

G_DEFINE_BOXED_TYPE(GVirStoragePoolHandle, gvir_storage_pool_handle,
                    gvir_storage_pool_handle_copy, gvir_storage_pool_handle_free)

static GVirStoragePoolInfo *
gvir_storage_pool_info_copy(GVirStoragePoolInfo *info)
{
    return g_slice_dup(GVirStoragePoolInfo, info);
}

static void
gvir_storage_pool_info_free(GVirStoragePoolInfo *info)
{
    g_slice_free(GVirStoragePoolInfo, info);
}

G_DEFINE_BOXED_TYPE(GVirStoragePoolInfo, gvir_storage_pool_info,
                    gvir_storage_pool_info_copy, gvir_storage_pool_info_free)

const gchar *gvir_storage_pool_get_name(GVirStoragePool *pool)
{
    const char *name;

    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), NULL);

    if (!(name = virStoragePoolGetName(pool->priv->handle)))
        gvir_warning("Failed to get storage_pool name on %p", pool->priv->handle);

    return name;
}


const gchar *gvir_storage_pool_get_uuid(GVirStoragePool *pool)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), NULL);

    return pool->priv->uuid;
}


gboolean gvir_storage_pool_get_active(GVirStoragePool *pool)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);

    return virStoragePoolIsActive(pool->priv->handle);
}


gboolean gvir_storage_pool_get_persistent(GVirStoragePool *pool)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);

    return virStoragePoolIsPersistent(pool->priv->handle);
}

/**
 * gvir_storage_pool_get_config:
 * @pool: the storage_pool
 * @flags: the flags
 * @err: Place-holder for possible errors
 *
 * Returns: (transfer full): the config. The returned object should be
 * unreffed with g_object_unref() when no longer needed.
 */
GVirConfigStoragePool *gvir_storage_pool_get_config(GVirStoragePool *pool,
                                                    guint flags,
                                                    GError **err)
{
    GVirStoragePoolPrivate *priv;
    gchar *xml;

    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = pool->priv;
    if (!(xml = virStoragePoolGetXMLDesc(priv->handle, flags))) {
        gvir_set_error_literal(err, GVIR_STORAGE_POOL_ERROR,
                               0,
                               "Unable to get storage_pool XML config");
        return NULL;
    }

    GVirConfigStoragePool *conf = gvir_config_storage_pool_new_from_xml(xml, err);

    free(xml);
    return conf;
}

/**
 * gvir_storage_pool_get_info:
 * @pool: the storage_pool
 * @err: Place-holder for possible errors
 *
 * Returns: (transfer full): the info. The returned pointer should be
 * freed using #g_boxed_free() when no longer needed.
 */
GVirStoragePoolInfo *gvir_storage_pool_get_info(GVirStoragePool *pool,
                                                GError **err)
{
    GVirStoragePoolPrivate *priv;
    virStoragePoolInfo info;
    GVirStoragePoolInfo *ret;

    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = pool->priv;
    if (virStoragePoolGetInfo(priv->handle, &info) < 0) {
        if (err)
            *err = gvir_error_new_literal(GVIR_STORAGE_POOL_ERROR,
                                          0,
                                          "Unable to get storage pool info");
        return NULL;
    }

    ret = g_slice_new(GVirStoragePoolInfo);
    ret->state = info.state;
    ret->capacity = info.capacity;
    ret->allocation = info.allocation;
    ret->available = info.available;

    return ret;
}

typedef gint (* CountFunction) (virStoragePoolPtr vpool);
typedef gint (* ListFunction) (virStoragePoolPtr vpool, gchar **lst, gint max);

static gchar ** fetch_list(virStoragePoolPtr vpool,
                           const char *name,
                           CountFunction count_func,
                           ListFunction list_func,
                           GCancellable *cancellable,
                           gint *length,
                           GError **err)
{
    gchar **lst = NULL;
    gint n = 0;

    if ((n = count_func(vpool)) < 0) {
        gvir_set_error(err, GVIR_STORAGE_POOL_ERROR,
                       0,
                       "Unable to count %s", name);
        goto error;
    }

    if (n) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto error;

        lst = g_new0(gchar *, n);
        if ((n = list_func(vpool, lst, n)) < 0) {
            gvir_set_error(err, GVIR_STORAGE_POOL_ERROR,
                           0,
                           "Unable to list %s %d", name, n);
            goto error;
        }
    }

    *length = n;
    return lst;

error:
    g_free(lst);
    return NULL;
}

/**
 * gvir_storage_pool_refresh:
 * @pool: the storage pool
 * @cancellable: (allow-none)(transfer none): cancellation object
 */
gboolean gvir_storage_pool_refresh(GVirStoragePool *pool,
                                   GCancellable *cancellable,
                                   GError **err)
{
    GVirStoragePoolPrivate *priv;
    GHashTable *vol_hash;
    gchar **volumes = NULL;
    gint nvolumes = 0;
    gboolean ret = FALSE;
    gint i;
    virStoragePoolPtr vpool = NULL;
    GError *lerr = NULL;

    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable),
                         FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    priv = pool->priv;
    vpool = priv->handle;

    if (virStoragePoolRefresh(vpool, 0) < 0) {
        gvir_set_error_literal(err, GVIR_STORAGE_POOL_ERROR,
                               0,
                               "Unable to refresh storage pool");
        goto cleanup;
    }

    volumes = fetch_list(vpool,
                         "Storage Volumes",
                         virStoragePoolNumOfVolumes,
                         virStoragePoolListVolumes,
                         cancellable,
                         &nvolumes,
                         &lerr);
    if (lerr) {
        g_propagate_error(err, lerr);
        lerr = NULL;
        goto cleanup;
    }

    if (g_cancellable_set_error_if_cancelled(cancellable, err))
        goto cleanup;

    vol_hash = g_hash_table_new_full(g_str_hash,
                                     g_str_equal,
                                     g_free,
                                     g_object_unref);

    for (i = 0 ; i < nvolumes ; i++) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto cleanup;

        virStorageVolPtr vvolume;
        GVirStorageVol *volume;

        vvolume = virStorageVolLookupByName(vpool, volumes[i]);
        if (!vvolume)
            continue;

        volume = GVIR_STORAGE_VOL(g_object_new(GVIR_TYPE_STORAGE_VOL,
                                               "handle", vvolume,
                                               "pool", pool,
                                               NULL));
        virStorageVolFree(vvolume);
        g_hash_table_insert(vol_hash, g_strdup(volumes[i]), volume);
    }

    g_mutex_lock(priv->lock);
    if (priv->volumes)
        g_hash_table_unref(priv->volumes);
    priv->volumes = vol_hash;
    g_mutex_unlock(priv->lock);

    ret = TRUE;

cleanup:
    for (i = 0 ; i < nvolumes ; i++)
        g_free(volumes[i]);
    g_free(volumes);
    return ret;
}

static void
gvir_storage_pool_refresh_helper(GTask *task,
                                 gpointer source_object,
                                 gpointer task_data G_GNUC_UNUSED,
                                 GCancellable *cancellable)
{
    GVirStoragePool *pool = GVIR_STORAGE_POOL(source_object);
    GError *err = NULL;

    if (!gvir_storage_pool_refresh(pool, cancellable, &err))
        g_task_return_error(task, err);
    else
        g_task_return_boolean(task, TRUE);
}

/**
 * gvir_storage_pool_refresh_async:
 * @pool: the storage pool
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 */
void gvir_storage_pool_refresh_async(GVirStoragePool *pool,
                                     GCancellable *cancellable,
                                     GAsyncReadyCallback callback,
                                     gpointer user_data)
{
    GTask *task;

    g_return_if_fail(GVIR_IS_STORAGE_POOL(pool));
    g_return_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable));

    task = g_task_new(G_OBJECT(pool),
                      cancellable,
                      callback,
                      user_data);
    g_task_run_in_thread(task, gvir_storage_pool_refresh_helper);
    g_object_unref(task);
}

/**
 * gvir_storage_pool_refresh_finish:
 * @pool: the storage pool
 * @result: (transfer none): async method result
 */
gboolean gvir_storage_pool_refresh_finish(GVirStoragePool *pool,
                                          GAsyncResult *result,
                                          GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(g_task_is_valid(result, pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    return g_task_propagate_boolean(G_TASK(result), err);
}

static void gvir_storage_vol_ref(gpointer obj, gpointer ignore G_GNUC_UNUSED)
{
    g_object_ref(obj);
}

/**
 * gvir_storage_pool_get_volumes:
 * @pool: the storage pool
 *
 * Return value: (element-type LibvirtGObject.StorageVol) (transfer full):
 * List of #GVirStorageVol.  The returned list should be freed with
 * g_list_free(), after its elements have been unreffed with
 * g_object_unref().
 */
GList *gvir_storage_pool_get_volumes(GVirStoragePool *pool)
{
    GVirStoragePoolPrivate *priv;
    GList *volumes = NULL;

    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), NULL);

    priv = pool->priv;
    g_mutex_lock(priv->lock);
    if (priv->volumes != NULL) {
        volumes = g_hash_table_get_values(priv->volumes);
        g_list_foreach(volumes, gvir_storage_vol_ref, NULL);
    } else {
        g_warn_if_reached();
    }
    g_mutex_unlock(priv->lock);

    return volumes;
}

/**
 * gvir_storage_pool_get_volume:
 * @pool: the storage pool
 * @name: Name of the requested storage volume
 *
 * Return value: (transfer full): the #GVirStorageVol, or NULL. The
 * returned object should be unreffed with g_object_unref() when no longer
 * needed.
 */
GVirStorageVol *gvir_storage_pool_get_volume(GVirStoragePool *pool,
                                             const gchar *name)
{
    GVirStoragePoolPrivate *priv;
    GVirStorageVol *volume;

    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), NULL);
    g_return_val_if_fail(name != NULL, NULL);

    priv = pool->priv;
    g_mutex_lock(priv->lock);
    if (priv->volumes != NULL) {
        volume = g_hash_table_lookup(priv->volumes, name);
        if (volume)
            g_object_ref(volume);
    } else {
        g_warn_if_reached();
        volume = NULL;
    }
    g_mutex_unlock(priv->lock);

    return volume;
}

/**
 * gvir_storage_pool_create_volume:
 * @pool: the storage pool in which to create the volume
 * @conf: the configuration for the new volume
 * @err: Place-holder for possible errors
 *
 * Returns: (transfer full): the newly created volume. The returned object
 * should be unreffed with g_object_unref() when no longer needed.
 */
GVirStorageVol *gvir_storage_pool_create_volume
                                (GVirStoragePool *pool,
                                 GVirConfigStorageVol *conf,
                                 GError **err)
{
    const gchar *xml;
    virStorageVolPtr handle;
    GVirStoragePoolPrivate *priv;

    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), NULL);
    g_return_val_if_fail(GVIR_CONFIG_IS_STORAGE_VOL(conf), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(conf));

    g_return_val_if_fail(xml != NULL, NULL);

    priv = pool->priv;
    if (!(handle = virStorageVolCreateXML(priv->handle, xml, 0))) {
        gvir_set_error_literal(err, GVIR_STORAGE_POOL_ERROR,
                               0,
                               "Failed to create volume");
        return NULL;
    }

    GVirStorageVol *volume;
    const char *name;

    volume = GVIR_STORAGE_VOL(g_object_new(GVIR_TYPE_STORAGE_VOL,
                                           "handle", handle,
                                           "pool", pool,
                                           NULL));
    name = gvir_storage_vol_get_name(volume);
    if (name == NULL) {
        g_object_unref(G_OBJECT(volume));
        return NULL;
    }

    g_mutex_lock(priv->lock);
    if (priv->volumes != NULL) {
        g_hash_table_insert(priv->volumes, g_strdup(name), volume);
    } else {
        g_warn_if_reached();
        g_object_unref(G_OBJECT(volume));
        g_mutex_unlock(priv->lock);
        return NULL;
    }
    g_mutex_unlock(priv->lock);

    return g_object_ref(volume);
}

/**
 * gvir_storage_pool_build:
 * @pool: the storage pool to build
 * @flags:  the flags
 * @err: return location for any #GError
 *
 * Return value: #True on success, #False otherwise.
 */
gboolean gvir_storage_pool_build (GVirStoragePool *pool,
                                  guint flags,
                                  GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virStoragePoolBuild(pool->priv->handle, flags)) {
        gvir_set_error_literal(err, GVIR_STORAGE_POOL_ERROR,
                               0,
                               "Failed to build storage pool");
        return FALSE;
    }

    return TRUE;
}

static void
gvir_storage_pool_build_helper(GTask *task,
                               gpointer source_object,
                               gpointer task_data,
                               GCancellable *cancellable G_GNUC_UNUSED)
{
    GVirStoragePool *pool = GVIR_STORAGE_POOL(source_object);
    guint flags = GPOINTER_TO_UINT(task_data);
    GError *err = NULL;

    if (!gvir_storage_pool_build(pool, flags, &err))
        g_task_return_error(task, err);
    else
        g_task_return_boolean(task, TRUE);
}

/**
 * gvir_storage_pool_build_async:
 * @pool: the storage pool to build
 * @flags:  the flags
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 */
void gvir_storage_pool_build_async (GVirStoragePool *pool,
                                    guint flags,
                                    GCancellable *cancellable,
                                    GAsyncReadyCallback callback,
                                    gpointer user_data)
{
    GTask *task;

    g_return_if_fail(GVIR_IS_STORAGE_POOL(pool));
    g_return_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable));

    task = g_task_new(G_OBJECT(pool),
                      cancellable,
                      callback,
                      user_data);
    g_task_set_task_data(task, GUINT_TO_POINTER(flags), NULL);
    g_task_run_in_thread(task, gvir_storage_pool_build_helper);
    g_object_unref(task);
}

/**
 * gvir_storage_pool_build_finish:
 * @pool: the storage pool to build
 * @result: (transfer none): async method result
 * @err: return location for any #GError
 *
 * Return value: #True on success, #False otherwise.
 */
gboolean gvir_storage_pool_build_finish(GVirStoragePool *pool,
                                        GAsyncResult *result,
                                        GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(g_task_is_valid(result, pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    return g_task_propagate_boolean(G_TASK(result), err);
}

/**
 * gvir_storage_pool_undefine:
 * @pool: the storage pool to undefine
 * @err: return location for any #GError
 *
 * Return value: #True on success, #False otherwise.
 */
gboolean gvir_storage_pool_undefine (GVirStoragePool *pool,
                                     GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virStoragePoolUndefine(pool->priv->handle)) {
        gvir_set_error_literal(err, GVIR_STORAGE_POOL_ERROR,
                               0,
                               "Failed to undefine storage pool");
        return FALSE;
    }

    return TRUE;
}

static void
gvir_storage_pool_undefine_helper(GTask *task,
                                  gpointer source_object,
                                  gpointer task_data G_GNUC_UNUSED,
                                  GCancellable *cancellable G_GNUC_UNUSED)
{
    GVirStoragePool *pool = GVIR_STORAGE_POOL(source_object);
    GError *err = NULL;

    if (!gvir_storage_pool_undefine(pool, &err))
        g_task_return_error(task, err);
    else
        g_task_return_boolean(task, TRUE);
}

/**
 * gvir_storage_pool_undefine_async:
 * @pool: the storage pool to undefine
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 */
void gvir_storage_pool_undefine_async (GVirStoragePool *pool,
                                       GCancellable *cancellable,
                                       GAsyncReadyCallback callback,
                                       gpointer user_data)
{
    GTask *task;

    g_return_if_fail(GVIR_IS_STORAGE_POOL(pool));
    g_return_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable));

    task = g_task_new(G_OBJECT(pool),
                      cancellable,
                      callback,
                      user_data);
    g_task_run_in_thread(task, gvir_storage_pool_undefine_helper);
    g_object_unref(task);
}

/**
 * gvir_storage_pool_undefine_finish:
 * @pool: the storage pool to undefine
 * @result: (transfer none): async method result
 * @err: return location for any #GError
 *
 * Return value: #True on success, #False otherwise.
 */
gboolean gvir_storage_pool_undefine_finish(GVirStoragePool *pool,
                                           GAsyncResult *result,
                                           GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(g_task_is_valid(result, pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    return g_task_propagate_boolean(G_TASK(result), err);
}

/**
 * gvir_storage_pool_start:
 * @pool: the storage pool to start
 * @flags:  the flags
 * @err: return location for any #GError
 *
 * Return value: #True on success, #False otherwise.
 */
gboolean gvir_storage_pool_start (GVirStoragePool *pool,
                                  guint flags,
                                  GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virStoragePoolCreate(pool->priv->handle, flags)) {
        gvir_set_error_literal(err, GVIR_STORAGE_POOL_ERROR,
                               0,
                               "Failed to start storage pool");
        return FALSE;
    }

    return TRUE;
}

static void
gvir_storage_pool_start_helper(GTask *task,
                               gpointer source_object,
                               gpointer task_data,
                               GCancellable *cancellable G_GNUC_UNUSED)
{
    GVirStoragePool *pool = GVIR_STORAGE_POOL(source_object);
    guint flags = GPOINTER_TO_UINT(task_data);
    GError *err = NULL;

    if (!gvir_storage_pool_start(pool, flags, &err))
        g_task_return_error(task, err);
    else
        g_task_return_boolean(task, TRUE);
}

/**
 * gvir_storage_pool_start_async:
 * @pool: the storage pool to start
 * @flags:  the flags
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 */
void gvir_storage_pool_start_async (GVirStoragePool *pool,
                                    guint flags,
                                    GCancellable *cancellable,
                                    GAsyncReadyCallback callback,
                                    gpointer user_data)
{
    GTask *task;

    g_return_if_fail(GVIR_IS_STORAGE_POOL(pool));
    g_return_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable));

    task = g_task_new(G_OBJECT(pool),
                      cancellable,
                      callback,
                      user_data);
    g_task_set_task_data(task, GUINT_TO_POINTER(flags), NULL);
    g_task_run_in_thread(task, gvir_storage_pool_start_helper);
    g_object_unref(task);
}

/**
 * gvir_storage_pool_start_finish:
 * @pool: the storage pool to start
 * @result: (transfer none): async method result
 * @err: return location for any #GError
 *
 * Return value: #True on success, #False otherwise.
 */
gboolean gvir_storage_pool_start_finish(GVirStoragePool *pool,
                                        GAsyncResult *result,
                                        GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(g_task_is_valid(result, pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    return g_task_propagate_boolean(G_TASK(result), err);
}

/**
 * gvir_storage_pool_stop:
 * @pool: the storage pool to stop
 * @err: return location for any #GError
 *
 * Return value: #True on success, #False otherwise.
 */
gboolean gvir_storage_pool_stop (GVirStoragePool *pool,
                                 GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virStoragePoolDestroy(pool->priv->handle)) {
        gvir_set_error_literal(err, GVIR_STORAGE_POOL_ERROR,
                               0,
                               "Failed to stop storage pool");
        return FALSE;
    }

    return TRUE;
}

static void
gvir_storage_pool_stop_helper(GTask *task,
                              gpointer source_object,
                              gpointer task_data G_GNUC_UNUSED,
                              GCancellable *cancellable G_GNUC_UNUSED)
{
    GVirStoragePool *pool = GVIR_STORAGE_POOL(source_object);
    GError *err = NULL;

    if (!gvir_storage_pool_stop(pool, &err))
        g_task_return_error(task, err);
    else
        g_task_return_boolean(task, TRUE);
}

/**
 * gvir_storage_pool_stop_async:
 * @pool: the storage pool to stop
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 */
void gvir_storage_pool_stop_async (GVirStoragePool *pool,
                                   GCancellable *cancellable,
                                   GAsyncReadyCallback callback,
                                   gpointer user_data)
{
    GTask *task;

    g_return_if_fail(GVIR_IS_STORAGE_POOL(pool));
    g_return_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable));

    task = g_task_new(G_OBJECT(pool),
                      cancellable,
                      callback,
                      user_data);
    g_task_run_in_thread(task, gvir_storage_pool_stop_helper);
    g_object_unref(task);
}

/**
 * gvir_storage_pool_stop_finish:
 * @pool: the storage pool to stop
 * @result: (transfer none): async method result
 * @err: return location for any #GError
 *
 * Return value: #True on success, #False otherwise.
 */
gboolean gvir_storage_pool_stop_finish(GVirStoragePool *pool,
                                       GAsyncResult *result,
                                       GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(g_task_is_valid(result, pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    return g_task_propagate_boolean(G_TASK(result), err);
}

/**
 * gvir_storage_pool_delete:
 * @pool: the storage pool to delete
 * @flags:  the flags
 * @err: return location for any #GError
 *
 * Return value: #True on success, #False otherwise.
 */
gboolean gvir_storage_pool_delete (GVirStoragePool *pool,
                                   guint flags,
                                   GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virStoragePoolDelete(pool->priv->handle, flags)) {
        gvir_set_error_literal(err, GVIR_STORAGE_POOL_ERROR,
                               0,
                               "Failed to delete storage pool");
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_storage_pool_get_autostart:
 * @pool: the storage pool
 * @err: return location for any #GError
 *
 * Return value: #True if autostart is enabled, #False otherwise.
 */
gboolean gvir_storage_pool_get_autostart(GVirStoragePool *pool,
                                         GError **err)
{
    int ret;

    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virStoragePoolGetAutostart(pool->priv->handle, &ret)) {
        gvir_set_error_literal(err, GVIR_STORAGE_POOL_ERROR,
                               0,
                               "Failed to get autostart flag from storage pool");
    }

    return !!ret;
}

/**
 * gvir_storage_pool_set_autostart:
 * @pool: the storage pool
 * @autostart: Whether or not to autostart
 * @err: return location for any #GError
 *
 * Sets whether or not storage pool @pool is started automatically on boot.
 *
 * Return value: #TRUE on success, #FALSE otherwise.
 */
gboolean gvir_storage_pool_set_autostart(GVirStoragePool *pool,
                                         gboolean autostart,
                                         GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virStoragePoolSetAutostart(pool->priv->handle, autostart)) {
        gvir_set_error_literal(err, GVIR_STORAGE_POOL_ERROR,
                               0,
                               "Failed to set autostart flag on storage pool");
        return FALSE;
    }

    return TRUE;
}

static void
gvir_storage_pool_delete_helper(GTask *task,
                                gpointer source_object,
                                gpointer task_data,
                                GCancellable *cancellable G_GNUC_UNUSED)
{
    GVirStoragePool *pool = GVIR_STORAGE_POOL(source_object);
    guint flags = GPOINTER_TO_UINT(task_data);
    GError *err = NULL;

    if (!gvir_storage_pool_delete(pool, flags, &err))
        g_task_return_error(task, err);
    else
        g_task_return_boolean(task, TRUE);
}

/**
 * gvir_storage_pool_delete_async:
 * @pool: the storage pool to delete
 * @flags:  the flags
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 */
void gvir_storage_pool_delete_async (GVirStoragePool *pool,
                                     guint flags,
                                     GCancellable *cancellable,
                                     GAsyncReadyCallback callback,
                                     gpointer user_data)
{
    GTask *task;

    g_return_if_fail(GVIR_IS_STORAGE_POOL(pool));
    g_return_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable));

    task = g_task_new(G_OBJECT(pool),
                      cancellable,
                      callback,
                      user_data);
    g_task_set_task_data(task, GUINT_TO_POINTER(flags), NULL);
    g_task_run_in_thread(task, gvir_storage_pool_delete_helper);
    g_object_unref(task);
}

/**
 * gvir_storage_pool_delete_finish:
 * @pool: the storage pool to delete
 * @result: (transfer none): async method result
 * @err: return location for any #GError
 *
 * Return value: #True on success, #False otherwise.
 */
gboolean gvir_storage_pool_delete_finish(GVirStoragePool *pool,
                                         GAsyncResult *result,
                                         GError **err)
{
    g_return_val_if_fail(GVIR_IS_STORAGE_POOL(pool), FALSE);
    g_return_val_if_fail(g_task_is_valid(result, pool), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    return g_task_propagate_boolean(G_TASK(result), err);
}

G_GNUC_INTERNAL void gvir_storage_pool_delete_vol(GVirStoragePool *pool,
                                                  GVirStorageVol *volume)
{
    GVirStoragePoolPrivate *priv;

    g_return_if_fail(GVIR_IS_STORAGE_POOL(pool));
    g_return_if_fail(GVIR_IS_STORAGE_VOL(volume));

    priv = pool->priv;
    g_mutex_lock(priv->lock);
    if (priv->volumes != NULL) {
        const gchar *name = gvir_storage_vol_get_name(volume);
        g_hash_table_remove(priv->volumes, name);
    } else {
        g_warn_if_reached();
    }
    g_mutex_unlock(priv->lock);
}
