/*
 * libvirt-gobject-connection.c: libvirt glib integration
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
#include "libvirt-gobject-compat.h"

#define GVIR_CONNECTION_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONNECTION, GVirConnectionPrivate))

struct _GVirConnectionPrivate
{
    GMutex *lock;
    gchar *uri;
    virConnectPtr conn;

    GHashTable *domains;
    GHashTable *pools;
    gboolean    domain_event;
};

G_DEFINE_TYPE(GVirConnection, gvir_connection, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_URI,
    PROP_HANDLE,
};

enum {
    VIR_CONNECTION_OPENED,
    VIR_CONNECTION_CLOSED,
    VIR_DOMAIN_ADDED,
    VIR_DOMAIN_REMOVED,
    LAST_SIGNAL
};

static gint signals[LAST_SIGNAL];

#define GVIR_CONNECTION_ERROR gvir_connection_error_quark()

static GQuark
gvir_connection_error_quark(void)
{
    return g_quark_from_static_string("gvir-connection");
}

static GVirNodeInfo *
gvir_node_info_copy(GVirNodeInfo *info)
{
    return g_slice_dup(GVirNodeInfo, info);
}

static void
gvir_node_info_free(GVirNodeInfo *info)
{
    g_slice_free(GVirNodeInfo, info);
}

G_DEFINE_BOXED_TYPE(GVirNodeInfo, gvir_node_info,
                    gvir_node_info_copy, gvir_node_info_free)

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
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->conn);
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

    case PROP_HANDLE:
        if (priv->conn)
            virConnectClose(priv->conn);
        priv->conn = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_connection_finalize(GObject *object)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GVirConnectionPrivate *priv = conn->priv;

    g_debug("Finalize GVirConnection=%p", conn);

    if (gvir_connection_is_open(conn))
        gvir_connection_close(conn);

    g_mutex_free(priv->lock);
    g_free(priv->uri);

    G_OBJECT_CLASS(gvir_connection_parent_class)->finalize(object);
}


static GVirStream* gvir_connection_stream_new(GVirConnection *self G_GNUC_UNUSED,
                                             gpointer handle)
{
    return g_object_new(GVIR_TYPE_STREAM, "handle", handle, NULL);
}

static void gvir_connection_class_init(GVirConnectionClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    gvir_event_register();

    object_class->finalize = gvir_connection_finalize;
    object_class->get_property = gvir_connection_get_property;
    object_class->set_property = gvir_connection_set_property;

    klass->stream_new = gvir_connection_stream_new;

    g_object_class_install_property(object_class,
                                    PROP_URI,
                                    g_param_spec_string("uri",
                                                        "URI",
                                                        "The connection URI",
                                                        NULL,
                                                        G_PARAM_READABLE |
                                                        G_PARAM_WRITABLE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The connection handle",
                                                       GVIR_TYPE_CONNECTION_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));

    signals[VIR_CONNECTION_OPENED] = g_signal_new("connection-opened",
                 G_OBJECT_CLASS_TYPE(object_class),
                 G_SIGNAL_RUN_FIRST,
                 G_STRUCT_OFFSET(GVirConnectionClass, connection_opened),
                 NULL, NULL,
                 g_cclosure_marshal_VOID__VOID,
                 G_TYPE_NONE,
                 0);

    signals[VIR_CONNECTION_CLOSED] = g_signal_new("connection-closed",
                 G_OBJECT_CLASS_TYPE(object_class),
                 G_SIGNAL_RUN_FIRST,
                 G_STRUCT_OFFSET(GVirConnectionClass, connection_closed),
                 NULL, NULL,
                 g_cclosure_marshal_VOID__VOID,
                 G_TYPE_NONE,
                 0);

    signals[VIR_DOMAIN_ADDED] = g_signal_new("domain-added",
                 G_OBJECT_CLASS_TYPE(object_class),
                 G_SIGNAL_RUN_FIRST,
                 G_STRUCT_OFFSET(GVirConnectionClass, domain_added),
                 NULL, NULL,
                 g_cclosure_marshal_VOID__OBJECT,
                 G_TYPE_NONE,
                 1,
                 GVIR_TYPE_DOMAIN);

    signals[VIR_DOMAIN_REMOVED] = g_signal_new("domain-removed",
                 G_OBJECT_CLASS_TYPE(object_class),
                 G_SIGNAL_RUN_FIRST,
                 G_STRUCT_OFFSET(GVirConnectionClass, domain_removed),
                 NULL, NULL,
                 g_cclosure_marshal_VOID__OBJECT,
                 G_TYPE_NONE,
                 1,
                 GVIR_TYPE_DOMAIN);

    g_type_class_add_private(klass, sizeof(GVirConnectionPrivate));
}


static void gvir_connection_init(GVirConnection *conn)
{
    GVirConnectionPrivate *priv;

    g_debug("Init GVirConnection=%p", conn);

    priv = conn->priv = GVIR_CONNECTION_GET_PRIVATE(conn);

    priv->lock = g_mutex_new();
    priv->domains = g_hash_table_new_full(g_str_hash,
                                          g_str_equal,
                                          NULL,
                                          g_object_unref);
    priv->pools = g_hash_table_new_full(g_str_hash,
                                        g_str_equal,
                                        NULL,
                                        g_object_unref);
}


GVirConnection *gvir_connection_new(const char *uri)
{
    return GVIR_CONNECTION(g_object_new(GVIR_TYPE_CONNECTION,
                                         "uri", uri,
                                         NULL));
}


static int domain_event_cb(virConnectPtr conn G_GNUC_UNUSED,
                           virDomainPtr dom,
                           int event,
                           int detail,
                           void *opaque)
{
    gchar uuid[VIR_UUID_STRING_BUFLEN];
    GHashTable *doms;
    GVirConnection *gconn = opaque;
    GVirDomain *gdom;
    GVirConnectionPrivate *priv = gconn->priv;

    if (virDomainGetUUIDString(dom, uuid) < 0) {
        g_warning("Failed to get domain UUID on %p", dom);
        return 0;
    }

    g_debug("%s: %s event:%d, detail:%d", G_STRFUNC, uuid, event, detail);

    g_mutex_lock(priv->lock);
    doms = g_hash_table_ref(priv->domains);
    gdom = g_hash_table_lookup(doms, uuid);
    if (gdom != NULL)
        g_object_ref(G_OBJECT(gdom));
    g_mutex_unlock(priv->lock);

    if (gdom == NULL) {
        gdom = GVIR_DOMAIN(g_object_new(GVIR_TYPE_DOMAIN, "handle", dom, NULL));

        g_mutex_lock(priv->lock);
        g_hash_table_insert(doms, (gpointer)gvir_domain_get_uuid(gdom),
                            g_object_ref(G_OBJECT(gdom)));
        g_mutex_unlock(priv->lock);
    }

    switch (event) {
        case VIR_DOMAIN_EVENT_DEFINED:
            if (detail == VIR_DOMAIN_EVENT_DEFINED_ADDED)
                g_signal_emit(gconn, signals[VIR_DOMAIN_ADDED], 0, gdom);
            else if (detail == VIR_DOMAIN_EVENT_DEFINED_UPDATED)
                g_signal_emit_by_name(gdom, "updated");
            else
                g_warn_if_reached();
            break;

        case VIR_DOMAIN_EVENT_UNDEFINED:
            if (detail == VIR_DOMAIN_EVENT_UNDEFINED_REMOVED) {
                g_mutex_lock(priv->lock);
                g_hash_table_remove(doms, uuid);
                g_mutex_unlock(priv->lock);

                g_signal_emit(gconn, signals[VIR_DOMAIN_REMOVED], 0, gdom);
            } else
                g_warn_if_reached();
            break;

        case VIR_DOMAIN_EVENT_STARTED:
            if (detail == VIR_DOMAIN_EVENT_STARTED_BOOTED) {
                if (!virDomainIsPersistent(dom))
                    g_signal_emit(gconn, signals[VIR_DOMAIN_ADDED], 0, gdom);
                g_signal_emit_by_name(gdom, "started::booted");
            } else if (detail == VIR_DOMAIN_EVENT_STARTED_MIGRATED)
                g_signal_emit_by_name(gdom, "started::migrated");
            else if (detail == VIR_DOMAIN_EVENT_STARTED_RESTORED)
                g_signal_emit_by_name(gdom, "started::restored");
            else if (detail == VIR_DOMAIN_EVENT_STARTED_FROM_SNAPSHOT)
                g_signal_emit_by_name(gdom, "started::from-snapshot");
            else
                g_warn_if_reached();
            break;

        case VIR_DOMAIN_EVENT_SUSPENDED:
            if (detail == VIR_DOMAIN_EVENT_SUSPENDED_PAUSED)
                g_signal_emit_by_name(gdom, "suspended::paused");
            else if (detail == VIR_DOMAIN_EVENT_SUSPENDED_MIGRATED)
                g_signal_emit_by_name(gdom, "suspended::migrated");
            else if (detail == VIR_DOMAIN_EVENT_SUSPENDED_IOERROR)
                g_signal_emit_by_name(gdom, "suspended::ioerror");
            else if (detail == VIR_DOMAIN_EVENT_SUSPENDED_WATCHDOG)
                g_signal_emit_by_name(gdom, "suspended::watchdog");
            else if (detail == VIR_DOMAIN_EVENT_SUSPENDED_RESTORED)
                g_signal_emit_by_name(gdom, "suspended::restored");
            else if (detail == VIR_DOMAIN_EVENT_SUSPENDED_FROM_SNAPSHOT)
                g_signal_emit_by_name(gdom, "suspended::from-snapshot");
            else
                g_warn_if_reached();
            break;

        case VIR_DOMAIN_EVENT_RESUMED:
            if (detail == VIR_DOMAIN_EVENT_RESUMED_UNPAUSED)
                g_signal_emit_by_name(gdom, "resumed::unpaused");
            else if (detail == VIR_DOMAIN_EVENT_RESUMED_MIGRATED)
                g_signal_emit_by_name(gdom, "resumed::migrated");
            else if (detail == VIR_DOMAIN_EVENT_RESUMED_FROM_SNAPSHOT)
                g_signal_emit_by_name(gdom, "resumed::from-snapshot");
            else
                g_warn_if_reached();
            break;

        case VIR_DOMAIN_EVENT_STOPPED:
            if (detail == VIR_DOMAIN_EVENT_STOPPED_SHUTDOWN)
                g_signal_emit_by_name(gdom, "stopped::shutdown");
            else if (detail == VIR_DOMAIN_EVENT_STOPPED_DESTROYED)
                g_signal_emit_by_name(gdom, "stopped::destroyed");
            else if (detail == VIR_DOMAIN_EVENT_STOPPED_CRASHED)
                g_signal_emit_by_name(gdom, "stopped::crashed");
            else if (detail == VIR_DOMAIN_EVENT_STOPPED_MIGRATED)
                g_signal_emit_by_name(gdom, "stopped::migrated");
            else if (detail == VIR_DOMAIN_EVENT_STOPPED_SAVED)
                g_signal_emit_by_name(gdom, "stopped::saved");
            else if (detail == VIR_DOMAIN_EVENT_STOPPED_FAILED)
                g_signal_emit_by_name(gdom, "stopped::failed");
            else if (detail == VIR_DOMAIN_EVENT_STOPPED_FROM_SNAPSHOT)
                g_signal_emit_by_name(gdom, "stopped::from-snapshot");
            else
                g_warn_if_reached();

            if (virDomainIsPersistent(dom) != 1) {
                g_mutex_lock(priv->lock);
                g_hash_table_remove(doms, uuid);
                g_mutex_unlock(priv->lock);

                g_signal_emit(gconn, signals[VIR_DOMAIN_REMOVED], 0, gdom);
            }
            break;

        case VIR_DOMAIN_EVENT_SHUTDOWN:
            break;

        default:
            g_warn_if_reached();
    }

    g_object_unref(G_OBJECT(gdom));
    g_hash_table_unref(doms);
    return 0;
}

/**
 * gvir_connection_open:
 * @conn: the connection
 * @cancellable: (allow-none)(transfer none): cancellation object
 */
