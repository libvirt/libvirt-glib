/*
 * libvirt-gobject-domain.c: libvirt glib integration
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

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-compat.h"
#include "libvirt-gobject/libvirt-gobject-domain-device-private.h"

#define GVIR_DOMAIN_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_DOMAIN, GVirDomainPrivate))

struct _GVirDomainPrivate
{
    virDomainPtr handle;
    gchar uuid[VIR_UUID_STRING_BUFLEN];
};

G_DEFINE_TYPE(GVirDomain, gvir_domain, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
    PROP_PERSISTENT,
};

enum {
    VIR_STARTED,
    VIR_SUSPENDED,
    VIR_RESUMED,
    VIR_STOPPED,
    VIR_UPDATED,
    LAST_SIGNAL
};

static gint signals[LAST_SIGNAL];

#define GVIR_DOMAIN_ERROR gvir_domain_error_quark()


static GQuark
gvir_domain_error_quark(void)
{
    return g_quark_from_static_string("gvir-domain");
}

static void gvir_domain_get_property(GObject *object,
                                     guint prop_id,
                                     GValue *value,
                                     GParamSpec *pspec)
{
    GVirDomain *conn = GVIR_DOMAIN(object);
    GVirDomainPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    case PROP_PERSISTENT:
        g_value_set_boolean(value, gvir_domain_get_persistent (conn));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_set_property(GObject *object,
                                     guint prop_id,
                                     const GValue *value,
                                     GParamSpec *pspec)
{
    GVirDomain *conn = GVIR_DOMAIN(object);
    GVirDomainPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virDomainFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_finalize(GObject *object)
{
    GVirDomain *conn = GVIR_DOMAIN(object);
    GVirDomainPrivate *priv = conn->priv;

    g_debug("Finalize GVirDomain=%p", conn);

    virDomainFree(priv->handle);

    G_OBJECT_CLASS(gvir_domain_parent_class)->finalize(object);
}


static void gvir_domain_constructed(GObject *object)
{
    GVirDomain *conn = GVIR_DOMAIN(object);
    GVirDomainPrivate *priv = conn->priv;

    G_OBJECT_CLASS(gvir_domain_parent_class)->constructed(object);

    /* xxx we may want to turn this into an initable */
    if (virDomainGetUUIDString(priv->handle, priv->uuid) < 0) {
        virErrorPtr verr = virGetLastError();
        if (verr) {
            g_warning("Failed to get domain UUID on %p: %s",
                      priv->handle, verr->message);
        } else {
            g_warning("Failed to get domain UUID on %p",
                      priv->handle);
        }
    }
}


static void gvir_domain_class_init(GVirDomainClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_domain_finalize;
    object_class->get_property = gvir_domain_get_property;
    object_class->set_property = gvir_domain_set_property;
    object_class->constructed = gvir_domain_constructed;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The domain handle",
                                                       GVIR_TYPE_DOMAIN_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));

    g_object_class_install_property(object_class,
                                    PROP_PERSISTENT,
                                    g_param_spec_boolean("persistent",
                                                         "Persistent",
                                                         "If domain is persistent",
                                                         TRUE,
                                                         G_PARAM_READABLE |
                                                         G_PARAM_STATIC_STRINGS));

    signals[VIR_STARTED] = g_signal_new("started",
                                        G_OBJECT_CLASS_TYPE(object_class),
                                        G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE |
                                        G_SIGNAL_NO_HOOKS | G_SIGNAL_DETAILED,
                                        G_STRUCT_OFFSET(GVirDomainClass, started),
                                        NULL, NULL,
                                        g_cclosure_marshal_VOID__VOID,
                                        G_TYPE_NONE,
                                        0);

    signals[VIR_SUSPENDED] = g_signal_new("suspended",
                                        G_OBJECT_CLASS_TYPE(object_class),
                                        G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE |
                                        G_SIGNAL_NO_HOOKS | G_SIGNAL_DETAILED,
                                        G_STRUCT_OFFSET(GVirDomainClass, suspended),
                                        NULL, NULL,
                                        g_cclosure_marshal_VOID__VOID,
                                        G_TYPE_NONE,
                                        0);

    signals[VIR_RESUMED] = g_signal_new("resumed",
                                        G_OBJECT_CLASS_TYPE(object_class),
                                        G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE |
                                        G_SIGNAL_NO_HOOKS | G_SIGNAL_DETAILED,
                                        G_STRUCT_OFFSET(GVirDomainClass, resumed),
                                        NULL, NULL,
                                        g_cclosure_marshal_VOID__VOID,
                                        G_TYPE_NONE,
                                        0);

    signals[VIR_STOPPED] = g_signal_new("stopped",
                                        G_OBJECT_CLASS_TYPE(object_class),
                                        G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE |
                                        G_SIGNAL_NO_HOOKS | G_SIGNAL_DETAILED,
                                        G_STRUCT_OFFSET(GVirDomainClass, stopped),
                                        NULL, NULL,
                                        g_cclosure_marshal_VOID__VOID,
                                        G_TYPE_NONE,
                                        0);

    signals[VIR_UPDATED] = g_signal_new("updated",
                                        G_OBJECT_CLASS_TYPE(object_class),
                                        G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                                        G_STRUCT_OFFSET(GVirDomainClass, updated),
                                        NULL, NULL,
                                        g_cclosure_marshal_VOID__VOID,
                                        G_TYPE_NONE,
                                        0);

    g_type_class_add_private(klass, sizeof(GVirDomainPrivate));
}


