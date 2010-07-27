/*
 * libvirt-gobject-connection.c: libvirt glib integration
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

#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-gobject/libvirt-gobject-connection.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_CONNECTION_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONNECTION, GVirConnectionPrivate))

struct _GVirConnectionPrivate
{
    gchar *uri;
    virConnectPtr conn;

    GHashTable *domains;
};

G_DEFINE_TYPE(GVirConnection, gvir_connection, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_URI,
};


#define GVIR_CONNECTION_ERROR gvir_connection_error_quark()

static GQuark
gvir_connection_error_quark(void)
{
    return g_quark_from_static_string("vir-g-connection");
}

static void gvir_connection_get_property(GObject *object,
                                         guint prop_id,
                                         GValue *value,
                                         GParamSpec *pspec)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GVirConnectionPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_URI:
        g_value_set_string(value, priv->uri);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_connection_set_property(GObject *object,
                                         guint prop_id,
                                         const GValue *value,
                                         GParamSpec *pspec)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GVirConnectionPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_URI:
        g_free(priv->uri);
        priv->uri = g_value_dup_string(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_connection_finalize(GObject *object)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GVirConnectionPrivate *priv = conn->priv;

    DEBUG("Finalize GVirConnection=%p", conn);

    if (gvir_connection_is_open(conn))
        gvir_connection_close(conn);

    g_free(priv->uri);

    G_OBJECT_CLASS(gvir_connection_parent_class)->finalize(object);
}


static void gvir_connection_class_init(GVirConnectionClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_connection_finalize;
    object_class->get_property = gvir_connection_get_property;
    object_class->set_property = gvir_connection_set_property;

    g_object_class_install_property(object_class,
                                    PROP_URI,
                                    g_param_spec_string("uri",
                                                        "URI",
                                                        "The connection URI",
                                                        NULL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_NAME |
                                                        G_PARAM_STATIC_NICK |
                                                        G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirConnectionPrivate));
}


static void gvir_connection_init(GVirConnection *conn)
{
    GVirConnectionPrivate *priv;

    DEBUG("Init GVirConnection=%p", conn);

    priv = conn->priv = GVIR_CONNECTION_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}


GVirConnection *gvir_connection_new(const char *uri)
{
    return GVIR_CONNECTION(g_object_new(GVIR_TYPE_CONNECTION,
                                         "uri", uri,
                                         NULL));
}


static void
gvir_connection_open_helper(GSimpleAsyncResult *res,
                            GObject *object,
                            GCancellable *cancellable)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GVirConnectionPrivate *priv = conn->priv;
    GError *err = NULL;

    if (priv->conn) {
        err = g_error_new(GVIR_CONNECTION_ERROR,
                          0,
                          "Connection %s is already open",
                          priv->uri);
        g_simple_async_result_set_from_error(res, err);
        g_error_free(err);
        return;
    }

    if (g_cancellable_set_error_if_cancelled(cancellable, &err)) {
        g_simple_async_result_set_from_error(res, err);
        g_error_free(err);
        return;
    }

    if (!(priv->conn = virConnectOpen(priv->uri))) {
        virErrorPtr verr = virGetLastError();
        err = g_error_new(GVIR_CONNECTION_ERROR,
                          0,
                          "Unable to open %s: %s",
                          priv->uri, verr->message);
        g_simple_async_result_set_from_error(res, err);
        g_error_free(err);
    }
}

gboolean gvir_connection_open(GVirConnection *conn,
                              GError **err)
{
    GVirConnectionPrivate *priv = conn->priv;

    if (priv->conn) {
        *err = g_error_new(GVIR_CONNECTION_ERROR,
                           0,
                           "Connection %s is already open",
                           priv->uri);
        return FALSE;
    }

    if (!(priv->conn = virConnectOpen(priv->uri))) {
        virErrorPtr verr = virGetLastError();
        *err = g_error_new(GVIR_CONNECTION_ERROR,
                           0,
                           "Unable to open %s: %s",
                           priv->uri, verr->message);
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_connection_open_async:
 * @cancellable: (allow-none): operation cancellation
 */
void gvir_connection_open_async(GVirConnection *conn,
                                GCancellable *cancellable,
                                GAsyncReadyCallback callback,
                                gpointer opaque)
{
    GSimpleAsyncResult *res;

    res = g_simple_async_result_new(G_OBJECT(conn),
                                    callback,
                                    opaque,
                                    gvir_connection_open);
    g_simple_async_result_run_in_thread(res,
                                        gvir_connection_open_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
}


gboolean gvir_connection_open_finish(GVirConnection *conn,
                                     GAsyncResult *result,
                                     GError **err)
{
    g_return_val_if_fail(GVIR_IS_CONNECTION(conn), FALSE);
    g_return_val_if_fail(G_IS_ASYNC_RESULT(result), FALSE);

    if (G_IS_SIMPLE_ASYNC_RESULT(result)) {
        GSimpleAsyncResult *simple = G_SIMPLE_ASYNC_RESULT(result);
        g_warn_if_fail (g_simple_async_result_get_source_tag(simple) == gvir_connection_open);
        if (g_simple_async_result_propagate_error(simple, err))
            return FALSE;
    }

    return TRUE;
}


gboolean gvir_connection_is_open(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;

    return priv->conn == NULL ? FALSE : TRUE;
}

void gvir_connection_close(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;
    DEBUG("Close GVirConnection=%p", conn);

    if (!priv->conn)
        return;

    virConnectClose(priv->conn);
    priv->conn = NULL;
    /* xxx signals */
}

