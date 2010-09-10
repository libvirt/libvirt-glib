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
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"

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
        err = gvir_error_new(GVIR_CONNECTION_ERROR,
                             0,
                             "Unable to open %s",
                             priv->uri);
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
        *err = gvir_error_new(GVIR_CONNECTION_ERROR,
                              0,
                              "Unable to open %s",
                              priv->uri);
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_connection_open_async:
 * @cancellable: (allow-none): operation cancellation
 * @opaque: (allow-none):
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

    if (priv->domains)
        g_hash_table_unref(priv->domains);

    virConnectClose(priv->conn);
    priv->conn = NULL;
    /* xxx signals */
}

gboolean gvir_connection_fetch_domains(GVirConnection *conn,
                                       GError **err)
{
    GVirConnectionPrivate *priv = conn->priv;
    GHashTable *doms;
    gchar **inactive = NULL;
    gint ninactive = 0;
    gint *active = NULL;
    gint nactive = 0;
    gboolean ret = FALSE;
    gint i;

    if ((nactive = virConnectNumOfDomains(priv->conn)) < 0) {
        *err = gvir_error_new(GVIR_CONNECTION_ERROR,
                              0,
                              "Unable to count domains");
        goto cleanup;
    }
    if (nactive) {
        active = g_new(gint, nactive);
        if ((nactive = virConnectListDomains(priv->conn, active, nactive)) < 0) {
            *err = gvir_error_new(GVIR_CONNECTION_ERROR,
                                  0,
                                  "Unable to list domains");
            goto cleanup;
        }
    }

    if ((ninactive = virConnectNumOfDefinedDomains(priv->conn)) < 0) {
        *err = gvir_error_new(GVIR_CONNECTION_ERROR,
                              0,
                              "Unable to count domains");
        goto cleanup;
    }

    if (ninactive) {
        inactive = g_new(gchar *, ninactive);
        if ((ninactive = virConnectListDefinedDomains(priv->conn, inactive, ninactive)) < 0) {
            *err = gvir_error_new(GVIR_CONNECTION_ERROR,
                                  0,
                                  "Unable to list domains %d", ninactive);
            goto cleanup;
        }
    }

    doms = g_hash_table_new_full(g_str_hash,
                                 g_str_equal,
                                 g_free,
                                 g_object_unref);

    for (i = 0 ; i < nactive ; i++) {
        virDomainPtr vdom = virDomainLookupByID(priv->conn, active[i]);
        GVirDomain *dom;
        if (!vdom)
            continue;

        dom = GVIR_DOMAIN(g_object_new(GVIR_TYPE_DOMAIN,
                                       "handle", vdom,
                                       NULL));

        g_hash_table_insert(doms,
                            g_strdup(gvir_domain_get_uuid(dom)),
                            dom);
    }

    for (i = 0 ; i < ninactive ; i++) {
        virDomainPtr vdom = virDomainLookupByName(priv->conn, inactive[i]);
        GVirDomain *dom;
        if (!vdom)
            continue;

        dom = GVIR_DOMAIN(g_object_new(GVIR_TYPE_DOMAIN,
                                       "handle", vdom,
                                       NULL));

        g_hash_table_insert(doms,
                            g_strdup(gvir_domain_get_uuid(dom)),
                            dom);
    }

    if (priv->domains)
        g_hash_table_unref(priv->domains);
    priv->domains = doms;

    ret = TRUE;

cleanup:
    g_free(active);
    for (i = 0 ; i < ninactive ; i++)
        g_free(inactive[i]);
    g_free(inactive);
    return ret;
}


const gchar *gvir_connection_get_uri(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;
    return priv->uri;
}

/**
 * gvir_connection_get_domains:
 *
 * Return value: (element-type LibvirtGObject.Domain) (transfer container): List of #GVirDomain
 */
GList *gvir_connection_get_domains(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;

    return g_hash_table_get_values(priv->domains);
}

GVirDomain *gvir_connection_get_domain(GVirConnection *conn,
                                       const gchar *uuid)
{
    GVirConnectionPrivate *priv = conn->priv;

    return g_hash_table_lookup(priv->domains, uuid);
}


GVirDomain *gvir_connection_find_domain_by_id(GVirConnection *conn,
                                              gint id)
{
    GVirConnectionPrivate *priv = conn->priv;
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, priv->domains);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        GVirDomain *dom = value;
        gint thisid = gvir_domain_get_id(dom, NULL);

        if (thisid == id)
            return dom;
    }

    return NULL;
}


GVirDomain *gvir_connection_find_domain_by_name(GVirConnection *conn,
                                                const gchar *name)
{
    GVirConnectionPrivate *priv = conn->priv;
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, priv->domains);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        GVirDomain *dom = value;
        gchar *thisname = gvir_domain_get_name(dom);

        if (strcmp(thisname, name) == 0) {
            g_free(thisname);
            return dom;
        }
        g_free(thisname);
    }

    return NULL;
}