static void gvir_domain_init(GVirDomain *conn)
{
    g_debug("Init GVirDomain=%p", conn);

    conn->priv = GVIR_DOMAIN_GET_PRIVATE(conn);
}

typedef struct virDomain GVirDomainHandle;

static GVirDomainHandle*
gvir_domain_handle_copy(GVirDomainHandle *src)
{
    virDomainRef((virDomainPtr)src);
    return src;
}

static void
gvir_domain_handle_free(GVirDomainHandle *src)
{
    virDomainFree((virDomainPtr)src);
}

G_DEFINE_BOXED_TYPE(GVirDomainHandle, gvir_domain_handle,
                    gvir_domain_handle_copy, gvir_domain_handle_free)

static GVirDomainInfo *
gvir_domain_info_copy(GVirDomainInfo *info)
{
    return g_slice_dup(GVirDomainInfo, info);
}


static void
gvir_domain_info_free(GVirDomainInfo *info)
{
    g_slice_free(GVirDomainInfo, info);
}


G_DEFINE_BOXED_TYPE(GVirDomainInfo, gvir_domain_info,
                    gvir_domain_info_copy, gvir_domain_info_free)


const gchar *gvir_domain_get_name(GVirDomain *dom)
{
    GVirDomainPrivate *priv;
    const char *name;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), NULL);

    priv = dom->priv;
    if (!(name = virDomainGetName(priv->handle))) {
        g_warning("Failed to get domain name on %p", priv->handle);
        return NULL;
    }

    return name;
}


const gchar *gvir_domain_get_uuid(GVirDomain *dom)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), NULL);

    return dom->priv->uuid;
}

gint gvir_domain_get_id(GVirDomain *dom,
                        GError **err)
{
    GVirDomainPrivate *priv;
    gint ret;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), -1);
    g_return_val_if_fail(err == NULL || *err == NULL, -1);

    priv = dom->priv;
    if ((ret = virDomainGetID(priv->handle)) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to get ID for domain");
    }
    return ret;
}


/**
 * gvir_domain_start:
 * @dom: the domain
 * @flags:  the flags
 */
gboolean gvir_domain_start(GVirDomain *dom,
                           guint flags,
                           GError **err)
{
    GVirDomainPrivate *priv;
    int ret;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    priv = dom->priv;
    if (flags)
        ret = virDomainCreateWithFlags(priv->handle, flags);
    else
        ret = virDomainCreate(priv->handle);
    if (ret < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to start domain");
        return FALSE;
    }

    return TRUE;
}

typedef struct {
    guint flags;
} DomainStartData;

static void domain_start_data_free(DomainStartData *data)
{
    g_slice_free(DomainStartData, data);
}

