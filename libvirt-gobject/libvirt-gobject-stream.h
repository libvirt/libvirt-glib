/*
 * libvirt-gobject-stream.h: libvirt gobject integration
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
 * Authors: Daniel P. Berrange <berrange@redhat.com>
 *          Marc-André Lureau <marcandre.lureau@redhat.com>
 */

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#ifndef __LIBVIRT_GOBJECT_STREAM_H__
#define __LIBVIRT_GOBJECT_STREAM_H__

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define GVIR_TYPE_STREAM            (gvir_stream_get_type ())
#define GVIR_STREAM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_STREAM, GVirStream))
#define GVIR_STREAM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_STREAM, GVirStreamClass))
#define GVIR_IS_STREAM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_STREAM))
#define GVIR_IS_STREAM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_STREAM))
#define GVIR_STREAM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_STREAM, GVirStreamClass))

#define GVIR_TYPE_STREAM_HANDLE     (gvir_stream_handle_get_type())

typedef struct _GVirStream GVirStream;
typedef struct _GVirStreamPrivate GVirStreamPrivate;
typedef struct _GVirStreamClass GVirStreamClass;

struct _GVirStream
{
    GIOStream parent_instance;

    GVirStreamPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirStreamClass
{
    GIOStreamClass parent_class;

    gpointer padding[20];
};

/**
 * GVirStreamSinkFunc:
 * @stream: a #GVirStream
 * @buf: (out) (array length=nbytes) (transfer none): data pointer
 * @nbytes: data size
 * @user_data: user data passed to the function
 *
 * Returns: the number of bytes filled, 0 upon end
 * of file, or -1 upon error
 */
typedef gint (* GVirStreamSinkFunc)(GVirStream *stream,
                                    const gchar *buf,
                                    gsize nbytes,
                                    gpointer user_data);

/**
 * GVirStreamSourceFunc:
 * @stream: a #GVirStream
 * @buf: (out) (array length=nbytes) (transfer none): data pointer
 * @nbytes: data size
 * @user_data: user data passed to the function
 *
 * Returns: the number of bytes filled, 0 upon end
 * of file, or -1 upon error
 */
typedef gint (* GVirStreamSourceFunc)(GVirStream *stream,
                                      gchar *buf,
                                      gsize nbytes,
                                      gpointer user_data);

GType gvir_stream_get_type(void);
GType gvir_stream_handle_get_type(void);

typedef enum {
    GVIR_STREAM_IO_CONDITION_READABLE = (1 << 0),
    GVIR_STREAM_IO_CONDITION_WRITABLE = (1 << 1),
    GVIR_STREAM_IO_CONDITION_HANGUP   = (1 << 2),
    GVIR_STREAM_IO_CONDITION_ERROR    = (1 << 3),
} GVirStreamIOCondition;

typedef gboolean (*GVirStreamIOFunc)(GVirStream *stream,
                                     GVirStreamIOCondition cond,
                                     gpointer opaque);

guint gvir_stream_add_watch(GVirStream *stream,
                            GVirStreamIOCondition cond,
                            GVirStreamIOFunc func,
                            gpointer opaque);
guint gvir_stream_add_watch_full(GVirStream *stream,
                                 gint priority,
                                 GVirStreamIOCondition cond,
                                 GVirStreamIOFunc func,
                                 gpointer opaque,
                                 GDestroyNotify notify);

gssize gvir_stream_receive_all(GVirStream *stream,
                               GCancellable *cancellable,
                               GVirStreamSinkFunc func,
                               gpointer user_data,
                               GError **error);
gssize gvir_stream_receive(GVirStream *stream,
                           gchar *buffer,
                           gsize size,
                           GCancellable *cancellable,
                           GError **error);

gssize gvir_stream_send_all(GVirStream *stream,
                            GCancellable *cancellable,
                            GVirStreamSourceFunc func,
                            gpointer user_data,
                            GError **error);
gssize gvir_stream_send(GVirStream *stream,
                        const gchar *buffer,
                        gsize size,
                        GCancellable *cancellable,
                        GError **error);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_STREAM_H__ */