gboolean gvir_connection_open(GVirConnection *conn,
                              GCancellable *cancellable,
                              GError **err)
{
    GVirConnectionPrivate *priv = conn->priv;

    if (g_cancellable_set_error_if_cancelled(cancellable, err))
        return FALSE;

    g_mutex_lock(priv->lock);
    if (priv->conn) {
        g_set_error(err, GVIR_CONNECTION_ERROR,
                    0,
                    "Connection %s is already open",
                    priv->uri);
        g_mutex_unlock(priv->lock);
        return FALSE;
    }

    if (!(priv->conn = virConnectOpen(priv->uri))) {
        gvir_set_error(err, GVIR_CONNECTION_ERROR,
                       0,
                       "Unable to open %s",
                       priv->uri);
        g_mutex_unlock(priv->lock);
        return FALSE;
    }

    if (!priv->uri) {
        char *uri = virConnectGetURI(priv->conn);
        if (!uri) {
            gvir_set_error_literal(err, GVIR_CONNECTION_ERROR,
                           0,
                           "Unable to get connection URI");
            virConnectClose(priv->conn);
            priv->conn = NULL;
            g_mutex_unlock(priv->lock);
            return FALSE;
        }
        priv->uri = g_strdup(uri);
        free(uri);
    }

    if (virConnectDomainEventRegister(priv->conn, domain_event_cb, conn, NULL) != -1)
        priv->domain_event = TRUE;
    else
        g_warning("Failed to register domain events, ignoring");

    g_mutex_unlock(priv->lock);

    g_signal_emit(conn, signals[VIR_CONNECTION_OPENED], 0);

    return TRUE;
}


