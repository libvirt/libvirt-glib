/*
 * libvirt-gobject-storage_vol.c: libvirt gobject integration
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
#include <libvirt/libvirt.h>

#ifndef __LIBVIRT_GOBJECT_STORAGE_VOL_H__
#define __LIBVIRT_GOBJECT_STORAGE_VOL_H__

G_BEGIN_DECLS

#define GVIR_TYPE_STORAGE_VOL            (gvir_storage_vol_get_type ())
#define GVIR_STORAGE_VOL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_STORAGE_VOL, GVirStorageVol))
#define GVIR_STORAGE_VOL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_STORAGE_VOL, GVirStorageVolClass))
#define GVIR_IS_STORAGE_VOL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_STORAGE_VOL))
#define GVIR_IS_STORAGE_VOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_STORAGE_VOL))
#define GVIR_STORAGE_VOL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_STORAGE_VOL, GVirStorageVolClass))

#define GVIR_TYPE_STORAGE_VOL_INFO       (gvir_storage_vol_info_get_type())
#define GVIR_TYPE_STORAGE_VOL_HANDLE     (gvir_storage_vol_handle_get_type())

typedef struct _GVirStorageVol GVirStorageVol;
typedef struct _GVirStorageVolPrivate GVirStorageVolPrivate;
typedef struct _GVirStorageVolClass GVirStorageVolClass;

struct _GVirStorageVol
{
    GObject parent;

    GVirStorageVolPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirStorageVolClass
{
    GObjectClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_STORAGE_VOL_STATE_FILE  = 0, /* Regular file based volume */
    GVIR_STORAGE_VOL_STATE_BLOCK = 1, /* Block based volume */
    GVIR_STORAGE_VOL_STATE_DIR   = 2, /* Directory-passthrough based volume */
} GVirStorageVolType;

/**
 * GVirStorageVolResizeFlags:
 * @GVIR_STORAGE_VOL_RESIZE_NONE: No flags
 * @GVIR_STORAGE_VOL_RESIZE_ALLOCATE: force allocation of new size
 * @GVIR_STORAGE_VOL_RESIZE_DELTA: size is relative to current
 * @GVIR_STORAGE_VOL_RESIZE_SHRINK: allow decrease in capacity. This combined
 * with #GVIR_STORAGE_VOL_RESIZE_DELTA, implies a negative delta.
 */
typedef enum {
    GVIR_STORAGE_VOL_RESIZE_NONE     = 0,
    GVIR_STORAGE_VOL_RESIZE_ALLOCATE = VIR_STORAGE_VOL_RESIZE_ALLOCATE,
    GVIR_STORAGE_VOL_RESIZE_DELTA    = VIR_STORAGE_VOL_RESIZE_DELTA,
    GVIR_STORAGE_VOL_RESIZE_SHRINK   = VIR_STORAGE_VOL_RESIZE_SHRINK,
} GVirStorageVolResizeFlags;

typedef struct _GVirStorageVolInfo GVirStorageVolInfo;
struct _GVirStorageVolInfo
{
    GVirStorageVolType type; /* Type flags */
    guint64 capacity;        /* Logical size bytes */
    guint64 allocation;      /* Current allocation bytes */
};

GType gvir_storage_vol_get_type(void);
GType gvir_storage_vol_info_get_type(void);
GType gvir_storage_vol_handle_get_type(void);

const gchar *gvir_storage_vol_get_name(GVirStorageVol *vol);
const gchar *gvir_storage_vol_get_path(GVirStorageVol *vol, GError **error);

gboolean gvir_storage_vol_delete(GVirStorageVol *vol,
                                 guint flags,
                                 GError **err);

GVirConfigStorageVol *gvir_storage_vol_get_config(GVirStorageVol *vol,
                                                  guint flags,
                                                  GError **err);
GVirStorageVolInfo *gvir_storage_vol_get_info(GVirStorageVol *vol,
                                              GError **err);
gboolean gvir_storage_vol_resize(GVirStorageVol *vol,
                                 guint64 capacity,
                                 guint flags,
                                 GError **err);

gboolean gvir_storage_vol_download(GVirStorageVol *vol,
                                   GVirStream *stream,
                                   guint64 offset,
                                   guint64 length,
                                   guint flags,
                                   GError **err);

gboolean gvir_storage_vol_upload(GVirStorageVol *vol,
                                 GVirStream *stream,
                                 guint64 offset,
                                 guint64 length,
                                 guint flags,
                                 GError **err);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_STORAGE_VOL_H__ */
