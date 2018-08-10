/*
 * libvirt-gobject-stream.c: libvirt glib integration
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
 * Authors: Daniel P. Berrange <berrange@redhat.com>
 *          Marc-André Lureau <marcandre.lureau@redhat.com>
 */

#include <config.h>

#include <libvirt/virterror.h>
#include <string.h>

#include <glib/gi18n-lib.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-compat.h"

#include "libvirt-gobject/libvirt-gobject-input-stream.h"
#include "libvirt-gobject/libvirt-gobject-output-stream.h"

#define GVIR_STREAM_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_STREAM, GVirStreamPrivate))

struct _GVirStreamPrivate
{
    virStreamPtr   handle;
    GInputStream  *input_stream;
    GOutputStream  *output_stream;

    gboolean eventRegistered;
    int eventLast;
    GList *sources;
};

typedef struct {
    GSource source;
    GVirStreamIOCondition cond;
    GVirStreamIOCondition newCond;
    GVirStream *stream;
} GVirStreamSource;


G_DEFINE_TYPE_WITH_PRIVATE(GVirStream, gvir_stream, G_TYPE_IO_STREAM);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_STREAM_ERROR gvir_stream_error_quark()

static void gvir_stream_update_events(GVirStream *stream);

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


static GOutputStream* gvir_stream_get_output_stream(GIOStream *io_stream)
{
    GVirStream *self = GVIR_STREAM(io_stream);

    if (self->priv->output_stream == NULL)
        self->priv->output_stream = (GOutputStream *)_gvir_output_stream_new(self);

    return self->priv->output_stream;
}


static gboolean gvir_stream_close(GIOStream *io_stream,
                                  GCancellable *cancellable,
                                  GError **error)
{
    GVirStream *self = GVIR_STREAM(io_stream);
    GError *local_error = NULL;
    gboolean i_ret = TRUE, o_ret = TRUE;

    g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

    if (self->priv->input_stream)
        i_ret = g_input_stream_close(self->priv->input_stream, cancellable, &local_error);

    if (local_error)
        g_propagate_error(error, local_error);

    if (self->priv->output_stream)
        o_ret = g_output_stream_close(self->priv->output_stream, cancellable, &local_error);

    if (local_error) {
        if (i_ret)
            g_propagate_error(error, local_error);
        else
            g_error_free(local_error);
    }

    return (i_ret && o_ret);
}

static gboolean close_in_idle (gpointer data)
{
    GTask *task = G_TASK (data);
    GIOStream *stream = G_IO_STREAM(g_task_get_source_object (task));
    GCancellable *cancellable = g_task_get_cancellable (task);
    GIOStreamClass *class;
    GError *error;

    class = G_IO_STREAM_GET_CLASS(stream);

    /* close is not blocked, just do it! */
    error = NULL;
    if (class->close_fn &&
        !class->close_fn(stream, cancellable, &error)) {
        g_task_return_error(task, error);

        goto exit;
    }

    g_task_return_boolean(task, TRUE);
exit:
    g_object_unref(task);
    return FALSE;
}

static void gvir_stream_close_async(GIOStream *stream,
                                    int io_priority G_GNUC_UNUSED,
                                    GCancellable *cancellable,
                                    GAsyncReadyCallback callback,
                                    gpointer user_data)
{
    GTask *task;

    task = g_task_new(G_OBJECT(stream),
                      cancellable,
                      callback,
                      user_data);
    g_idle_add(close_in_idle, task);
}