static void
gvir_domain_start_helper(GSimpleAsyncResult *res,
                         GObject *object,
                         GCancellable *cancellable G_GNUC_UNUSED)
{
    GVirDomain *dom = GVIR_DOMAIN(object);
    DomainStartData *data;
    GError *err = NULL;

    data = g_simple_async_result_get_op_res_gpointer(res);

    if (!gvir_domain_start(dom, data->flags, &err))
        g_simple_async_result_take_error(res, err);
}

/**
 * gvir_domain_start_async:
 * @dom: the domain
 * @flags:  the flags
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 *
 * Asynchronous variant of #gvir_domain_start.
 */
void gvir_domain_start_async(GVirDomain *dom,
                             guint flags,
                             GCancellable *cancellable,
                             GAsyncReadyCallback callback,
                             gpointer user_data)
{
    GSimpleAsyncResult *res;
    DomainStartData *data;

    g_return_if_fail(GVIR_IS_DOMAIN(dom));
    g_return_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable));

    data = g_slice_new0(DomainStartData);
    data->flags = flags;

    res = g_simple_async_result_new(G_OBJECT(dom),
                                    callback,
                                    user_data,
                                    gvir_domain_start_async);
    g_simple_async_result_set_op_res_gpointer (res, data, (GDestroyNotify)domain_start_data_free);
    g_simple_async_result_run_in_thread(res,
                                        gvir_domain_start_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
}

gboolean gvir_domain_start_finish(GVirDomain *dom,
                                  GAsyncResult *result,
                                  GError **err)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(g_simple_async_result_is_valid(result, G_OBJECT(dom), gvir_domain_start_async), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(result), err))
        return FALSE;

    return TRUE;
}

/**
 * gvir_domain_resume:
 * @dom: the domain
 * @err: Place-holder for possible errors
 *
 * Returns: TRUE on success
 */
gboolean gvir_domain_resume(GVirDomain *dom,
                            GError **err)
{
    GVirDomainPrivate *priv;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    priv = dom->priv;
    if (virDomainResume(priv->handle) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to resume domain");
        return FALSE;
    }

    return TRUE;
}

static void
gvir_domain_resume_helper(GSimpleAsyncResult *res,
                          GObject *object,
                          GCancellable *cancellable G_GNUC_UNUSED)
{
    GVirDomain *dom = GVIR_DOMAIN(object);
    GError *err = NULL;

    if (!gvir_domain_resume(dom, &err))
        g_simple_async_result_take_error(res, err);
}

/**
 * gvir_domain_resume_async:
 * @dom: the domain to resume
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 *
 * Asynchronous variant of #gvir_domain_resume.
 */
void gvir_domain_resume_async(GVirDomain *dom,
                              GCancellable *cancellable,
                              GAsyncReadyCallback callback,
                              gpointer user_data)
{
    GSimpleAsyncResult *res;

    g_return_if_fail(GVIR_IS_DOMAIN(dom));
    g_return_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable));

    res = g_simple_async_result_new(G_OBJECT(dom),
                                    callback,
                                    user_data,
                                    gvir_domain_resume_async);
    g_simple_async_result_run_in_thread(res,
                                        gvir_domain_resume_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
}

gboolean gvir_domain_resume_finish(GVirDomain *dom,
                                   GAsyncResult *result,
                                   GError **err)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(g_simple_async_result_is_valid(result, G_OBJECT(dom), gvir_domain_resume_async), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(result), err))
        return FALSE;

    return TRUE;
}

/**
 * gvir_domain_stop:
 * @dom: the domain
 * @flags:  the flags
 */
