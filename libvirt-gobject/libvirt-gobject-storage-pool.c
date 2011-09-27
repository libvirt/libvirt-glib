/*
 * libvirt-gobject-storage_pool.c: libvirt glib integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010 Red Hat
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_STORAGE_POOL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_STORAGE_POOL, GVirStoragePoolPrivate))

struct _GVirStoragePoolPrivate
{
    GMutex *lock;
    virStoragePoolPtr handle;

    GHashTable *volumes;
};

G_DEFINE_TYPE(GVirStoragePool, gvir_storage_pool, G_TYPE_OBJECT);


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

    DEBUG("Finalize GVirStoragePool=%p", pool);

    if (priv->volumes) {
        g_hash_table_unref(priv->volumes);
        priv->volumes = NULL;
    }

    virStoragePoolFree(priv->handle);

    g_mutex_free(priv->lock);

    G_OBJECT_CLASS(gvir_storage_pool_parent_class)->finalize(object);
}


static void gvir_storage_pool_class_init(GVirStoragePoolClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_storage_pool_finalize;
    object_class->get_property = gvir_storage_pool_get_property;
    object_class->set_property = gvir_storage_pool_set_property;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The storage_pool handle",
                                                       GVIR_TYPE_STORAGE_POOL_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_NAME |
                                                       G_PARAM_STATIC_NICK |
                                                       G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirStoragePoolPrivate));
}


static void gvir_storage_pool_init(GVirStoragePool *pool)
{
    GVirStoragePoolPrivate *priv;

    DEBUG("Init GVirStoragePool=%p", pool);

    priv = pool->priv = GVIR_STORAGE_POOL_GET_PRIVATE(pool);

    memset(priv, 0, sizeof(*priv));

    priv->lock = g_mutex_new();
}

static gpointer
gvir_storage_pool_handle_copy(gpointer src)
{
    virStoragePoolRef(src);
    return src;
}


GType gvir_storage_pool_handle_get_type(void)
{
    static GType handle_type = 0;

    if (G_UNLIKELY(handle_type == 0))
        handle_type = g_boxed_type_register_static
            ("GVirStoragePoolHandle",
             gvir_storage_pool_handle_copy,
             (GBoxedFreeFunc)virStoragePoolFree);

    return handle_type;
}


const gchar *gvir_storage_pool_get_name(GVirStoragePool *pool)
{
    GVirStoragePoolPrivate *priv = pool->priv;
    const char *name;

    if (!(name = virStoragePoolGetName(priv->handle))) {
        g_error("Failed to get storage_pool name on %p", priv->handle);
    }

    return name;
}


gchar *gvir_storage_pool_get_uuid(GVirStoragePool *pool)
{
    GVirStoragePoolPrivate *priv = pool->priv;
    char *uuid = g_new(gchar, VIR_UUID_STRING_BUFLEN);

    if (virStoragePoolGetUUIDString(priv->handle, uuid) < 0) {
        g_error("Failed to get storage_pool UUID on %p", priv->handle);
    }
    return uuid;
}


/**
 * gvir_storage_pool_get_config:
 * @pool: the storage_pool
 * @flags: the flags
 * Returns: (transfer full): the config
 */
GVirConfigStoragePool *gvir_storage_pool_get_config(GVirStoragePool *pool,
                                                    guint64 flags,
                                                    GError **err)
{
    GVirStoragePoolPrivate *priv = pool->priv;
    gchar *xml;

    if (!(xml = virStoragePoolGetXMLDesc(priv->handle, flags))) {
        *err = gvir_error_new_literal(GVIR_STORAGE_POOL_ERROR,
                                      0,
                                      "Unable to get storage_pool XML config");
        return NULL;
    }

    GVirConfigStoragePool *conf = gvir_config_storage_pool_new(xml);

    g_free(xml);
    return conf;
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
    gint i;

    if ((n = count_func(vpool)) < 0) {
        *err = gvir_error_new(GVIR_STORAGE_POOL_ERROR,
                              0,
                              "Unable to count %s", name);
        goto error;
    }

    if (n) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto error;

        lst = g_new(gchar *, n);
        if ((n = list_func(vpool, lst, n)) < 0) {
            *err = gvir_error_new(GVIR_STORAGE_POOL_ERROR,
                                  0,
                                  "Unable to list %s %d", name, n);
            goto error;
        }
    }

    *length = n;
    return lst;

