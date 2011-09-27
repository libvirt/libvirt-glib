/*
 * libvirt-gobject-secret.c: libvirt glib integration
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

#define GVIR_SECRET_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_SECRET, GVirSecretPrivate))

struct _GVirSecretPrivate
{
    virSecretPtr handle;
};

G_DEFINE_TYPE(GVirSecret, gvir_secret, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_SECRET_ERROR gvir_secret_error_quark()


static GQuark
gvir_secret_error_quark(void)
{
    return g_quark_from_static_string("gvir-secret");
}

static void gvir_secret_get_property(GObject *object,
                                     guint prop_id,
                                     GValue *value,
                                     GParamSpec *pspec)
{
    GVirSecret *conn = GVIR_SECRET(object);
    GVirSecretPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_secret_set_property(GObject *object,
                                     guint prop_id,
                                     const GValue *value,
                                     GParamSpec *pspec)
{
    GVirSecret *conn = GVIR_SECRET(object);
    GVirSecretPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virSecretFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_secret_finalize(GObject *object)
{
    GVirSecret *conn = GVIR_SECRET(object);
    GVirSecretPrivate *priv = conn->priv;

    DEBUG("Finalize GVirSecret=%p", conn);

    virSecretFree(priv->handle);

    G_OBJECT_CLASS(gvir_secret_parent_class)->finalize(object);
}


static void gvir_secret_class_init(GVirSecretClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_secret_finalize;
    object_class->get_property = gvir_secret_get_property;
    object_class->set_property = gvir_secret_set_property;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The secret handle",
                                                       GVIR_TYPE_SECRET_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_NAME |
                                                       G_PARAM_STATIC_NICK |
                                                       G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirSecretPrivate));
}


static void gvir_secret_init(GVirSecret *conn)
{
    GVirSecretPrivate *priv;

    DEBUG("Init GVirSecret=%p", conn);

    priv = conn->priv = GVIR_SECRET_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}

static gpointer
gvir_secret_handle_copy(gpointer src)
{
    virSecretRef(src);
    return src;
}


GType gvir_secret_handle_get_type(void)
{
    static GType handle_type = 0;

    if (G_UNLIKELY(handle_type == 0))
        handle_type = g_boxed_type_register_static
            ("GVirSecretHandle",
             gvir_secret_handle_copy,
             (GBoxedFreeFunc)virSecretFree);

    return handle_type;
}


gchar *gvir_secret_get_uuid(GVirSecret *secret)
{
    GVirSecretPrivate *priv = secret->priv;
    char *uuid = g_new(gchar, VIR_UUID_STRING_BUFLEN);

    if (virSecretGetUUIDString(priv->handle, uuid) < 0) {
        g_error("Failed to get secret UUID on %p", priv->handle);
    }
    return uuid;
}


/**
 * gvir_secret_get_config:
 * @secret: the secret
 * @flags: the flags
 * Returns: (transfer full): the config
 */
GVirConfigSecret *gvir_secret_get_config(GVirSecret *secret,
                                         guint64 flags,
                                         GError **err)
{
    GVirSecretPrivate *priv = secret->priv;
    gchar *xml;

    if (!(xml = virSecretGetXMLDesc(priv->handle, flags))) {
        *err = gvir_error_new_literal(GVIR_SECRET_ERROR,
                                      0,
                                      "Unable to get secret XML config");
        return NULL;
    }

    GVirConfigSecret *conf = gvir_config_secret_new(xml);

    g_free(xml);
    return conf;
}