static gboolean
gvir_stream_close_finish(GIOStream *stream,
                         GAsyncResult *result,
                         GError **error)
{
    g_return_val_if_fail(GVIR_IS_STREAM(stream), FALSE);
    g_return_val_if_fail(g_task_is_valid(result, stream), FALSE);
    g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

    return g_task_propagate_boolean(G_TASK(result), error);
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
    GList *tmp;

    if (self->priv->input_stream)
        g_object_unref(self->priv->input_stream);

    tmp = priv->sources;
    while (tmp) {
        GVirStreamSource *source = tmp->data;
        g_source_destroy((GSource*)source);
        tmp = tmp->next;
    }
    g_list_free(priv->sources);
    priv->sources = NULL;

    if (priv->handle) {
        gvir_stream_update_events(self);

        if (virStreamFinish(priv->handle) < 0)
            gvir_critical("cannot finish stream");

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
    stream_class->get_output_stream = gvir_stream_get_output_stream;
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
                                                       G_PARAM_STATIC_STRINGS));
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
 * @buffer: (array length=size) (element-type guint8): a buffer
 *     to read data into (which should be at least @size bytes long).
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
gssize gvir_stream_receive(GVirStream *self,
                           gchar *buffer,
                           gsize size,
                           GCancellable *cancellable,
                           GError **error)
{
    int got;

    g_return_val_if_fail(GVIR_IS_STREAM(self), -1);
    g_return_val_if_fail(buffer != NULL, -1);

    if (g_cancellable_set_error_if_cancelled (cancellable, error))
        return -1;

    got = virStreamRecv(self->priv->handle, buffer, size);

    if (got == -2) {  /* blocking */
        g_set_error_literal(error, G_IO_ERROR, G_IO_ERROR_WOULD_BLOCK,
                            _("virStreamRecv call would block"));
    } else if (got < 0) {
        g_set_error(error, G_IO_ERROR, G_IO_ERROR_INVALID_ARGUMENT,
                    _("Got virStreamRecv error in %s"), G_STRFUNC);
    }

    return got;
}

struct stream_sink_helper {
    GVirStream *self;
    GVirStreamSinkFunc func;
    gpointer user_data;
    GCancellable *cancellable;
};

static int
stream_sink(virStreamPtr st G_GNUC_UNUSED,
            const char *bytes,
            size_t nbytes,
            void *opaque)
{
  struct stream_sink_helper *helper = opaque;

  if (g_cancellable_is_cancelled(helper->cancellable))
      return -1;

  return helper->func(helper->self, bytes, nbytes, helper->user_data);
}

/**
 * gvir_stream_receive_all:
 * @stream: the stream
 * @cancellable: cancellation notifier
 * @func: (scope notified): the callback for writing data to application
 * @user_data: (closure): data to be passed to @callback
 * @error: #GError for error reporting, or %NULL to ignore.
 *
 * Receive the entire data stream, sending the data to the
 * requested data sink. This is simply a convenient alternative
 * to virStreamRecv, for apps that do blocking-I/o.
 *
 * Returns: the number of bytes consumed or -1 upon error
 */
gssize
gvir_stream_receive_all(GVirStream *self,
                        GCancellable *cancellable,
                        GVirStreamSinkFunc func,
                        gpointer user_data,
                        GError **error)
{
    struct stream_sink_helper helper = {
        .self = self,
        .func = func,
        .user_data = user_data,
        .cancellable = cancellable,
    };
    int r;

    g_return_val_if_fail(GVIR_IS_STREAM(self), -1);
    g_return_val_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable), -1);
    g_return_val_if_fail(func != NULL, -1);
    g_return_val_if_fail(error == NULL || *error == NULL, -1);

    r = virStreamRecvAll(self->priv->handle, stream_sink, &helper);
    if (r < 0) {
        gvir_set_error_literal(error, GVIR_STREAM_ERROR,
                               0,
                               _("Unable to perform RecvAll"));
    }

    return r;
}


/**
 * gvir_stream_send:
 * @stream: the stream
 * @buffer: a buffer to write data from (which should be at least @size
 *     bytes long).
 * @size: the number of bytes you want to write to the stream
 * @cancellable: (allow-none): a %GCancellable or %NULL
 * @error: #GError for error reporting, or %NULL to ignore.
 *
 * Send data (up to @size bytes) from a stream.
 * On error -1 is returned and @error is set accordingly.
 *
 * gvir_stream_send() can return any number of bytes, up to
 * @size. If more than @size bytes have been sendd, the additional
 * data will be returned in future calls to gvir_stream_send().
 *
 * If there is no data available, a %G_IO_ERROR_WOULD_BLOCK error will be
 * returned.
 *
 * Returns: Number of bytes written.
 */
gssize gvir_stream_send(GVirStream *self,
                        const gchar *buffer,
                        gsize size,
                        GCancellable *cancellable,
                        GError **error)
{
    int got;

    g_return_val_if_fail(GVIR_IS_STREAM(self), -1);
    g_return_val_if_fail(buffer != NULL, -1);
    g_return_val_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable), -1);
    g_return_val_if_fail(error == NULL || *error == NULL, -1);

    if (g_cancellable_set_error_if_cancelled (cancellable, error))
        return -1;

    got = virStreamSend(self->priv->handle, buffer, size);

    if (got == -2) {  /* blocking */
        g_set_error_literal(error, G_IO_ERROR, G_IO_ERROR_WOULD_BLOCK,
                            _("virStreamSend call would block"));
    } else if (got < 0) {
        g_set_error(error, G_IO_ERROR, G_IO_ERROR_INVALID_ARGUMENT,
                    _("Got virStreamRecv error in %s"), G_STRFUNC);
    }

    return got;
}