static void
gvir_connection_open_helper(GSimpleAsyncResult *res,
                            GObject *object,
                            GCancellable *cancellable)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GError *err = NULL;

    if (!gvir_connection_open(conn, cancellable, &err)) {
        g_simple_async_result_set_from_error(res, err);
        g_error_free(err);
    }
}


/**
 * gvir_connection_open_async:
 * @conn: the connection
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 */
void gvir_connection_open_async(GVirConnection *conn,
                                GCancellable *cancellable,
                                GAsyncReadyCallback callback,
                                gpointer user_data)
{
    GSimpleAsyncResult *res;

    res = g_simple_async_result_new(G_OBJECT(conn),
                                    callback,
                                    user_data,
                                    gvir_connection_open_async);
    g_simple_async_result_run_in_thread(res,
                                        gvir_connection_open_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
}


/**
 * gvir_connection_open_finish:
 * @conn: the connection
 * @result: (transfer none): async method result
 */
gboolean gvir_connection_open_finish(GVirConnection *conn,
                                     GAsyncResult *result,
                                     GError **err)
{
    g_return_val_if_fail(GVIR_IS_CONNECTION(conn), FALSE);
    g_return_val_if_fail(g_simple_async_result_is_valid(result, G_OBJECT(conn),
                                                        gvir_connection_open_async),
                         FALSE);

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(result), err))
        return FALSE;

    return TRUE;
}