error:
    for (i = 0 ; i < n; i++)
        g_free(lst[i]);
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
    GVirStoragePoolPrivate *priv = pool->priv;
    GHashTable *vol_hash;
    gchar **volumes = NULL;
    gint nvolumes = 0;
    gboolean ret = FALSE;
    gint i;
    virStoragePoolPtr vpool = NULL;

    vpool = priv->handle;

    if (virStoragePoolRefresh(vpool, 0) < 0) {
        *err = gvir_error_new_literal(GVIR_STORAGE_POOL_ERROR,
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
                        err);
    if (*err != NULL)
        goto cleanup;

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
                                               NULL));

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
gvir_storage_pool_refresh_helper(GSimpleAsyncResult *res,
                                 GObject *object,
                                 GCancellable *cancellable)
{
    GVirStoragePool *pool = GVIR_STORAGE_POOL(object);
    GError *err = NULL;

    if (!gvir_storage_pool_refresh(pool, cancellable, &err)) {
        g_simple_async_result_set_from_error(res, err);
        g_error_free(err);
    }
}

/**
 * gvir_storage_pool_refresh_async:
 * @pool: the storage pool
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (transfer none): completion callback
 * @opaque: (transfer none)(allow-none): opaque data for callback
 */
void gvir_storage_pool_refresh_async(GVirStoragePool *pool,
                                     GCancellable *cancellable,
                                     GAsyncReadyCallback callback,
                                     gpointer opaque)
{
    GSimpleAsyncResult *res;

    res = g_simple_async_result_new(G_OBJECT(pool),
                                    callback,
                                    opaque,
                                    gvir_storage_pool_refresh);
    g_simple_async_result_run_in_thread(res,
                                        gvir_storage_pool_refresh_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
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
    g_return_val_if_fail(G_IS_ASYNC_RESULT(result), FALSE);

    if (G_IS_SIMPLE_ASYNC_RESULT(result)) {
        GSimpleAsyncResult *simple = G_SIMPLE_ASYNC_RESULT(result);
        g_warn_if_fail (g_simple_async_result_get_source_tag(simple) ==
                        gvir_storage_pool_refresh);
        if (g_simple_async_result_propagate_error(simple, err))
            return FALSE;
    }

    return TRUE;
}

static void gvir_storage_vol_ref(gpointer obj, gpointer ignore G_GNUC_UNUSED)
{
    g_object_ref(obj);
}

/**
 * gvir_storage_pool_get_volumes:
 * @pool: the storage pool
 *
 * Return value: (element-type LibvirtGObject.StoragePool) (transfer full): List
 * of #GVirStorageVol
 */
GList *gvir_storage_pool_get_volumes(GVirStoragePool *pool)
{
    GVirStoragePoolPrivate *priv = pool->priv;
    GList *volumes;

    g_mutex_lock(priv->lock);
    volumes = g_hash_table_get_values(priv->volumes);
    g_list_foreach(volumes, gvir_storage_vol_ref, NULL);
    g_mutex_unlock(priv->lock);

    return volumes;
}

/**
 * gvir_storage_pool_get_volume:
 * @pool: the storage pool
 * @name: Name of the requested storage volume
 *
 * Return value: (transfer full): the #GVirStorageVol, or NULL
 */
GVirStoragePool *gvir_storage_pool_get_volume(GVirStoragePool *pool,
                                              const gchar *name)
{
    GVirStoragePoolPrivate *priv = pool->priv;
    GVirStoragePool *volume;

    g_mutex_lock(priv->lock);
    volume = g_hash_table_lookup(priv->volumes, name);
    if (volume)
        g_object_ref(volume);
    g_mutex_unlock(priv->lock);

    return volume;
}

/**
 * gvir_storage_pool_create_volume:
 * @pool: the storage pool in which to create the volume
 * @conf: the configuration for the new volume
 * Returns: (transfer full): the newly created volume
 */
GVirStorageVol *gvir_storage_pool_create_volume
                                (GVirStoragePool *pool,
                                 GVirConfigStorageVol *conf,
                                 GError **err)
{
    const gchar *xml;
    virStorageVolPtr handle;
    GVirStoragePoolPrivate *priv = pool->priv;

    xml = gvir_config_object_get_doc(GVIR_CONFIG_OBJECT(conf));

    g_return_val_if_fail(xml != NULL, NULL);

    if (!(handle = virStorageVolCreateXML(priv->handle, xml, 0))) {
        *err = gvir_error_new_literal(GVIR_STORAGE_POOL_ERROR,
                                      0,
                                      "Failed to create volume");
        return NULL;
    }

    GVirStorageVol *volume;

    volume = GVIR_STORAGE_VOL(g_object_new(GVIR_TYPE_STORAGE_VOL,
                                           "handle", handle,
                                           NULL));

    g_mutex_lock(priv->lock);
    g_hash_table_insert(priv->volumes,
                        g_strdup(gvir_storage_vol_get_name(volume)),
                        volume);
    g_mutex_unlock(priv->lock);

    return g_object_ref(volume);
}
