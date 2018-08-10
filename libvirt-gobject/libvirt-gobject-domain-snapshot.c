/*
 * libvirt-gobject-domain_snapshot.c: libvirt glib integration
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-compat.h"

#define GVIR_DOMAIN_SNAPSHOT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_DOMAIN_SNAPSHOT, GVirDomainSnapshotPrivate))

struct _GVirDomainSnapshotPrivate
{
    virDomainSnapshotPtr handle;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirDomainSnapshot, gvir_domain_snapshot, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_DOMAIN_SNAPSHOT_ERROR gvir_domain_snapshot_error_quark()


static GQuark
gvir_domain_snapshot_error_quark(void)
{
    return g_quark_from_static_string("gvir-domain-snapshot");
}

static void gvir_domain_snapshot_get_property(GObject *object,
                                              guint prop_id,
                                              GValue *value,
                                              GParamSpec *pspec)
{
    GVirDomainSnapshot *snapshot = GVIR_DOMAIN_SNAPSHOT(object);
    GVirDomainSnapshotPrivate *priv = snapshot->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_snapshot_set_property(GObject *object,
                                              guint prop_id,
                                              const GValue *value,
                                              GParamSpec *pspec)
{
    GVirDomainSnapshot *snapshot = GVIR_DOMAIN_SNAPSHOT(object);
    GVirDomainSnapshotPrivate *priv = snapshot->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virDomainSnapshotFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_snapshot_finalize(GObject *object)
{
    GVirDomainSnapshot *snapshot = GVIR_DOMAIN_SNAPSHOT(object);
    GVirDomainSnapshotPrivate *priv = snapshot->priv;

    virDomainSnapshotFree(priv->handle);

    G_OBJECT_CLASS(gvir_domain_snapshot_parent_class)->finalize(object);
}


static void gvir_domain_snapshot_class_init(GVirDomainSnapshotClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_domain_snapshot_finalize;
    object_class->get_property = gvir_domain_snapshot_get_property;
    object_class->set_property = gvir_domain_snapshot_set_property;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The domain_snapshot handle",
                                                       GVIR_TYPE_DOMAIN_SNAPSHOT_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));
}


static void gvir_domain_snapshot_init(GVirDomainSnapshot *snapshot)
{
    snapshot->priv = GVIR_DOMAIN_SNAPSHOT_GET_PRIVATE(snapshot);
}

typedef struct virDomainSnapshot GVirDomainSnapshotHandle;

static GVirDomainSnapshotHandle*
gvir_domain_snapshot_handle_copy(GVirDomainSnapshotHandle *src)
{
    virDomainSnapshotRef((virDomainSnapshotPtr)src);
    return src;
}

static void
gvir_domain_snapshot_handle_free(GVirDomainSnapshotHandle *src)
{
    virDomainSnapshotFree((virDomainSnapshotPtr)src);
}

G_DEFINE_BOXED_TYPE(GVirDomainSnapshotHandle, gvir_domain_snapshot_handle,
                    gvir_domain_snapshot_handle_copy, gvir_domain_snapshot_handle_free)

const gchar *gvir_domain_snapshot_get_name(GVirDomainSnapshot *snapshot)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN_SNAPSHOT(snapshot), NULL);

    GVirDomainSnapshotPrivate *priv = snapshot->priv;
    const char *name;

    if (!(name = virDomainSnapshotGetName(priv->handle))) {
        gvir_warning("Failed to get domain_snapshot name on %p", priv->handle);
        return NULL;
    }

    return name;
}


/**
 * gvir_domain_snapshot_get_config:
 * @snapshot: the domain_snapshot
 * @flags: the flags
 *
 * Returns: (transfer full): the config. The returned object should be
 * unreffed with g_object_unref() when no longer needed.

 */