gboolean gvir_connection_is_open(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;
    gboolean open = TRUE;
    g_mutex_lock(priv->lock);
    if (!priv->conn)
        open = FALSE;
    g_mutex_unlock(priv->lock);
    return open;
}

void gvir_connection_close(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;
    g_debug("Close GVirConnection=%p", conn);

    g_mutex_lock(priv->lock);

    if (priv->domains) {
        g_hash_table_unref(priv->domains);
        priv->domains = NULL;
    }

    if (priv->pools) {
        g_hash_table_unref(priv->pools);
        priv->pools = NULL;
    }

    if (priv->conn) {
        virConnectDomainEventDeregister(priv->conn, domain_event_cb);
        priv->domain_event = FALSE;
        virConnectClose(priv->conn);
        priv->conn = NULL;
    }
    /* xxx signals */

    g_mutex_unlock(priv->lock);

    g_signal_emit(conn, signals[VIR_CONNECTION_CLOSED], 0);
}

typedef gint (* CountFunction) (virConnectPtr vconn);
typedef gint (* ListFunction) (virConnectPtr vconn, gchar **lst, gint max);

static gchar ** fetch_list(virConnectPtr vconn,
                           const char *name,
                           CountFunction count_func,
                           ListFunction list_func,
                           GCancellable *cancellable,
                           gint *length,
                           GError **err)
{
    gchar **lst = NULL;
    gint n = 0;
    gint i;

    if ((n = count_func(vconn)) < 0) {
        gvir_set_error(err, GVIR_CONNECTION_ERROR,
                       0,
                       "Unable to count %s", name);
        goto error;
    }

    if (n) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto error;

        lst = g_new(gchar *, n);
        if ((n = list_func(vconn, lst, n)) < 0) {
            gvir_set_error(err, GVIR_CONNECTION_ERROR,
                           0,
                           "Unable to list %s %d", name, n);
            goto error;
        }
    }

    *length = n;
    return lst;

error:
    for (i = 0 ; i < n; i++)
        g_free(lst[i]);
    g_free(lst);
    return NULL;
}

/**
 * gvir_connection_fetch_domains:
 * @conn: the connection
 * @cancellable: (allow-none)(transfer none): cancellation object
 */
gboolean gvir_connection_fetch_domains(GVirConnection *conn,
                                       GCancellable *cancellable,
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
    virConnectPtr vconn = NULL;
    GError *lerr = NULL;

    g_mutex_lock(priv->lock);
    if (!priv->conn) {
        g_set_error_literal(err, GVIR_CONNECTION_ERROR,
                            0,
                            "Connection is not open");
        g_mutex_unlock(priv->lock);
        goto cleanup;
    }
    vconn = priv->conn;
    /* Stop another thread closing the connection just at the minute */
    virConnectRef(vconn);
    g_mutex_unlock(priv->lock);

    if (g_cancellable_set_error_if_cancelled(cancellable, err))
        goto cleanup;

    if ((nactive = virConnectNumOfDomains(vconn)) < 0) {
        gvir_set_error_literal(err, GVIR_CONNECTION_ERROR,
                               0,
                               "Unable to count domains");
        goto cleanup;
    }
    if (nactive) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto cleanup;

        active = g_new(gint, nactive);
        if ((nactive = virConnectListDomains(vconn, active, nactive)) < 0) {
            gvir_set_error_literal(err, GVIR_CONNECTION_ERROR,
                                   0,
                                   "Unable to list domains");
            goto cleanup;
        }
    }

    if (g_cancellable_set_error_if_cancelled(cancellable, err))
        goto cleanup;

    inactive = fetch_list(vconn,
                          "Domains",
                          virConnectNumOfDefinedDomains,
                          virConnectListDefinedDomains,
                          cancellable,
                          &ninactive,
                          &lerr);
    if (lerr) {
        g_propagate_error(err, lerr);
        lerr = NULL;
        goto cleanup;
    }

    doms = g_hash_table_new_full(g_str_hash,
                                 g_str_equal,
                                 NULL,
                                 g_object_unref);

    for (i = 0 ; i < nactive ; i++) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto cleanup;

        virDomainPtr vdom = virDomainLookupByID(vconn, active[i]);
        GVirDomain *dom;
        if (!vdom)
            continue;

        dom = GVIR_DOMAIN(g_object_new(GVIR_TYPE_DOMAIN,
                                       "handle", vdom,
                                       NULL));

        g_hash_table_insert(doms,
                            (gpointer)gvir_domain_get_uuid(dom),
                            dom);
    }

    for (i = 0 ; i < ninactive ; i++) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto cleanup;

        virDomainPtr vdom = virDomainLookupByName(vconn, inactive[i]);
        GVirDomain *dom;
        if (!vdom)
            continue;

        dom = GVIR_DOMAIN(g_object_new(GVIR_TYPE_DOMAIN,
                                       "handle", vdom,
                                       NULL));

        g_hash_table_insert(doms,
                            (gpointer)gvir_domain_get_uuid(dom),
                            dom);
    }

    g_mutex_lock(priv->lock);
    if (priv->domains)
        g_hash_table_unref(priv->domains);
    priv->domains = doms;
    virConnectClose(vconn);
    g_mutex_unlock(priv->lock);

    ret = TRUE;

