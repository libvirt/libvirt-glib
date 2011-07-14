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

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_STREAM_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_STREAM, GVirStreamPrivate))

struct _GVirStreamPrivate
{
    virStreamPtr handle;
};

G_DEFINE_TYPE(GVirStream, gvir_stream, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_STREAM_ERROR gvir_stream_error_quark()


static GQuark
gvir_stream_error_quark(void)
{
    return g_quark_from_static_string("vir-g-stream");
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
        priv->handle = g_value_dup_boxed(value);
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

    if (priv->handle) {
        if (virStreamFinish(priv->handle) < 0)
            g_critical("cannot finish stream");

        virStreamFree(priv->handle);
    }

    G_OBJECT_CLASS(gvir_stream_parent_class)->finalize(object);
}


static void gvir_stream_class_init(GVirStreamClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_stream_finalize;
    object_class->get_property = gvir_stream_get_property;
    object_class->set_property = gvir_stream_set_property;

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


static gpointer
gvir_stream_handle_copy(gpointer src)
{
    virStreamRef(src);
    return src;
}


GType gvir_stream_handle_get_type(void)
{
    static GType handle_type = 0;

    if (G_UNLIKELY(handle_type == 0))
        handle_type = g_boxed_type_register_static
            ("GVirStreamHandle",
             gvir_stream_handle_copy,
             (GBoxedFreeFunc)virStreamFree);

    return handle_type;
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
 *
 * Receive the entire data stream, sending the data to the
 * requested data sink. This is simply a convenient alternative
 * to virStreamRecv, for apps that do blocking-I/o.
 */
gint
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