gboolean gvir_domain_stop(GVirDomain *dom,
                          guint flags,
                          GError **err)
{
    GVirDomainPrivate *priv;
    int ret;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    priv = dom->priv;
    if (flags)
        ret = virDomainDestroyFlags(priv->handle, flags);
    else
        ret = virDomainDestroy(priv->handle);
    if (ret < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to stop domain");
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_domain_delete:
 * @dom: the domain
 * @flags:  the flags
 */
gboolean gvir_domain_delete(GVirDomain *dom,
                            guint flags,
                            GError **err)
{
    GVirDomainPrivate *priv;
    int ret;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    priv = dom->priv;
    if (flags)
        ret = virDomainUndefineFlags(priv->handle, flags);
    else
        ret = virDomainUndefine(priv->handle);
    if (ret < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to delete domain");
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_domain_shutdown:
 * @dom: the domain
 * @flags:  the %GVirDomainShutdownFlags flags
 */
gboolean gvir_domain_shutdown(GVirDomain *dom,
                              guint flags,
                              GError **err)
{
    GVirDomainPrivate *priv;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    priv = dom->priv;
    if (virDomainShutdownFlags(priv->handle, flags) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to shutdown domain");
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_domain_reboot:
 * @dom: the domain
 * @flags:  the flags
 */
gboolean gvir_domain_reboot(GVirDomain *dom,
                            guint flags,
                            GError **err)
{
    GVirDomainPrivate *priv;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    priv = dom->priv;
    if (virDomainReboot(priv->handle, flags) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to reboot domain");
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_domain_save_to_file:
 * @dom: the domain
 * @filename: path to the output file
 * @custom_conf: (allow-none): configuration for domain or NULL
 * @flags: the flags
 *
 * Returns: TRUE on success, FALSE otherwise
 */
gboolean gvir_domain_save_to_file(GVirDomain *dom,
                                  gchar *filename,
                                  GVirConfigDomain *custom_conf,
                                  guint flags,
                                  GError **err)
{
    GVirDomainPrivate *priv;
    int ret;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(filename != NULL, FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    priv = dom->priv;

    if (flags || custom_conf != NULL) {
        gchar *custom_xml = NULL;

        if (custom_conf != NULL)
            custom_xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(custom_conf));

        ret = virDomainSaveFlags(priv->handle, filename, custom_xml, flags);
        g_free(custom_xml);
    }
    else {
        ret = virDomainSave(priv->handle, filename);
    }

    if (ret < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to save domain to file");
        return FALSE;
    }

    return TRUE;
}

typedef struct {
    gchar *filename;
    GVirConfigDomain *custom_conf;
    guint flags;
} DomainSaveToFileData;

static void domain_save_to_file_data_free(DomainSaveToFileData *data)
{
    g_free(data->filename);
    g_clear_object(&data->custom_conf);
    g_slice_free(DomainSaveToFileData, data);
}

static void
gvir_domain_save_to_file_helper(GSimpleAsyncResult *res,
                                GObject *object,
                                GCancellable *cancellable G_GNUC_UNUSED)
{
    GVirDomain *dom = GVIR_DOMAIN(object);
    DomainSaveToFileData *data;
    GError *err = NULL;

    data = g_simple_async_result_get_op_res_gpointer(res);

    if (!gvir_domain_save_to_file(dom, data->filename, data->custom_conf, data->flags, &err))
        g_simple_async_result_take_error(res, err);
}

/**
 * gvir_domain_save_to_file_async:
 * @dom: the domain
 * @filename: path to output file
 * @custom_conf: (allow-none): configuration for domain or NULL
 * @flags: the flags
 * @cancellable: (allow-none) (transfer none): cancallation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 *
 * Asynchronous variant of #gvir_domain_save_to_file
 */
void gvir_domain_save_to_file_async(GVirDomain *dom,
                                    gchar *filename,
                                    GVirConfigDomain *custom_conf,
                                    guint flags,
                                    GCancellable *cancellable,
                                    GAsyncReadyCallback callback,
                                    gpointer user_data)
{
    GSimpleAsyncResult *res;
    DomainSaveToFileData *data;

    g_return_if_fail(GVIR_IS_DOMAIN(dom));
    g_return_if_fail(filename != NULL);
    g_return_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable));

    data = g_slice_new0(DomainSaveToFileData);
    data->filename = g_strdup(filename);
    if (custom_conf != NULL)
        data->custom_conf = g_object_ref(custom_conf);
    data->flags = flags;

    res = g_simple_async_result_new(G_OBJECT(dom),
                                    callback,
                                    user_data,
                                    gvir_domain_save_to_file_async);
    g_simple_async_result_set_op_res_gpointer(res, data, (GDestroyNotify)
                                              domain_save_to_file_data_free);

    g_simple_async_result_run_in_thread(res,
                                        gvir_domain_save_to_file_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);

    g_object_unref(res);
}

/**
 * gvir_domain_save_to_file_finish:
 * @dom: the domain to save
 * @result: (transfer none): async method result
 * @err: Place-holder for possible errors
 *
 * Finishes the operation started by #gvir_domain_save_to_file_async.
 *
 * Returns: TRUE if domain was saved successfully, FALSE otherwise.
 */
gboolean gvir_domain_save_to_file_finish(GVirDomain *dom,
                                         GAsyncResult *result,
                                         GError **err)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(g_simple_async_result_is_valid
                                  (result,
                                   G_OBJECT(dom),
                                   gvir_domain_save_to_file_async), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(result), err))
        return FALSE;

    return TRUE;
}

/**
 * gvir_domain_get_config:
 * @dom: the domain
 * @flags: the %GVirDomainXMLFlags flags
 *
 * Returns: (transfer full): the config. The returned object should be
 * unreffed with g_object_unref() when no longer needed.
 */
GVirConfigDomain *gvir_domain_get_config(GVirDomain *dom,
                                         guint flags,
                                         GError **err)
{
    GVirDomainPrivate *priv;
    gchar *xml;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = dom->priv;
    if (!(xml = virDomainGetXMLDesc(priv->handle, flags))) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to get domain XML config");
        return NULL;
    }

    GVirConfigDomain *conf = gvir_config_domain_new_from_xml(xml, err);
    g_free(xml);
    if ((err != NULL) && (*err != NULL))
        return NULL;

    return conf;
}

/**
 * gvir_domain_set_config:
 * @domain: the domain
 * @conf: the new configuration for the domain
 * @err: (allow-none): Place-holder for error or NULL
 *
 * Resets configuration of an existing domain.
 *
 * Note: If domain is already running, the new configuration will not take
 * affect until domain reboots.
 *
 * Returns: TRUE on success, FALSE if an error occurred.
 */
gboolean gvir_domain_set_config(GVirDomain *domain,
                                GVirConfigDomain *conf,
                                GError **err)
{
    gchar *xml;
    virConnectPtr conn;
    virDomainPtr handle;
    gchar uuid[VIR_UUID_STRING_BUFLEN];
    GVirDomainPrivate *priv;

    g_return_val_if_fail(GVIR_IS_DOMAIN (domain), FALSE);
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN (conf), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    priv = domain->priv;
    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(conf));

    g_return_val_if_fail(xml != NULL, FALSE);

    if ((conn = virDomainGetConnect(priv->handle)) == NULL) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Failed to get domain connection");
        g_free (xml);

        return FALSE;
    }

    handle = virDomainDefineXML(conn, xml);
    g_free (xml);

    if (handle == NULL) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Failed to set "
                               "domain configuration");
        return FALSE;
    }

    virDomainGetUUIDString(handle, uuid);
    virDomainFree(handle);

    if (g_strcmp0 (uuid, priv->uuid) != 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Failed to set "
                               "domain configuration");

        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_domain_get_info:
 * @dom: the domain
 *
 * Returns: (transfer full): the info. The returned object should be
 * unreffed with g_object_unref() when no longer needed.
 */