cleanup:
    g_free(active);
    for (i = 0 ; i < ninactive ; i++)
        g_free(inactive[i]);
    g_free(inactive);
    return ret;
}

/**
 * gvir_connection_fetch_storage_pools:
 * @conn: the connection
 * @cancellable: (allow-none)(transfer none): cancellation object
 */
gboolean gvir_connection_fetch_storage_pools(GVirConnection *conn,
                                             GCancellable *cancellable,
                                             GError **err)
{
    GVirConnectionPrivate *priv = conn->priv;
    GHashTable *pools;
    gchar **inactive = NULL;
    gint ninactive = 0;
    gchar **active = NULL;
    gint nactive = 0;
    gboolean ret = FALSE;
    gint i;
    virConnectPtr vconn = NULL;
    GError *lerr = NULL;

    g_mutex_lock(priv->lock);
    if (!priv->conn) {
        g_set_error_literal(err, GVIR_CONNECTION_ERROR,
                            0,
                            "Connection is not open");
        g_mutex_unlock(priv->lock);
        goto cleanup;
    }
    vconn = priv->conn;
    /* Stop another thread closing the connection just at the minute */
    virConnectRef(vconn);
    g_mutex_unlock(priv->lock);

    if (g_cancellable_set_error_if_cancelled(cancellable, err))
        goto cleanup;

    active = fetch_list(vconn,
                        "Storage Pools",
                        virConnectNumOfStoragePools,
                        virConnectListStoragePools,
                        cancellable,
                        &nactive,
                        &lerr);
    if (lerr) {
        g_propagate_error(err, lerr);
        lerr = NULL;
        goto cleanup;
    }

    if (g_cancellable_set_error_if_cancelled(cancellable, err))
        goto cleanup;

    inactive = fetch_list(vconn,
                          "Storage Pools",
                          virConnectNumOfDefinedStoragePools,
                          virConnectListDefinedStoragePools,
                          cancellable,
                          &ninactive,
                          &lerr);
    if (lerr) {
        g_propagate_error(err, lerr);
        lerr = NULL;
        goto cleanup;
    }

    pools = g_hash_table_new_full(g_str_hash,
                                  g_str_equal,
                                  NULL,
                                  g_object_unref);

    for (i = 0 ; i < nactive ; i++) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto cleanup;

        virStoragePoolPtr vpool;
        GVirStoragePool *pool;

        vpool = virStoragePoolLookupByName(vconn, active[i]);
        if (!vpool)
            continue;

        pool = GVIR_STORAGE_POOL(g_object_new(GVIR_TYPE_STORAGE_POOL,
                                              "handle", vpool,
                                              NULL));

        g_hash_table_insert(pools,
                            (gpointer)gvir_storage_pool_get_uuid(pool),
                            pool);
    }

    for (i = 0 ; i < ninactive ; i++) {
        if (g_cancellable_set_error_if_cancelled(cancellable, err))
            goto cleanup;

        virStoragePoolPtr vpool;
        GVirStoragePool *pool;

        vpool = virStoragePoolLookupByName(vconn, inactive[i]);
        if (!vpool)
            continue;

        pool = GVIR_STORAGE_POOL(g_object_new(GVIR_TYPE_STORAGE_POOL,
                                              "handle", vpool,
                                              NULL));

        g_hash_table_insert(pools,
                            (gpointer)gvir_storage_pool_get_uuid(pool),
                            pool);
    }

    g_mutex_lock(priv->lock);
    if (priv->pools)
        g_hash_table_unref(priv->pools);
    priv->pools = pools;
    virConnectClose(vconn);
    g_mutex_unlock(priv->lock);

    ret = TRUE;

cleanup:
    for (i = 0 ; i < nactive ; i++)
        g_free(active[i]);
    g_free(active);
    for (i = 0 ; i < ninactive ; i++)
        g_free(inactive[i]);
    g_free(inactive);
    return ret;
}

