/*
 * libvirt-gobject-stream.c: libvirt glib integration
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
 * Authors: Daniel P. Berrange <berrange@redhat.com>
 *          Marc-André Lureau <marcandre.lureau@redhat.com>
 */

#include <config.h>

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-compat.h"

#include "libvirt-gobject/libvirt-gobject-input-stream.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_STREAM_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_STREAM, GVirStreamPrivate))

struct _GVirStreamPrivate
{
    virStreamPtr   handle;
    GInputStream  *input_stream;
    gboolean       in_dispose;
};

G_DEFINE_TYPE(GVirStream, gvir_stream, G_TYPE_IO_STREAM);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_STREAM_ERROR gvir_stream_error_quark()


static GQuark
gvir_stream_error_quark(void)
{
    return g_quark_from_static_string("gvir-stream");
}


static GInputStream* gvir_stream_get_input_stream(GIOStream *io_stream)
{
    GVirStream *self = GVIR_STREAM(io_stream);

    if (self->priv->input_stream == NULL)
        self->priv->input_stream = (GInputStream *)_gvir_input_stream_new(self);

    return self->priv->input_stream;
}


static gboolean gvir_stream_close(GIOStream *io_stream,
                                  GCancellable *cancellable, G_GNUC_UNUSED GError **error)
{
    GVirStream *self = GVIR_STREAM(io_stream);

    if (self->priv->input_stream)
        g_input_stream_close(self->priv->input_stream, cancellable, NULL);

    if (self->priv->in_dispose)
        return TRUE;

    return TRUE; /* FIXME: really close the stream? */
}


static void gvir_stream_close_async(GIOStream *stream, G_GNUC_UNUSED int io_priority,
                                    GCancellable *cancellable, GAsyncReadyCallback callback,
                                    gpointer user_data)
{
    GSimpleAsyncResult *res;
    GIOStreamClass *class;
    GError *error;

    class = G_IO_STREAM_GET_CLASS(stream);

    /* close is not blocked, just do it! */
    error = NULL;
    if (class->close_fn &&
        !class->close_fn(stream, cancellable, &error)) {
        g_simple_async_report_take_gerror_in_idle(G_OBJECT (stream),
                                                  callback, user_data,
                                                  error);
        return;
    }

    res = g_simple_async_result_new(G_OBJECT (stream),
                                    callback,
                                    user_data,
                                    gvir_stream_close_async);
    g_simple_async_result_complete_in_idle(res);
    g_object_unref (res);
}


static gboolean
gvir_stream_close_finish(G_GNUC_UNUSED GIOStream *stream,
                         G_GNUC_UNUSED GAsyncResult *result,
                         G_GNUC_UNUSED GError **error)
{
    return TRUE;
}


static void gvir_stream_get_property(GObject *object,
                                     guint prop_id,
                                     GValue *value,
                                     GParamSpec *pspec)
{
    GVirStream *self = GVIR_STREAM(object);
    GVirStreamPrivate *priv = self->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_stream_set_property(GObject *object,
                                     guint prop_id,
                                     const GValue *value,
                                     GParamSpec *pspec)
{
    GVirStream *self = GVIR_STREAM(object);
    GVirStreamPrivate *priv = self->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virStreamFree(priv->handle);
        priv->handle = g_value_get_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_stream_finalize(GObject *object)
{
    GVirStream *self = GVIR_STREAM(object);
    GVirStreamPrivate *priv = self->priv;

    DEBUG("Finalize GVirStream=%p", self);

    if (self->priv->input_stream)
        g_object_unref(self->priv->input_stream);

    if (priv->handle) {
        if (virStreamFinish(priv->handle) < 0)
            g_critical("cannot finish stream");

        virStreamFree(priv->handle);
    }

    G_OBJECT_CLASS(gvir_stream_parent_class)->finalize(object);
}


static void gvir_stream_class_init(GVirStreamClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    GIOStreamClass *stream_class = G_IO_STREAM_CLASS(klass);

    object_class->finalize = gvir_stream_finalize;
    object_class->get_property = gvir_stream_get_property;
    object_class->set_property = gvir_stream_set_property;

    stream_class->get_input_stream = gvir_stream_get_input_stream;
    stream_class->close_fn = gvir_stream_close;
    stream_class->close_async = gvir_stream_close_async;
    stream_class->close_finish = gvir_stream_close_finish;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The stream handle",
                                                       GVIR_TYPE_STREAM_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_NAME |
                                                       G_PARAM_STATIC_NICK |
                                                       G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirStreamPrivate));
}