GVirDomainInfo *gvir_domain_get_info(GVirDomain *dom,
                                     GError **err)
{
    GVirDomainPrivate *priv;
    virDomainInfo info;
    GVirDomainInfo *ret;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = dom->priv;
    if (virDomainGetInfo(priv->handle, &info) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to get domain info");
        return NULL;
    }

    ret = g_slice_new(GVirDomainInfo);
    ret->state = info.state;
    ret->maxMem = info.maxMem;
    ret->memory = info.memory;
    ret->nrVirtCpu = info.nrVirtCpu;
    ret->cpuTime = info.cpuTime;

    return ret;
}

static void
gvir_domain_get_info_helper(GSimpleAsyncResult *res,
                            GObject *object,
                            GCancellable *cancellable G_GNUC_UNUSED)
{
    GVirDomain *dom = GVIR_DOMAIN(object);
    GVirDomainInfo *info;
    GError *err = NULL;

    info = gvir_domain_get_info(dom, &err);
    if (err)
        g_simple_async_result_take_error(res, err);
    else
        g_simple_async_result_set_op_res_gpointer
                                (res,
                                 info,
                                 (GDestroyNotify) gvir_domain_info_free);
}

/**
 * gvir_domain_get_info_async:
 * @dom: the domain
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 *
 * Asynchronous variant of #gvir_domain_get_info.
 */
