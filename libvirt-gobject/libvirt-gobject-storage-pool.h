/*
 * libvirt-gobject-storage_pool.c: libvirt gobject integration
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

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#ifndef __LIBVIRT_GOBJECT_STORAGE_POOL_H__
#define __LIBVIRT_GOBJECT_STORAGE_POOL_H__

G_BEGIN_DECLS

#define GVIR_TYPE_STORAGE_POOL            (gvir_storage_pool_get_type ())
#define GVIR_STORAGE_POOL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_STORAGE_POOL, GVirStoragePool))
#define GVIR_STORAGE_POOL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_STORAGE_POOL, GVirStoragePoolClass))
#define GVIR_IS_STORAGE_POOL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_STORAGE_POOL))
#define GVIR_IS_STORAGE_POOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_STORAGE_POOL))
#define GVIR_STORAGE_POOL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_STORAGE_POOL, GVirStoragePoolClass))

#define GVIR_TYPE_STORAGE_POOL_INFO       (gvir_storage_pool_info_get_type())
#define GVIR_TYPE_STORAGE_POOL_HANDLE     (gvir_storage_pool_handle_get_type())

typedef struct _GVirStoragePool GVirStoragePool;
typedef struct _GVirStoragePoolPrivate GVirStoragePoolPrivate;
typedef struct _GVirStoragePoolClass GVirStoragePoolClass;

struct _GVirStoragePool
{
    GObject parent;

    GVirStoragePoolPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirStoragePoolClass
{
    GObjectClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_STORAGE_POOL_STATE_INACTIVE     = 0, /* Not running */
    GVIR_STORAGE_POOL_STATE_BUILDING     = 1, /* Initializing pool, not available */
    GVIR_STORAGE_POOL_STATE_RUNNING      = 2, /* Running normally */
    GVIR_STORAGE_POOL_STATE_DEGRADED     = 3, /* Running degraded */
    GVIR_STORAGE_POOL_STATE_INACCESSIBLE = 4, /* Running, but not accessible */
} GVirStoragePoolState;

typedef struct _GVirStoragePoolInfo GVirStoragePoolInfo;
struct _GVirStoragePoolInfo
{
    GVirStoragePoolState state; /* the state */
    guint64 capacity;           /* Logical size bytes */
    guint64 allocation;         /* Current allocation bytes */
    guint64 available;          /* Remaining free space bytes */
};

GType gvir_storage_pool_get_type(void);
GType gvir_storage_pool_info_get_type(void);
GType gvir_storage_pool_handle_get_type(void);

const gchar *gvir_storage_pool_get_name(GVirStoragePool *pool);
const gchar *gvir_storage_pool_get_uuid(GVirStoragePool *pool);
gboolean gvir_storage_pool_get_active(GVirStoragePool *pool);
gboolean gvir_storage_pool_get_persistent(GVirStoragePool *pool);

GVirConfigStoragePool *gvir_storage_pool_get_config(GVirStoragePool *pool,
                                                    guint flags,
                                                    GError **err);

GVirStoragePoolInfo *gvir_storage_pool_get_info(GVirStoragePool *pool,
                                                GError **err);

gboolean gvir_storage_pool_refresh(GVirStoragePool *pool,
                                   GCancellable *cancellable,
                                   GError **err);
void gvir_storage_pool_refresh_async(GVirStoragePool *pool,
                                     GCancellable *cancellable,
                                     GAsyncReadyCallback callback,
                                     gpointer user_data);
gboolean gvir_storage_pool_refresh_finish(GVirStoragePool *pool,
                                          GAsyncResult *result,
                                          GError **err);

GList *gvir_storage_pool_get_volumes(GVirStoragePool *pool);
GVirStorageVol *gvir_storage_pool_get_volume(GVirStoragePool *pool,
                                             const gchar *name);
GVirStorageVol *gvir_storage_pool_create_volume
                                (GVirStoragePool *pool,
                                 GVirConfigStorageVol *conf,
                                 GError **err);

gboolean gvir_storage_pool_build (GVirStoragePool *pool,
                                  guint flags,
                                  GError **err);
void gvir_storage_pool_build_async (GVirStoragePool *pool,
                                    guint flags,
                                    GCancellable *cancellable,
                                    GAsyncReadyCallback callback,
                                    gpointer user_data);
gboolean gvir_storage_pool_build_finish(GVirStoragePool *pool,
                                        GAsyncResult *result,
                                        GError **err);

gboolean gvir_storage_pool_undefine (GVirStoragePool *pool,
                                     GError **err);
void gvir_storage_pool_undefine_async (GVirStoragePool *pool,
                                       GCancellable *cancellable,
                                       GAsyncReadyCallback callback,
                                       gpointer user_data);
gboolean gvir_storage_pool_undefine_finish(GVirStoragePool *pool,
                                           GAsyncResult *result,
                                           GError **err);

gboolean gvir_storage_pool_start (GVirStoragePool *pool,
                                  guint flags,
                                  GError **err);
void gvir_storage_pool_start_async (GVirStoragePool *pool,
                                    guint flags,
                                    GCancellable *cancellable,
                                    GAsyncReadyCallback callback,
                                    gpointer user_data);
gboolean gvir_storage_pool_start_finish(GVirStoragePool *pool,
                                        GAsyncResult *result,
                                        GError **err);

gboolean gvir_storage_pool_stop (GVirStoragePool *pool,
                                 GError **err);
void gvir_storage_pool_stop_async (GVirStoragePool *pool,
                                   GCancellable *cancellable,
                                   GAsyncReadyCallback callback,
                                   gpointer user_data);
gboolean gvir_storage_pool_stop_finish(GVirStoragePool *pool,
                                       GAsyncResult *result,
                                       GError **err);

gboolean gvir_storage_pool_delete (GVirStoragePool *pool,
                                   guint flags,
                                   GError **err);
void gvir_storage_pool_delete_async (GVirStoragePool *pool,
                                     guint flags,
                                     GCancellable *cancellable,
                                     GAsyncReadyCallback callback,
                                     gpointer user_data);
gboolean gvir_storage_pool_delete_finish(GVirStoragePool *pool,
                                         GAsyncResult *result,
                                         GError **err);
gboolean gvir_storage_pool_get_autostart(GVirStoragePool *pool,
                                         GError **err);
gboolean gvir_storage_pool_set_autostart(GVirStoragePool *pool,
                                         gboolean autostart,
                                         GError **err);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_STORAGE_POOL_H__ */