static void
gvir_connection_fetch_domains_helper(GSimpleAsyncResult *res,
                                     GObject *object,
                                     GCancellable *cancellable)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GError *err = NULL;

    if (!gvir_connection_fetch_domains(conn, cancellable, &err)) {
        g_simple_async_result_set_from_error(res, err);
        g_error_free(err);
    }
}


/**
 * gvir_connection_fetch_domains_async:
 * @conn: the connection
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 */
void gvir_connection_fetch_domains_async(GVirConnection *conn,
                                         GCancellable *cancellable,
                                         GAsyncReadyCallback callback,
                                         gpointer user_data)
{
    GSimpleAsyncResult *res;

    res = g_simple_async_result_new(G_OBJECT(conn),
                                    callback,
                                    user_data,
                                    gvir_connection_fetch_domains_async);
    g_simple_async_result_run_in_thread(res,
                                        gvir_connection_fetch_domains_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
}

/**
 * gvir_connection_fetch_domains_finish:
 * @conn: the connection
 * @result: (transfer none): async method result
 */
gboolean gvir_connection_fetch_domains_finish(GVirConnection *conn,
                                              GAsyncResult *result,
                                              GError **err)
{
    g_return_val_if_fail(GVIR_IS_CONNECTION(conn), FALSE);
    g_return_val_if_fail(g_simple_async_result_is_valid(result, G_OBJECT(conn),
                                                        gvir_connection_fetch_domains_async),
                         FALSE);

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(result), err))
        return FALSE;

    return TRUE;
}

static void
gvir_connection_fetch_pools_helper(GSimpleAsyncResult *res,
                                   GObject *object,
                                   GCancellable *cancellable)
{
    GVirConnection *conn = GVIR_CONNECTION(object);
    GError *err = NULL;

    if (!gvir_connection_fetch_storage_pools(conn, cancellable, &err)) {
        g_simple_async_result_set_from_error(res, err);
        g_error_free(err);
    }
}

/**
 * gvir_connection_fetch_storage_pools_async:
 * @conn: the connection
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 */
void gvir_connection_fetch_storage_pools_async(GVirConnection *conn,
                                               GCancellable *cancellable,
                                               GAsyncReadyCallback callback,
                                               gpointer user_data)
{
    GSimpleAsyncResult *res;

    res = g_simple_async_result_new(G_OBJECT(conn),
                                    callback,
                                    user_data,
                                    gvir_connection_fetch_storage_pools_async);
    g_simple_async_result_run_in_thread(res,
                                        gvir_connection_fetch_pools_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
}

/**
 * gvir_connection_fetch_storage_pools_finish:
 * @conn: the connection
 * @result: (transfer none): async method result
 */
gboolean gvir_connection_fetch_storage_pools_finish(GVirConnection *conn,
                                                    GAsyncResult *result,
                                                    GError **err)
{
    g_return_val_if_fail(GVIR_IS_CONNECTION(conn), FALSE);
    g_return_val_if_fail(g_simple_async_result_is_valid(result, G_OBJECT(conn),
                                                        gvir_connection_fetch_storage_pools_async),
                         FALSE);

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(result), err))
        return FALSE;

    return TRUE;
}

const gchar *gvir_connection_get_uri(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;
    return priv->uri;
}

static void gvir_domain_ref(gpointer obj, gpointer ignore G_GNUC_UNUSED)
{
    g_object_ref(obj);
}

/**
 * gvir_connection_get_domains:
 *
 * Return value: (element-type LibvirtGObject.Domain) (transfer full): List of #GVirDomain
 */
GList *gvir_connection_get_domains(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;
    GList *domains = NULL;

    g_mutex_lock(priv->lock);
    if (priv->domains != NULL) {
        domains = g_hash_table_get_values(priv->domains);
        g_list_foreach(domains, gvir_domain_ref, NULL);
    }
    g_mutex_unlock(priv->lock);

    return domains;
}

/**
 * gvir_connection_get_storage_pools:
 *
 * Return value: (element-type LibvirtGObject.StoragePool) (transfer full): List
 * of #GVirStoragePool
 */
GList *gvir_connection_get_storage_pools(GVirConnection *conn)
{
    GVirConnectionPrivate *priv = conn->priv;
    GList *pools = NULL;

    g_mutex_lock(priv->lock);
    if (priv->pools != NULL) {
        pools = g_hash_table_get_values(priv->pools);
        g_list_foreach(pools, gvir_domain_ref, NULL);
    }
    g_mutex_unlock(priv->lock);

    return pools;
}

/**
 * gvir_connection_get_domain:
 * @uuid: uuid string of the requested domain
 *
 * Return value: (transfer full): the #GVirDomain, or NULL
 */
