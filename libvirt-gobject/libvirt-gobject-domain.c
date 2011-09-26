/*
 * libvirt-gobject-domain.c: libvirt glib integration
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

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_DOMAIN_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_DOMAIN, GVirDomainPrivate))

struct _GVirDomainPrivate
{
    virDomainPtr handle;
};

G_DEFINE_TYPE(GVirDomain, gvir_domain, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


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

    DEBUG("Finalize GVirDomain=%p", conn);

    virDomainFree(priv->handle);

    G_OBJECT_CLASS(gvir_domain_parent_class)->finalize(object);
}


static void gvir_domain_class_init(GVirDomainClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_domain_finalize;
    object_class->get_property = gvir_domain_get_property;
    object_class->set_property = gvir_domain_set_property;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The domain handle",
                                                       GVIR_TYPE_DOMAIN_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_NAME |
                                                       G_PARAM_STATIC_NICK |
                                                       G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirDomainPrivate));
}


static void gvir_domain_init(GVirDomain *conn)
{
    GVirDomainPrivate *priv;

    DEBUG("Init GVirDomain=%p", conn);

    priv = conn->priv = GVIR_DOMAIN_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}

static gpointer
gvir_domain_handle_copy(gpointer src)
{
    virDomainRef(src);
    return src;
}


GType gvir_domain_handle_get_type(void)
{
    static GType handle_type = 0;

    if (G_UNLIKELY(handle_type == 0))
        handle_type = g_boxed_type_register_static
            ("GVirDomainHandle",
             gvir_domain_handle_copy,
             (GBoxedFreeFunc)virDomainFree);

    return handle_type;
}

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


GType gvir_domain_info_get_type(void)
{
    static GType info_type = 0;

    if (G_UNLIKELY(info_type == 0))
        info_type = g_boxed_type_register_static
            ("GVirDomainInfo",
             (GBoxedCopyFunc)gvir_domain_info_copy,
             (GBoxedFreeFunc)gvir_domain_info_free);

    return info_type;
}


const gchar *gvir_domain_get_name(GVirDomain *dom)
{
    GVirDomainPrivate *priv = dom->priv;
    const char *name;

    if (!(name = virDomainGetName(priv->handle))) {
        g_error("Failed to get domain name on %p", priv->handle);
    }

    return name;
}


gchar *gvir_domain_get_uuid(GVirDomain *dom)
{
    GVirDomainPrivate *priv = dom->priv;
    char *uuid = g_new(gchar, VIR_UUID_STRING_BUFLEN);

    if (virDomainGetUUIDString(priv->handle, uuid) < 0) {
        g_error("Failed to get domain UUID on %p", priv->handle);
    }
    return uuid;
}

gint gvir_domain_get_id(GVirDomain *dom,
                        GError **err)
{
    GVirDomainPrivate *priv = dom->priv;
    gint ret;

    if ((ret = virDomainGetID(priv->handle)) < 0) {
        *err = gvir_error_new_literal(GVIR_DOMAIN_ERROR,
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
                           guint64 flags G_GNUC_UNUSED,
                           GError **err)
{
    GVirDomainPrivate *priv = dom->priv;

    if (virDomainCreate(priv->handle) < 0) {
        *err = gvir_error_new_literal(GVIR_DOMAIN_ERROR,
                                      0,
                                      "Unable to start domain");
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_domain_stop:
 * @dom: the domain
 * @flags:  the flags
 */
gboolean gvir_domain_stop(GVirDomain *dom,
                          guint64 flags G_GNUC_UNUSED,
                          GError **err)
{
    GVirDomainPrivate *priv = dom->priv;

    if (virDomainDestroy(priv->handle) < 0) {
        *err = gvir_error_new_literal(GVIR_DOMAIN_ERROR,
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
                            guint64 flags G_GNUC_UNUSED,
                            GError **err)
{
    GVirDomainPrivate *priv = dom->priv;

    if (virDomainUndefine(priv->handle) < 0) {
        *err = gvir_error_new_literal(GVIR_DOMAIN_ERROR,
                                      0,
                                      "Unable to delete domain");
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_domain_shutdown:
 * @dom: the domain
 * @flags:  the flags
 */
gboolean gvir_domain_shutdown(GVirDomain *dom,
                              guint64 flags G_GNUC_UNUSED,
                              GError **err)
{
    GVirDomainPrivate *priv = dom->priv;

    if (virDomainShutdown(priv->handle) < 0) {
        *err = gvir_error_new_literal(GVIR_DOMAIN_ERROR,
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
                            guint64 flags,
                            GError **err)
{
    GVirDomainPrivate *priv = dom->priv;

    if (virDomainReboot(priv->handle, flags) < 0) {
        *err = gvir_error_new_literal(GVIR_DOMAIN_ERROR,
                                      0,
                                      "Unable to reboot domain");
        return FALSE;
    }

    return TRUE;
}

/**
 * gvir_domain_get_config:
 * @dom: the domain
 * @flags:  the flags
 * Returns: (transfer full): the config
 */
GVirConfigDomain *gvir_domain_get_config(GVirDomain *dom,
                                         guint64 flags,
                                         GError **err)
{
    GVirDomainPrivate *priv = dom->priv;
    gchar *xml;

    if (!(xml = virDomainGetXMLDesc(priv->handle, flags))) {
        *err = gvir_error_new_literal(GVIR_DOMAIN_ERROR,
                                      0,
                                      "Unable to get domain XML config");
        return NULL;
    }

    GVirConfigDomain *conf = gvir_config_domain_new(xml);

    g_free(xml);
    return conf;
}


/**
 * gvir_domain_get_info:
 * @dom: the domain
 * Returns: (transfer full): the info
 */
GVirDomainInfo *gvir_domain_get_info(GVirDomain *dom,
                                     GError **err)
{
    GVirDomainPrivate *priv = dom->priv;
    virDomainInfo info;
    GVirDomainInfo *ret;

    if (virDomainGetInfo(priv->handle, &info) < 0) {
        *err = gvir_error_new_literal(GVIR_DOMAIN_ERROR,
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

/**
 * gvir_domain_screenshot:
 * @stream: stream to use as output
 * @monitor_id: monitor ID to take screenshot from
 * @flags: extra flags, currently unused
 *
 * Returns: (transfer full): mime-type of the image format, or NULL upon error.
 */
gchar *gvir_domain_screenshot(GVirDomain *dom,
                              GVirStream *stream,
                              guint64 monitor_id,
                              guint64 flags,
                              GError **err)
{
    GVirDomainPrivate *priv;
    virStreamPtr st = NULL;
    gchar *mime = NULL;

    g_return_val_if_fail(GVIR_IS_DOMAIN(dom), NULL);
    g_return_val_if_fail(GVIR_IS_STREAM(stream), NULL);

    priv = dom->priv;
    g_object_get(stream, "handle", &st, NULL);

    if (!(mime = virDomainScreenshot(priv->handle,
                                     st,
                                     monitor_id,
                                     flags))) {
        *err = gvir_error_new_literal(GVIR_DOMAIN_ERROR,
                                      0,
                                      "Unable to take a screenshot");
        goto end;
    }
end:
    if (st != NULL)
        virStreamFree(st);
    return mime;
}
