/*
 * libvirt-gobject-output-stream.h: libvirt gobject integration
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

#include <config.h>

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-output-stream.h"
#include "libvirt-gobject-compat.h"

#define gvir_output_stream_get_type _gvir_output_stream_get_type
G_DEFINE_TYPE(GVirOutputStream, gvir_output_stream, G_TYPE_OUTPUT_STREAM);

enum
{
    PROP_0,
    PROP_STREAM
};

struct _GVirOutputStreamPrivate
{
    GVirStream *stream;

    /* pending operation metadata */
    GSimpleAsyncResult *result;
    GCancellable *cancellable;
    const void * buffer;
    gsize count;
};

static void gvir_output_stream_get_property(GObject *object,
                                            guint prop_id,
                                            GValue *value,
                                            GParamSpec *pspec)
{
    GVirOutputStream *stream = GVIR_OUTPUT_STREAM(object);

    switch (prop_id) {
    case PROP_STREAM:
        g_value_set_object(value, stream->priv->stream);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void gvir_output_stream_set_property(GObject *object,
                                            guint prop_id,
                                            const GValue *value,
                                            GParamSpec *pspec)
{
    GVirOutputStream *stream = GVIR_OUTPUT_STREAM(object);

    switch (prop_id) {
    case PROP_STREAM:
        stream->priv->stream = g_value_get_object(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void gvir_output_stream_finalize(GObject *object)
{
    GVirOutputStream *stream = GVIR_OUTPUT_STREAM(object);

    g_debug("Finalize output stream GVirStream=%p", stream->priv->stream);
    stream->priv->stream = NULL; // unowned

    if (G_OBJECT_CLASS(gvir_output_stream_parent_class)->finalize)
        (*G_OBJECT_CLASS(gvir_output_stream_parent_class)->finalize)(object);
}

static gboolean
gvir_output_stream_write_ready(GVirStream *stream,
                               GVirStreamIOCondition cond,
                               void *opaque)
{
    GVirOutputStream *output_stream = GVIR_OUTPUT_STREAM(opaque);
    GVirOutputStreamPrivate *priv = output_stream->priv;
    GSimpleAsyncResult *simple = priv->result;
    GError *error = NULL;
    gssize result;

    if (!(cond & GVIR_STREAM_IO_CONDITION_WRITABLE)) {
        g_warn_if_reached();
        g_simple_async_result_set_error(simple,
                                        G_IO_ERROR,
                                        G_IO_ERROR_INVALID_ARGUMENT,
                                        "%s",
                                        "Expected stream to be writable");
        goto cleanup;
    }

    result  = gvir_stream_send(stream, priv->buffer, priv->count,
                               priv->cancellable, &error);

    if (g_error_matches(error, G_IO_ERROR, G_IO_ERROR_WOULD_BLOCK)) {
        g_warn_if_reached();
        g_simple_async_result_set_error(simple,
                                        G_IO_ERROR,
                                        G_IO_ERROR_INVALID_ARGUMENT,
                                        "%s",
                                        "Expected stream to be writable");
        goto cleanup;
    }

    if (result >= 0)
        g_simple_async_result_set_op_res_gssize(simple, result);

    if (error)
        g_simple_async_result_take_error(simple, error);

    if (priv->cancellable) {
        g_object_unref(priv->cancellable);
        priv->cancellable = NULL;
    }

cleanup:
    priv->result = NULL;
    g_simple_async_result_complete(simple);
    g_object_unref(simple);
    return FALSE;
}

static void gvir_output_stream_write_async(GOutputStream *stream,
                                           const void *buffer,
                                           gsize count,
                                           int io_priority G_GNUC_UNUSED,
                                           GCancellable *cancellable,
                                           GAsyncReadyCallback callback,
                                           gpointer user_data)
{
    GVirOutputStream *output_stream = GVIR_OUTPUT_STREAM(stream);

    g_return_if_fail(GVIR_IS_OUTPUT_STREAM(stream));
    g_return_if_fail(output_stream->priv->result == NULL);

    gvir_stream_add_watch_full(output_stream->priv->stream,
                               G_PRIORITY_DEFAULT,
                               GVIR_STREAM_IO_CONDITION_WRITABLE,
                               gvir_output_stream_write_ready,
                               g_object_ref(stream),
                               (GDestroyNotify)g_object_unref);

    output_stream->priv->result =
        g_simple_async_result_new(G_OBJECT(stream), callback, user_data,
                                  gvir_output_stream_write_async);
    if (cancellable)
        g_object_ref(cancellable);
    output_stream->priv->cancellable = cancellable;
    output_stream->priv->buffer = buffer;
    output_stream->priv->count = count;
}


static gssize gvir_output_stream_write_finish(GOutputStream *stream,
                                              GAsyncResult *result,
                                              GError **error G_GNUC_UNUSED)
{
    GVirOutputStream *output_stream = GVIR_OUTPUT_STREAM(stream);
    GSimpleAsyncResult *simple;
    virStreamPtr handle;
    gssize count;

    g_return_val_if_fail(GVIR_IS_OUTPUT_STREAM(stream), -1);
    g_return_val_if_fail(g_simple_async_result_is_valid(result, G_OBJECT(stream),
                                                        gvir_output_stream_write_async),
                         -1);
    g_object_get(output_stream->priv->stream, "handle", &handle, NULL);

    simple = G_SIMPLE_ASYNC_RESULT(result);

    count = g_simple_async_result_get_op_res_gssize(simple);

    virStreamFree(handle);

    return count;
}


static void gvir_output_stream_class_init(GVirOutputStreamClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GOutputStreamClass *goutputstream_class = G_OUTPUT_STREAM_CLASS(klass);

    g_type_class_add_private(klass, sizeof(GVirOutputStreamPrivate));

    gobject_class->finalize = gvir_output_stream_finalize;
    gobject_class->get_property = gvir_output_stream_get_property;
    gobject_class->set_property = gvir_output_stream_set_property;

    goutputstream_class->write_fn = NULL;
    goutputstream_class->write_async = gvir_output_stream_write_async;
    goutputstream_class->write_finish = gvir_output_stream_write_finish;

    g_object_class_install_property(gobject_class, PROP_STREAM,
                                    g_param_spec_object("stream",
                                                        "stream",
                                                        "GVirStream",
                                                        GVIR_TYPE_STREAM, G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}

static void gvir_output_stream_init(GVirOutputStream *stream)
{
    stream->priv = G_TYPE_INSTANCE_GET_PRIVATE(stream, GVIR_TYPE_OUTPUT_STREAM, GVirOutputStreamPrivate);
}

GVirOutputStream* _gvir_output_stream_new(GVirStream *stream)
{
    return GVIR_OUTPUT_STREAM(g_object_new(GVIR_TYPE_OUTPUT_STREAM, "stream", stream, NULL));
}