void gvir_domain_get_info_async(GVirDomain *dom,
                                GCancellable *cancellable,
                                GAsyncReadyCallback callback,
                                gpointer user_data)
{
    GSimpleAsyncResult *res;

    g_return_if_fail(GVIR_IS_DOMAIN(dom));
    g_return_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable));

    res = g_simple_async_result_new(G_OBJECT(dom),
                                    callback,
                                    user_data,
                                    gvir_domain_get_info_async);
    g_simple_async_result_run_in_thread(res,
                                        gvir_domain_get_info_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
}

/**
 * gvir_domain_get_info_finish:
 * @dom: the domain
 * @result: (transfer none): async method result
 * @err: Place-holder for possible errors
 *
 * Finishes the operation started by #gvir_domain_get_info_async.
 *
 * Returns: (transfer full): the info. The returned object should be
 * unreffed with g_object_unref() when no longer needed.
 */
GVirDomainInfo *gvir_domain_get_info_finish(GVirDomain *dom,
                                            GAsyncResult *result,
                                            GError **err)
{
    GSimpleAsyncResult *res = G_SIMPLE_ASYNC_RESULT(result);
    GVirDomainInfo *ret;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), NULL);
    g_return_val_if_fail
                (g_simple_async_result_is_valid(result,
                                                G_OBJECT(dom),
                                                gvir_domain_get_info_async),
                 NULL);

    if (g_simple_async_result_propagate_error(res, err))
        return NULL;

    ret = g_simple_async_result_get_op_res_gpointer(res);

    return gvir_domain_info_copy (ret);
}

/**
 * gvir_domain_screenshot:
 * @stream: stream to use as output
 * @monitor_id: monitor ID to take screenshot from
 * @flags: extra flags, currently unused
 *
 * Returns: (transfer full): a newly allocated string containing the
 * mime-type of the image format, or NULL upon error.
 */
gchar *gvir_domain_screenshot(GVirDomain *dom,
                              GVirStream *stream,
                              guint monitor_id,
                              guint flags,
                              GError **err)
{
    GVirDomainPrivate *priv;
    virStreamPtr st = NULL;
    gchar *mime = NULL;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), NULL);
    g_return_val_if_fail(GVIR_IS_STREAM(stream), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = dom->priv;
    g_object_get(stream, "handle", &st, NULL);

    if (!(mime = virDomainScreenshot(priv->handle,
                                     st,
                                     monitor_id,
                                     flags))) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to take a screenshot");
        goto end;
    }
end:
    if (st != NULL)
        virStreamFree(st);
    return mime;
}


/**
 * gvir_domain_open_console:
 * @dom: (transfer none): the domain
 * @devname: (transfer none)(allow-none): the device name
 * @stream: (transfer none): stream to use as output
 * @flags: extra flags, currently unused
 *
 * Open a text console for the domain @dom, connecting it to the
 * stream @stream. If @devname is NULL, the default console will
 * be opened, otherwise @devname can be used to specify a non-default
 * console device.
 *
 * Returns: TRUE if the console was opened, FALSE otherwise.
 */
