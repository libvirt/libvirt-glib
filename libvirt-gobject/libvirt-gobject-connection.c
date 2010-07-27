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

#define VIR_G_CONNECTION_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), VIR_G_TYPE_CONNECTION, VirGConnectionPrivate))

struct _VirGConnectionPrivate
{
    gchar *uri;
    virConnectPtr conn;

    GHashTable *domains;
};

G_DEFINE_TYPE(VirGConnection, vir_g_connection, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_URI,
};


#define VIR_G_CONNECTION_ERROR vir_g_connection_error_quark()

static GQuark
vir_g_connection_error_quark(void)
{
    return g_quark_from_static_string("vir-g-connection");
}

static void vir_g_connection_get_property(GObject *object,
                                          guint prop_id,
                                          GValue *value,
                                          GParamSpec *pspec)
{
    VirGConnection *conn = VIR_G_CONNECTION(object);
    VirGConnectionPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_URI:
        g_value_set_string(value, priv->uri);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void vir_g_connection_set_property(GObject *object,
                                          guint prop_id,
                                          const GValue *value,
                                          GParamSpec *pspec)
{
    VirGConnection *conn = VIR_G_CONNECTION(object);
    VirGConnectionPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_URI:
        g_free(priv->uri);
        priv->uri = g_value_dup_string(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void vir_g_connection_finalize(GObject *object)
{
    VirGConnection *conn = VIR_G_CONNECTION(object);
    VirGConnectionPrivate *priv = conn->priv;

    DEBUG("Finalize VirGConnection=%p", conn);

    if (vir_g_connection_is_open(conn))
        vir_g_connection_close(conn);

    g_free(priv->uri);

    G_OBJECT_CLASS(vir_g_connection_parent_class)->finalize(object);
}


static void vir_g_connection_class_init(VirGConnectionClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = vir_g_connection_finalize;
    object_class->get_property = vir_g_connection_get_property;
    object_class->set_property = vir_g_connection_set_property;

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

    g_type_class_add_private(klass, sizeof(VirGConnectionPrivate));
}


static void vir_g_connection_init(VirGConnection *conn)
{
    VirGConnectionPrivate *priv;

    DEBUG("Init VirGConnection=%p", conn);

    priv = conn->priv = VIR_G_CONNECTION_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}


VirGConnection *vir_g_connection_new(const char *uri)
{
    return VIR_G_CONNECTION(g_object_new(VIR_G_TYPE_CONNECTION,
                                         "uri", uri,
                                         NULL));
}


static void
vir_g_connection_open_helper(GSimpleAsyncResult *res,
                             GObject *object,
                             GCancellable *cancellable)
{
    VirGConnection *conn = VIR_G_CONNECTION(object);
    VirGConnectionPrivate *priv = conn->priv;
    GError *err = NULL;

    if (priv->conn) {
        err = g_error_new(VIR_G_CONNECTION_ERROR,
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
        err = g_error_new(VIR_G_CONNECTION_ERROR,
                          0,
                          "Unable to open %s: %s",
                          priv->uri, verr->message);
        g_simple_async_result_set_from_error(res, err);
        g_error_free(err);
    }
}

gboolean vir_g_connection_open(VirGConnection *conn,
                               GError **err)
{
    VirGConnectionPrivate *priv = conn->priv;

    if (priv->conn) {
        *err = g_error_new(VIR_G_CONNECTION_ERROR,
                           0,
                           "Connection %s is already open",
                           priv->uri);
        return FALSE;
    }

    if (!(priv->conn = virConnectOpen(priv->uri))) {
        virErrorPtr verr = virGetLastError();
        *err = g_error_new(VIR_G_CONNECTION_ERROR,
                           0,
                           "Unable to open %s: %s",
                           priv->uri, verr->message);
        return FALSE;
    }

    return TRUE;
}

/**
 * vir_g_connection_open_async:
 * @cancellable: (allow-none): operation cancellation
 */
void vir_g_connection_open_async(VirGConnection *conn,
                                 GCancellable *cancellable,
                                 GAsyncReadyCallback callback,
                                 gpointer opaque)
{
    GSimpleAsyncResult *res;

    res = g_simple_async_result_new(G_OBJECT(conn),
                                    callback,
                                    opaque,
                                    vir_g_connection_open);
    g_simple_async_result_run_in_thread(res,
                                        vir_g_connection_open_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
}


gboolean vir_g_connection_open_finish(VirGConnection *conn,
                                      GAsyncResult *result,
                                      GError **err)
{
    g_return_val_if_fail(VIR_G_IS_CONNECTION(conn), FALSE);
    g_return_val_if_fail(G_IS_ASYNC_RESULT(result), FALSE);

    if (G_IS_SIMPLE_ASYNC_RESULT(result)) {
        GSimpleAsyncResult *simple = G_SIMPLE_ASYNC_RESULT(result);
        g_warn_if_fail (g_simple_async_result_get_source_tag(simple) == vir_g_connection_open);
        if (g_simple_async_result_propagate_error(simple, err))
            return FALSE;
    }

    return TRUE;
}


gboolean vir_g_connection_is_open(VirGConnection *conn)
{
    VirGConnectionPrivate *priv = conn->priv;

    return priv->conn == NULL ? FALSE : TRUE;
}

void vir_g_connection_close(VirGConnection *conn)
{
    VirGConnectionPrivate *priv = conn->priv;
    DEBUG("Close VirGConnection=%p", conn);

    if (!priv->conn)
        return;

    virConnectClose(priv->conn);
    priv->conn = NULL;
    /* xxx signals */
}