GVirConfigDomainSnapshot *gvir_domain_snapshot_get_config
                                (GVirDomainSnapshot *snapshot,
                                 guint flags,
                                 GError **err)
{
    GVirDomainSnapshotPrivate *priv;
    gchar *xml;

    g_return_val_if_fail(GVIR_IS_DOMAIN_SNAPSHOT(snapshot), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = snapshot->priv;
    if (!(xml = virDomainSnapshotGetXMLDesc(priv->handle, flags))) {
        gvir_set_error_literal(err, GVIR_DOMAIN_SNAPSHOT_ERROR,
                               0,
                               "Unable to get domain_snapshot XML config");
        return NULL;
    }

    GVirConfigDomainSnapshot *conf = gvir_config_domain_snapshot_new_from_xml(xml, err);

    free(xml);
    return conf;
}

/**
 * gvir_domain_snapshot_delete:
 * @snapshot: The domain snapshot
 * @flags: Bitwise or of #GVirDomainSnapshotDeleteFlags
 * @error: (allow-none): Place-holder for error or NULL
 *
 * Returns: TRUE on success, FALSE otherwise
 */
gboolean gvir_domain_snapshot_delete (GVirDomainSnapshot *snapshot,
                                      guint flags,
                                      GError **error)
{
    GVirDomainSnapshotPrivate *priv;
    int status;

    g_return_val_if_fail(GVIR_IS_DOMAIN_SNAPSHOT (snapshot), FALSE);
    g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

    priv = snapshot->priv;
    status = virDomainSnapshotDelete(priv->handle, flags);
    if (status < 0) {
        gvir_set_error(error, GVIR_DOMAIN_SNAPSHOT_ERROR, 0,
                       "Unable to delete snapshot `%s'",
                       gvir_domain_snapshot_get_name(snapshot));
        return FALSE;
    }
    return TRUE;
}



static void _delete_async_thread(GTask *task,
                                 gpointer source_object,
                                 gpointer task_data,
                                 GCancellable *cancellable G_GNUC_UNUSED)
{
    GError *error = NULL;
    gboolean status;

    status = gvir_domain_snapshot_delete(source_object,
                                         GPOINTER_TO_UINT(task_data),
                                         &error);

    if (status)
        g_task_return_boolean(task, TRUE);
    else
        g_task_return_error(task, error);
}


/**
 * gvir_domain_snapshot_delete_async:
 * @snapshot: A #GVirDomainSnapshot
 * @flags: Bitwise-OR of #GVirDomainSnapshotDeleteFlags
 * @cancellable: (allow-none) (transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 */
void gvir_domain_snapshot_delete_async(GVirDomainSnapshot *snapshot,
                                       guint flags,
                                       GCancellable *cancellable,
                                       GAsyncReadyCallback callback,
                                       gpointer user_data)
{
    GTask *task;

    g_return_if_fail(GVIR_IS_DOMAIN_SNAPSHOT(snapshot));

    task = g_task_new(snapshot, cancellable, callback, user_data);
    g_task_set_task_data(task, GUINT_TO_POINTER(flags), NULL);
    g_task_run_in_thread(task, _delete_async_thread);
    g_object_unref(task);
}

/**
 * gvir_domain_snapshot_delete_finish:
 * @snapshot: A #GVirDomainSnapshot
 * @res: (transfer none): async method result
 *
 * Returns: %TRUE on success, %FALSE otherwise.
 */
gboolean gvir_domain_snapshot_delete_finish(GVirDomainSnapshot *snapshot,
                                            GAsyncResult *res,
                                            GError **error)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN_SNAPSHOT(snapshot), FALSE);
    g_return_val_if_fail(g_task_is_valid(res, snapshot), FALSE);

    return g_task_propagate_boolean(G_TASK(res), error);
}

/**
 * gvir_domain_snapshot_get_is_current:
 * @snapshot: The domain snapshot
 * @flags: Currently unused, pass 0
 * @is_current: (out): %TRUE if the given snapshot is the current snapshot
 * of its domain, %FALSE otherwise.
 * @error: (allow-none): Place-holder for error or %NULL
 *
 * Returns: %TRUE on success, %FALSE otherwise.
 */
gboolean gvir_domain_snapshot_get_is_current(GVirDomainSnapshot *snapshot,
                                             guint flags,
                                             gboolean *is_current,
                                             GError **error)
{
    gint status;

    g_return_val_if_fail(GVIR_IS_DOMAIN_SNAPSHOT(snapshot), FALSE);
    g_return_val_if_fail(is_current != NULL, FALSE);
    g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

    status = virDomainSnapshotIsCurrent(snapshot->priv->handle, flags);
    if (status == -1) {
        gvir_set_error(error, GVIR_DOMAIN_SNAPSHOT_ERROR, 0,
                       "Could not determine if `%s' is the current snapshot",
                       gvir_domain_snapshot_get_name(snapshot));
        return FALSE;
    }

    *is_current = status;

    return TRUE;
}



/**
 * gvir_domain_snapshot_revert_to:
 * @snapshot: The domain snapshot
 * @flags: Bitwise OR of GVirDomainSnapshotRevertFlags
 * @error: (allow-none): Place-holder for error or NULL
 *
 * Returns: TRUE if the snapshot's domain has successfully been
 * reverted to the given snapshot, FALSE otherwise, in which case
 * @error will be set.
 */