gboolean gvir_domain_open_console(GVirDomain *dom,
                                  GVirStream *stream,
                                  const gchar *devname,
                                  guint flags,
                                  GError **err)
{
    GVirDomainPrivate *priv;
    virStreamPtr st = NULL;
    gboolean ret = FALSE;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(GVIR_IS_STREAM(stream), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    priv = dom->priv;
    g_object_get(stream, "handle", &st, NULL);

    if (virDomainOpenConsole(priv->handle,
                             devname,
                             st,
                             flags) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to open console");
        goto cleanup;
    }

    ret = TRUE;
cleanup:
    if (st != NULL)
        virStreamFree(st);
    return ret;
}


/**
 * gvir_domain_open_graphics:
 * @dom: the domain
 * @idx: the graphics index
 * @fd: pre-opened socket pair
 * @flags: extra flags, currently unused
 *
 * Open a connection to the local graphics display, connecting it to the
 * socket pair file descriptor passed in as @fd.
 *
 * Returns: TRUE if the graphics connection was opened, FALSE otherwise.
 */
gboolean gvir_domain_open_graphics(GVirDomain *dom,
                                   guint idx,
                                   int fd,
                                   unsigned int flags,
                                   GError **err)
{
    GVirDomainPrivate *priv;
    gboolean ret = FALSE;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    priv = dom->priv;

    if (virDomainOpenGraphics(priv->handle,
                              idx,
                              fd,
                              flags) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to open graphics");
        goto cleanup;
    }

    ret = TRUE;
cleanup:
    return ret;
}

/**
 * gvir_domain_suspend:
 * @dom: the domain to suspend
 * @err: Place-holder for possible errors
 *
 * Suspends an active domain, the process is frozen without further access to
 * CPU resources and I/O but the memory used by the domain at the hypervisor
 * level will stay allocated. Use gvir_domain_resume() to reactivate the domain.
 *
 * Returns: TRUE if domain was suspended successfully, FALSE otherwise.
 */
gboolean gvir_domain_suspend (GVirDomain *dom,
                              GError **err)
{
    gboolean ret = FALSE;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virDomainSuspend(dom->priv->handle) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to suspend domain");
        goto cleanup;
    }

    ret = TRUE;
cleanup:
    return ret;
}

/**
 * gvir_domain_save:
 * @dom: the domain to save
 * @flags: extra flags, currently unused
 * @err: Place-holder for possible errors
 *
 * Saves the state of the domain on disk and stops it. Use #gvir_domain_start
 * to restore the saved state of the domain. A saved domain can be restored
 * even after shutdown/reboot of host machine.
 *
 * Returns: TRUE if domain was saved successfully, FALSE otherwise.
 */
gboolean gvir_domain_save (GVirDomain *dom,
                           unsigned int flags,
                           GError **err)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (virDomainManagedSave(dom->priv->handle, flags) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_ERROR,
                               0,
                               "Unable to save domain");
        return FALSE;
    }

    return TRUE;
}

typedef struct {
    guint flags;
} DomainSaveData;

static void domain_save_data_free(DomainSaveData *data)
{
    g_slice_free (DomainSaveData, data);
}

static void
gvir_domain_save_helper(GSimpleAsyncResult *res,
                        GObject *object,
                        GCancellable *cancellable G_GNUC_UNUSED)
{
    GVirDomain *dom = GVIR_DOMAIN(object);
    DomainSaveData *data;
    GError *err = NULL;

    data = g_simple_async_result_get_op_res_gpointer (res);

    if (!gvir_domain_save(dom, data->flags, &err))
        g_simple_async_result_take_error(res, err);
}

/**
 * gvir_domain_save_async:
 * @dom: the domain to save
 * @flags: extra flags, currently unused
 * @cancellable: (allow-none)(transfer none): cancellation object
 * @callback: (scope async): completion callback
 * @user_data: (closure): opaque data for callback
 *
 * Asynchronous variant of #gvir_domain_save.
 */
void gvir_domain_save_async (GVirDomain *dom,
                             unsigned int flags,
                             GCancellable *cancellable,
                             GAsyncReadyCallback callback,
                             gpointer user_data)
{
    GSimpleAsyncResult *res;
    DomainSaveData *data;

    g_return_if_fail(GVIR_IS_DOMAIN(dom));
    g_return_if_fail((cancellable == NULL) || G_IS_CANCELLABLE(cancellable));

    data = g_slice_new0(DomainSaveData);
    data->flags = flags;

    res = g_simple_async_result_new(G_OBJECT(dom),
                                    callback,
                                    user_data,
                                    gvir_domain_save_async);
    g_simple_async_result_set_op_res_gpointer (res, data, (GDestroyNotify) domain_save_data_free);
    g_simple_async_result_run_in_thread(res,
                                        gvir_domain_save_helper,
                                        G_PRIORITY_DEFAULT,
                                        cancellable);
    g_object_unref(res);
}