static void gvir_stream_init(GVirStream *self)
{
    self->priv = GVIR_STREAM_GET_PRIVATE(self);
}

typedef struct virStream GVirStreamHandle;

static GVirStreamHandle*
gvir_stream_handle_copy(GVirStreamHandle *src)
{
    virStreamRef((virStreamPtr)src);
    return src;
}

static void
gvir_stream_handle_free(GVirStreamHandle *src)
{
    virStreamFree((virStreamPtr)src);
}

G_DEFINE_BOXED_TYPE(GVirStreamHandle, gvir_stream_handle,
                    gvir_stream_handle_copy, gvir_stream_handle_free)

/**
 * gvir_stream_receive:
 * @stream: the stream
 * @buffer: a buffer to read data into (which should be at least @size
 *     bytes long).
 * @size: the number of bytes you want to read from the stream
 * @cancellable: (allow-none): a %GCancellable or %NULL
 * @error: #GError for error reporting, or %NULL to ignore.
 *
 * Receive data (up to @size bytes) from a stream.
 * On error -1 is returned and @error is set accordingly.
 *
 * gvir_stream_receive() can return any number of bytes, up to
 * @size. If more than @size bytes have been received, the additional
 * data will be returned in future calls to gvir_stream_receive().
 *
 * If there is no data available, a %G_IO_ERROR_WOULD_BLOCK error will be
 * returned.
 *
 * Returns: Number of bytes read, or 0 if the end of stream reached,
 * or -1 on error.
 */
gssize gvir_stream_receive(GVirStream *self, gchar *buffer, gsize size,
                           GCancellable *cancellable, GError **error)
{
    int got;

    g_return_val_if_fail(GVIR_IS_STREAM(self), -1);
    g_return_val_if_fail(buffer != NULL, -1);

    if (g_cancellable_set_error_if_cancelled (cancellable, error))
        return -1;

    got = virStreamRecv(self->priv->handle, buffer, size);

    if (got == -2) {  /* blocking */
        g_set_error(error, G_IO_ERROR, G_IO_ERROR_WOULD_BLOCK, NULL);
    } else if (got < 0) {
        g_set_error(error, G_IO_ERROR, G_IO_ERROR_INVALID_ARGUMENT,
                    "Got virStreamRecv error in %s", G_STRFUNC);
    }

    return got;
}

struct stream_sink_helper {
    GVirStream *self;
    GVirStreamSinkFunc func;
    gpointer user_data;
};

static int
stream_sink(virStreamPtr st G_GNUC_UNUSED,
            const char *bytes, size_t nbytes, void *opaque)
{
  struct stream_sink_helper *helper = opaque;

  return helper->func(helper->self, bytes, nbytes, helper->user_data);
}

/**
 * gvir_stream_receive_all:
 * @stream: the stream
 * @func: (scope notified): the callback for writing data to application
 * @user_data: (closure): data to be passed to @callback
 * Returns: the number of bytes consumed or -1 upon error
 *
 * Receive the entire data stream, sending the data to the
 * requested data sink. This is simply a convenient alternative
 * to virStreamRecv, for apps that do blocking-I/o.
 */
gssize
gvir_stream_receive_all(GVirStream *self, GVirStreamSinkFunc func, gpointer user_data, GError **err)
{
    struct stream_sink_helper helper = {
        .self = self,
        .func = func,
        .user_data = user_data
    };
    int r;

    g_return_val_if_fail(GVIR_IS_STREAM(self), -1);
    g_return_val_if_fail(func != NULL, -1);

    r = virStreamRecvAll(self->priv->handle, stream_sink, &helper);
    if (r < 0) {
        if (err != NULL)
            *err = gvir_error_new_literal(GVIR_STREAM_ERROR,
                                          0,
                                          "Unable to perform RecvAll");
    }

    return r;
}