gboolean gvir_domain_snapshot_revert_to(GVirDomainSnapshot *snapshot,
                                        guint flags,
                                        GError **error)
{
    int status;

    g_return_val_if_fail(GVIR_IS_DOMAIN_SNAPSHOT(snapshot), FALSE);
    g_return_val_if_fail((error == NULL) || (*error == NULL), FALSE);


    status = virDomainRevertToSnapshot(snapshot->priv->handle,
                                       flags);
    if (status != 0) {
        gvir_set_error(error, GVIR_DOMAIN_SNAPSHOT_ERROR,
                       0, "Failed to revert to snapshot `%s'",
                       gvir_domain_snapshot_get_name(snapshot));
        return FALSE;
    }

    return TRUE;
}

static void _revert_to_async_thread(GTask *task,
                                    gpointer source_object,
                                    gpointer task_data,
                                    GCancellable *cancellable G_GNUC_UNUSED)
{
    GError *error = NULL;
    gboolean status;

    status = gvir_domain_snapshot_revert_to(source_object,
                                            GPOINTER_TO_UINT(task_data),
                                            &error);

    if (status)
        g_task_return_boolean(task, TRUE);
    else
        g_task_return_error(task, error);
}


/**
 * gvir_domain_snapshot_revert_to_async:
 * @snapshot: A #GVirDomainSnapshot
 * @flags: Bitwise OR of #GVirDomainSnapshotRevertFlags
 * @cancellable: (allow-none) (transfer none): cancellation object
 * @callback: (scope async): The callback
 * @user_data: (closure): Opaque data for callback
 */
void gvir_domain_snapshot_revert_to_async(GVirDomainSnapshot *snapshot,
                                          guint flags,
                                          GCancellable *cancellable,
                                          GAsyncReadyCallback callback,
                                          gpointer user_data)
{
    GTask *task;

    g_return_if_fail(GVIR_IS_DOMAIN_SNAPSHOT(snapshot));

    task = g_task_new(snapshot, cancellable, callback, user_data);
    g_task_set_task_data(task, GUINT_TO_POINTER(flags), NULL);
    g_task_run_in_thread(task, _revert_to_async_thread);
    g_object_unref(task);
}



/**
 * gvir_domain_snapshot_revert_to_finish:
 * @snapshot: The domain snapshot
 * @result: (transfer none): The result
 *
 * Returns: %TRUE on success, %FALSE otherwise.
 */
gboolean gvir_domain_snapshot_revert_to_finish(GVirDomainSnapshot *snapshot,
                                               GAsyncResult *result,
                                               GError **error)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN_SNAPSHOT(snapshot), FALSE);
    g_return_val_if_fail(g_task_is_valid(result, snapshot), FALSE);

    return g_task_propagate_boolean(G_TASK(result), error);
}



/**
 * gvir_domain_snapshot_set_config:
 * @snapshot: The domain snapshot
 * @conf: The new config object
 * @error: (allow-none): Place-holder for error or %NULL
 *
 * Updates the given snapshot's configuration according to the
 * given GVirConfigDomainSnapshot.
 *
 * Returns: %TRUE if no error was reported, %FALSE otherwise.
 */
gboolean gvir_domain_snapshot_set_config(GVirDomainSnapshot *snapshot,
                                         GVirConfigDomainSnapshot *conf,
                                         GError **error)
{
    gchar *xml;
    virConnectPtr conn;
    virDomainSnapshotPtr handle;
    virDomainPtr domain;
    GVirDomainSnapshotPrivate *priv;

    g_return_val_if_fail(GVIR_IS_DOMAIN_SNAPSHOT(snapshot), FALSE);
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(conf), FALSE);
    g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

    priv = snapshot->priv;
    handle = priv->handle;
    domain = virDomainSnapshotGetDomain(handle);


    if ((conn = virDomainSnapshotGetConnect(priv->handle)) == NULL) {
        gvir_set_error_literal(error, GVIR_DOMAIN_SNAPSHOT_ERROR,
                               0,
                               "Failed to get domain connection");
        return FALSE;
    }


    /* XXX Changing the name will create a new snapshot */
    if (g_strcmp0 (gvir_domain_snapshot_get_name(snapshot),
                   gvir_config_domain_snapshot_get_name(conf)) != 0) {
        gvir_set_error_literal(error, GVIR_DOMAIN_SNAPSHOT_ERROR,
                               0,
                               "Cannot set config: snapshot names don't match");
        return FALSE;
    }


    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(conf));

    handle = virDomainSnapshotCreateXML(domain,
                                        xml,
                                        VIR_DOMAIN_SNAPSHOT_CREATE_REDEFINE);
    g_free(xml);

    if (handle == NULL) {
        gvir_set_error(error, GVIR_DOMAIN_SNAPSHOT_ERROR,
                       0,
                       "Failed to create snapshot `%s' from XML definition",
                       gvir_domain_snapshot_get_name(snapshot));
        return FALSE;
    }
    virDomainSnapshotFree(handle);
    return TRUE;
}