struct stream_source_helper {
    GVirStream *self;
    GVirStreamSourceFunc func;
    gpointer user_data;
    GCancellable *cancellable;
};

static int
stream_source(virStreamPtr st G_GNUC_UNUSED,
              char *bytes,
              size_t nbytes,
              void *opaque)
{
  struct stream_source_helper *helper = opaque;

  if (g_cancellable_is_cancelled(helper->cancellable))
      return -1;

  return helper->func(helper->self, bytes, nbytes, helper->user_data);
}

/**
 * gvir_stream_send_all:
 * @stream: the stream
 * @cancellable: cancellation notifier
 * @func: (scope notified): the callback for writing data to application
 * @user_data: (closure): data to be passed to @callback
 * @error: #GError for error reporting, or %NULL to ignore.
 *
 * Send the entire data stream, sending the data to the
 * requested data source. This is simply a convenient alternative
 * to virStreamRecv, for apps that do blocking-I/o.
 *
 * Returns: the number of bytes consumed or -1 upon error
 */
gssize
gvir_stream_send_all(GVirStream *self,
                     GCancellable *cancellable,
                     GVirStreamSourceFunc func,
                     gpointer user_data,
                     GError **error)
{
    struct stream_source_helper helper = {
        .self = self,
        .func = func,
        .user_data = user_data,
        .cancellable = cancellable,
    };
    int r;

    g_return_val_if_fail(GVIR_IS_STREAM(self), -1);
    g_return_val_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable), -1);
    g_return_val_if_fail(func != NULL, -1);
    g_return_val_if_fail(error == NULL || *error == NULL, -1);

    r = virStreamSendAll(self->priv->handle, stream_source, &helper);
    if (r < 0) {
        gvir_set_error_literal(error, GVIR_STREAM_ERROR,
                               0,
                               _("Unable to perform SendAll"));
    }

    return r;
}


static void gvir_stream_handle_events(virStreamPtr st G_GNUC_UNUSED,
                                      int events,
                                      void *opaque)
{
    GVirStream *stream = GVIR_STREAM(opaque);
    GVirStreamPrivate *priv = stream->priv;
    GList *tmp = priv->sources;

    while (tmp) {
        GVirStreamSource *source = tmp->data;
        source->newCond = 0;
        if (source->cond & GVIR_STREAM_IO_CONDITION_READABLE) {
            if (events & VIR_STREAM_EVENT_READABLE)
                source->newCond |= GVIR_STREAM_IO_CONDITION_READABLE;
            if (events & VIR_STREAM_EVENT_HANGUP)
                source->newCond |= GVIR_STREAM_IO_CONDITION_HANGUP;
            if (events & VIR_STREAM_EVENT_ERROR)
                source->newCond |= GVIR_STREAM_IO_CONDITION_ERROR;
        }
        if (source->cond & GVIR_STREAM_IO_CONDITION_WRITABLE) {
            if (events & VIR_STREAM_EVENT_WRITABLE)
                source->newCond |= GVIR_STREAM_IO_CONDITION_WRITABLE;
            if (events & VIR_STREAM_EVENT_HANGUP)
                source->newCond |= GVIR_STREAM_IO_CONDITION_HANGUP;
            if (events & VIR_STREAM_EVENT_ERROR)
                source->newCond |= GVIR_STREAM_IO_CONDITION_ERROR;
        }
        tmp = tmp->next;
    }

}


static void gvir_stream_update_events(GVirStream *stream)
{
    GVirStreamPrivate *priv = stream->priv;
    int mask = 0;
    GList *tmp = priv->sources;

    while (tmp) {
        GVirStreamSource *source = tmp->data;
        if (source->cond & GVIR_STREAM_IO_CONDITION_READABLE)
            mask |= VIR_STREAM_EVENT_READABLE;
        if (source->cond & GVIR_STREAM_IO_CONDITION_WRITABLE)
            mask |= VIR_STREAM_EVENT_WRITABLE;
        tmp = tmp->next;
    }

    if (mask) {
        if (priv->eventRegistered) {
            virStreamEventUpdateCallback(priv->handle, mask);
        } else {
            virStreamEventAddCallback(priv->handle, mask,
                                      gvir_stream_handle_events,
                                      stream,
                                      NULL);
            priv->eventRegistered = TRUE;
        }
    } else {
        if (priv->eventRegistered) {
            virStreamEventRemoveCallback(priv->handle);
            priv->eventRegistered = FALSE;
        }
    }
}