GVirDomain *gvir_connection_get_domain(GVirConnection *conn,
                                       const gchar *uuid)
{
    GVirConnectionPrivate *priv = conn->priv;
    GVirDomain *dom;
    g_mutex_lock(priv->lock);
    dom = g_hash_table_lookup(priv->domains, uuid);
    if (dom)
        g_object_ref(dom);
    g_mutex_unlock(priv->lock);
    return dom;
}

/**
 * gvir_connection_get_storage_pool:
 * @uuid: uuid string of the requested storage pool
 *
 * Return value: (transfer full): the #GVirStoragePool, or NULL
 */
GVirStoragePool *gvir_connection_get_storage_pool(GVirConnection *conn,
                                                  const gchar *uuid)
{
    GVirConnectionPrivate *priv = conn->priv;
    GVirStoragePool *pool;

    g_mutex_lock(priv->lock);
    pool = g_hash_table_lookup(priv->pools, uuid);
    if (pool)
        g_object_ref(pool);
    g_mutex_unlock(priv->lock);

    return pool;
}

/**
 * gvir_connection_find_domain_by_id:
 * @id: id of the requested domain
 *
 * Return value: (transfer full): the #GVirDomain, or NULL
 */
GVirDomain *gvir_connection_find_domain_by_id(GVirConnection *conn,
                                              gint id)
{
    GVirConnectionPrivate *priv = conn->priv;
    GHashTableIter iter;
    gpointer key, value;

    g_mutex_lock(priv->lock);
    g_hash_table_iter_init(&iter, priv->domains);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        GVirDomain *dom = value;
        gint thisid = gvir_domain_get_id(dom, NULL);

        if (thisid == id) {
            g_object_ref(dom);
            g_mutex_unlock(priv->lock);
            return dom;
        }
    }
    g_mutex_unlock(priv->lock);

    return NULL;
}


/**
 * gvir_connection_find_domain_by_name:
 * @name: name of the requested domain
 *
 * Return value: (transfer full): the #GVirDomain, or NULL
 */
GVirDomain *gvir_connection_find_domain_by_name(GVirConnection *conn,
                                                const gchar *name)
{
    GVirConnectionPrivate *priv = conn->priv;
    GHashTableIter iter;
    gpointer key, value;

    g_mutex_lock(priv->lock);
    g_hash_table_iter_init(&iter, priv->domains);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        GVirDomain *dom = value;
        const gchar *thisname = gvir_domain_get_name(dom);

        if (thisname == NULL)
            continue;

        if (strcmp(thisname, name) == 0) {
            g_object_ref(dom);
            g_mutex_unlock(priv->lock);
            return dom;
        }
    }
    g_mutex_unlock(priv->lock);

    return NULL;
}

/**
 * gvir_connection_find_storage_pool_by_name:
 * @name: name of the requested storage pool
 *
 * Return value: (transfer full): the #GVirStoragePool, or NULL
 */
GVirStoragePool *gvir_connection_find_storage_pool_by_name(GVirConnection *conn,
                                                           const gchar *name)
{
    GVirConnectionPrivate *priv = conn->priv;
    GHashTableIter iter;
    gpointer key, value;

    g_mutex_lock(priv->lock);
    g_hash_table_iter_init(&iter, priv->pools);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        GVirStoragePool *pool = value;
        const gchar *thisname = gvir_storage_pool_get_name(pool);

        if (thisname == NULL)
            continue;

        if (strcmp(thisname, name) == 0) {
            g_object_ref(pool);
            g_mutex_unlock(priv->lock);
            return pool;
        }
    }
    g_mutex_unlock(priv->lock);

    return NULL;
}

typedef struct virConnect GVirConnectionHandle;

static GVirConnectionHandle*
gvir_connection_handle_copy(GVirConnectionHandle *src)
{
    virConnectRef((virConnectPtr)src);
    return src;
}

static void
gvir_connection_handle_free(GVirConnectionHandle *src)
{
    virConnectClose((virConnectPtr)src);
}

G_DEFINE_BOXED_TYPE(GVirConnectionHandle, gvir_connection_handle,
                    gvir_connection_handle_copy, gvir_connection_handle_free)

/**
 * gvir_connection_get_stream:
 * @flags: flags to use for the stream
 *
 * Return value: (transfer full): a #GVirStream stream, or NULL
 */
GVirStream *gvir_connection_get_stream(GVirConnection *self,
                                       guint flags)
{
    GVirConnectionClass *klass;

    g_return_val_if_fail(GVIR_IS_CONNECTION(self), NULL);
    g_return_val_if_fail(self->priv->conn, NULL);

    klass = GVIR_CONNECTION_GET_CLASS(self);
    g_return_val_if_fail(klass->stream_new, NULL);

    virStreamPtr st = virStreamNew(self->priv->conn, flags | VIR_STREAM_NONBLOCK);

    return klass->stream_new(self, st);
}

