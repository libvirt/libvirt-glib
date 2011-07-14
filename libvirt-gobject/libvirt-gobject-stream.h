/*
 * libvirt-gobject-stream.h: libvirt gobject integration
 *
 * Copyright (C) 2011 Red Hat
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
 * Authors: Daniel P. Berrange <berrange@redhat.com>
 *          Marc-André Lureau <marcandre.lureau@redhat.com>
 */

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#ifndef __LIBVIRT_GOBJECT_STREAM_H__
#define __LIBVIRT_GOBJECT_STREAM_H__

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
    GObject parent;

    GVirStreamPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirStreamClass
{
    GObjectClass parent_class;

    gpointer padding[20];
};

/**
 * GVirStreamSinkFunc:
 * @stream: a #GVirStream
 * @buf: data pointer
 * @nbytes: data size
 * @user_data: user data passed to the function
 */
typedef gint (* GVirStreamSinkFunc) (GVirStream *stream,
                                     const gchar *buf,
                                     gsize nbytes,
                                     gpointer user_data);

GType gvir_stream_get_type(void);
GType gvir_stream_handle_get_type(void);

gint gvir_stream_receive_all(GVirStream *stream, GVirStreamSinkFunc func, gpointer user_data, GError **err);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_STREAM_H__ */