static gboolean gvir_stream_source_prepare(GSource *source,
                                           gint *timeout)
{
    GVirStreamSource *gsource = (GVirStreamSource*)source;
    if (gsource->newCond) {
        *timeout = 0;
        return TRUE;
    }
    *timeout = -1;
    return FALSE;
}

static gboolean gvir_stream_source_check(GSource *source)
{
    GVirStreamSource *gsource = (GVirStreamSource*)source;
    if (gsource->newCond)
        return TRUE;
    return FALSE;
}

static gboolean gvir_stream_source_dispatch(GSource *source,
                                            GSourceFunc callback,
                                            gpointer user_data)
{
    GVirStreamSource *gsource = (GVirStreamSource*)source;
    GVirStreamIOFunc func = (GVirStreamIOFunc)callback;
    gboolean ret;
    ret = func(gsource->stream, gsource->newCond, user_data);
    gsource->newCond = 0;
    return ret;
}

static void gvir_stream_source_finalize(GSource *source)
{
    GVirStreamSource *gsource = (GVirStreamSource*)source;
    GVirStreamPrivate *priv = gsource->stream->priv;

    priv->sources = g_list_remove(priv->sources, source);
    gvir_stream_update_events(gsource->stream);

    g_clear_object(&gsource->stream);
}

GSourceFuncs gvir_stream_source_funcs = {
    .prepare = gvir_stream_source_prepare,
    .check = gvir_stream_source_check,
    .dispatch = gvir_stream_source_dispatch,
    .finalize = gvir_stream_source_finalize,
};


/**
 * gvir_stream_add_watch: (skip)
 * @stream: the stream
 * @cond: the conditions to watch for (bitfield of #GVirStreamIOCondition)
 * @func: (closure opaque): the function to call when the condition is satisfied
 * @opaque: (closure): user data to pass to @func
 *
 * Adds a watch for @stream to the mainloop
 *
 * Returns: the event source id
 */
guint gvir_stream_add_watch(GVirStream *stream,
                            GVirStreamIOCondition cond,
                            GVirStreamIOFunc func,
                            gpointer opaque)
{
    return gvir_stream_add_watch_full(stream,
                                      G_PRIORITY_DEFAULT,
                                      cond,
                                      func,
                                      opaque,
                                      NULL);
}

/**
 * gvir_stream_add_watch_full: (rename-to gvir_stream_add_watch)
 * @stream: the stream
 * @priority: the priority of the #GVirStream source
 * @cond: the conditions to watch for (bitfield of #GVirStreamIOCondition)
 * @func: (closure opaque): the function to call when the condition is satisfied
 * @opaque: (closure): user data to pass to @func
 * @notify: the function to call when the source is removed
 *
 * Adds a watch for @stream to the mainloop
 *
 * Returns: the event source id
 */
guint gvir_stream_add_watch_full(GVirStream *stream,
                                 gint priority,
                                 GVirStreamIOCondition cond,
                                 GVirStreamIOFunc func,
                                 gpointer opaque,
                                 GDestroyNotify notify)
{
    g_return_val_if_fail(GVIR_IS_STREAM(stream), 0);

    GVirStreamPrivate *priv = stream->priv;
    GVirStreamSource *source = (GVirStreamSource*)g_source_new(&gvir_stream_source_funcs,
                                                               sizeof(GVirStreamSource));
    guint ret;

    source->stream = g_object_ref(stream);
    source->cond = cond;

    if (priority != G_PRIORITY_DEFAULT)
        g_source_set_priority((GSource*)source, priority);

    priv->sources = g_list_append(priv->sources, source);

    gvir_stream_update_events(source->stream);

    g_source_set_callback((GSource*)source, (GSourceFunc)func, opaque, notify);
    ret = g_source_attach((GSource*)source, g_main_context_default());

    g_source_unref((GSource*)source);

    return ret;
}