/**
 * gvir_connection_create_domain:
 * @conn: the connection on which to create the domain
 * @conf: the configuration for the new domain
 *
 * Create the configuration file for a new persistent domain.
 * The returned domain will initially be in the shutoff state.
 *
 * Returns: (transfer full): the newly created domain
 */
GVirDomain *gvir_connection_create_domain(GVirConnection *conn,
                                          GVirConfigDomain *conf,
                                          GError **err)
{
    gchar *xml;
    virDomainPtr handle;
    GVirConnectionPrivate *priv = conn->priv;

    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(conf));

    g_return_val_if_fail(xml != NULL, NULL);

    handle = virDomainDefineXML(priv->conn, xml);
    g_free(xml);
    if (!handle) {
        gvir_set_error_literal(err, GVIR_CONNECTION_ERROR,
                               0,
                               "Failed to create domain");
        return NULL;
    }

    GVirDomain *domain;

    domain = GVIR_DOMAIN(g_object_new(GVIR_TYPE_DOMAIN,
                                       "handle", handle,
                                       NULL));

    g_mutex_lock(priv->lock);
    g_hash_table_insert(priv->domains,
                        (gpointer)gvir_domain_get_uuid(domain),
                        g_object_ref(domain));
    g_mutex_unlock(priv->lock);

    return domain;
}

/**
 * gvir_connection_start_domain:
 * @conn: the connection on which to create the domain
 * @conf: the configuration for the new domain
 *
 * Start a new transient domain without persistent configuration.
 * The returned domain will initially be running.
 *
 * Returns: (transfer full): the newly created domain
 */
GVirDomain *gvir_connection_start_domain(GVirConnection *conn,
                                         GVirConfigDomain *conf,
                                         guint flags,
                                         GError **err)
{
    gchar *xml;
    virDomainPtr handle;
    GVirConnectionPrivate *priv = conn->priv;

    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(conf));

    g_return_val_if_fail(xml != NULL, NULL);

    handle = virDomainCreateXML(priv->conn, xml, flags);
    g_free(xml);
    if (!handle) {
        gvir_set_error_literal(err, GVIR_CONNECTION_ERROR,
                               0,
                               "Failed to create domain");
        return NULL;
    }

    GVirDomain *domain;

    domain = GVIR_DOMAIN(g_object_new(GVIR_TYPE_DOMAIN,
                                       "handle", handle,
                                       NULL));

    g_mutex_lock(priv->lock);
    g_hash_table_insert(priv->domains,
                        (gpointer)gvir_domain_get_uuid(domain),
                        g_object_ref(domain));
    g_mutex_unlock(priv->lock);

    return domain;
}

/**
 * gvir_connection_create_storage_pool:
 * @conn: the connection on which to create the pool
 * @conf: the configuration for the new storage pool
 * @flags:  the flags
 * @err: return location for any #GError
 *
 * Returns: (transfer full): the newly created storage pool
 */
GVirStoragePool *gvir_connection_create_storage_pool
                                (GVirConnection *conn,
                                 GVirConfigStoragePool *conf,
                                 guint flags,
                                 GError **err) {
    const gchar *xml;
    virStoragePoolPtr handle;
    GVirConnectionPrivate *priv = conn->priv;

    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(conf));

    g_return_val_if_fail(xml != NULL, NULL);

    if (!(handle = virStoragePoolDefineXML(priv->conn, xml, flags))) {
        gvir_set_error_literal(err, GVIR_CONNECTION_ERROR,
                               flags,
                               "Failed to create storage pool");
        return NULL;
    }

    GVirStoragePool *pool;

    pool = GVIR_STORAGE_POOL(g_object_new(GVIR_TYPE_STORAGE_POOL,
                                          "handle", handle,
                                          NULL));

    g_mutex_lock(priv->lock);
    g_hash_table_insert(priv->pools,
                        (gpointer)gvir_storage_pool_get_uuid(pool),
                        pool);
    g_mutex_unlock(priv->lock);

    return g_object_ref(pool);
}

/**
 * gvir_connection_get_node_info:
 * @conn: the connection
 * @err: return location for any #GError
 *
 * Returns: (transfer full): the info
 */
GVirNodeInfo *gvir_connection_get_node_info(GVirConnection *conn,
                                            GError **err)
{
    GVirConnectionPrivate *priv = conn->priv;
    virNodeInfo info;
    GVirNodeInfo *ret;

    if (virNodeGetInfo(priv->conn, &info) < 0) {
        gvir_set_error_literal(err, GVIR_CONNECTION_ERROR,
                               0,
                               "Unable to get node info");
        return NULL;
    }

    ret = g_slice_new(GVirNodeInfo);
    g_utf8_strncpy (ret->model, info.model, sizeof (ret->model));
    ret->memory = info.memory;
    ret->cpus = info.cpus;
    ret->mhz = info.mhz;
    ret->nodes = info.nodes;
    ret->sockets = info.sockets;
    ret->cores = info.cores;
    ret->threads = info.threads;

    return ret;
}