/**
 * gvir_domain_save_finish:
 * @dom: the domain to save
 * @result: (transfer none): async method result
 * @err: Place-holder for possible errors
 *
 * Finishes the operation started by #gvir_domain_save_async.
 *
 * Returns: TRUE if domain was saved successfully, FALSE otherwise.
 */
gboolean gvir_domain_save_finish (GVirDomain *dom,
                                  GAsyncResult *result,
                                  GError **err)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(g_simple_async_result_is_valid(result, G_OBJECT(dom),
                                                        gvir_domain_save_async),
                         FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    if (g_simple_async_result_propagate_error(G_SIMPLE_ASYNC_RESULT(result), err))
        return FALSE;

    return TRUE;
}

/**
 * gvir_domain_get_persistent:
 * @dom: the domain
 *
 * Returns: TRUE if domain is persistent, FALSE otherwise.
 */
gboolean gvir_domain_get_persistent(GVirDomain *dom)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);

    return virDomainIsPersistent(dom->priv->handle) == 1;
}

/**
 * gvir_domain_get_saved:
 * @dom: the domain
 *
 * Returns: TRUE if a stopped domain has a saved state to which it can be
 * restored to using #gvir_domain_start, FALSE otherwise.
 */
gboolean gvir_domain_get_saved(GVirDomain *dom)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);

    return virDomainHasManagedSaveImage(dom->priv->handle, 0) == 1;
}

/**
 * gvir_domain_get_devices:
 * @domain: the domain
 * @err: place-holder for possible errors, or NULL
 *
 * Gets the list of devices attached to @domain. The returned list should
 * be freed with g_list_free(), after its elements have been unreffed with
 * g_object_unref().
 *
 * Returns: (element-type LibvirtGObject.DomainDevice) (transfer full): a newly
 * allocated #GList of #GVirDomainDevice.
 */
GList *gvir_domain_get_devices(GVirDomain *domain,
                               GError **err)
{
    GVirConfigDomain *config;
    GList *config_devices;
    GList *node;
    GList *ret = NULL;

    g_return_val_if_fail(GVIR_IS_DOMAIN(domain), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    config = gvir_domain_get_config(domain, 0, err);
    if (config == NULL)
        return NULL;

    config_devices = gvir_config_domain_get_devices(config);
    for (node = config_devices; node != NULL; node = node->next) {
        GVirConfigDomainDevice *device_config;
        GVirDomainDevice *device;

        device_config = GVIR_CONFIG_DOMAIN_DEVICE(node->data);
        device = gvir_domain_device_new(domain, device_config);
        if (device != NULL)
             ret = g_list_prepend(ret, device);

        g_object_unref (device_config);
    }
    g_list_free (config_devices);

    return g_list_reverse (ret);
}

/**
 * gvir_domain_create_snapshot:
 * @dom: the domain
 * @custom_conf: (allow-none): configuration of snapshot or NULL
 * @flags: bitwise-OR of #GVirDomainSnapshotCreateFlags
 * @err: (allow-none):Place-holder for error or NULL
 *
 * Returns: (transfer full): snapshot of domain. The returned object should be
 * unreffed when no longer needed
 */
GVirDomainSnapshot *
gvir_domain_create_snapshot(GVirDomain *dom,
                             GVirConfigDomainSnapshot *custom_conf,
                             guint flags,
                             GError **err)
{
    GVirDomainPrivate *priv;
    virDomainSnapshot *snapshot;
    GVirDomainSnapshot *dom_snapshot;
    gchar *custom_xml = NULL;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = dom->priv;

    if (custom_conf != NULL)
        custom_xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(custom_conf));

    if (!(snapshot = virDomainSnapshotCreateXML(priv->handle,
                                                custom_xml,
                                                flags))) {
        const gchar *domain_name = NULL;
        domain_name = gvir_domain_get_name(dom);

        gvir_set_error(err, GVIR_DOMAIN_ERROR,
                       0,
                      "Unable to create snapshot of %s", domain_name);

        g_free(custom_xml);
        return NULL;
    }

    dom_snapshot = GVIR_DOMAIN_SNAPSHOT(g_object_new(GVIR_TYPE_DOMAIN_SNAPSHOT,
                                                     "handle",
                                                     snapshot,
                                                     NULL));

    g_free(custom_xml);
    return dom_snapshot;
}
