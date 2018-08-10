/*
 * libvirt-gobject-secret.c: libvirt glib integration
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

#define GVIR_SECRET_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_SECRET, GVirSecretPrivate))

struct _GVirSecretPrivate
{
    virSecretPtr handle;
    gchar uuid[VIR_UUID_STRING_BUFLEN];
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirSecret, gvir_secret, G_TYPE_OBJECT);


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
    GVirSecret *secret = GVIR_SECRET(object);
    GVirSecretPrivate *priv = secret->priv;

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
    GVirSecret *secret = GVIR_SECRET(object);
    GVirSecretPrivate *priv = secret->priv;

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
    GVirSecret *secret = GVIR_SECRET(object);
    GVirSecretPrivate *priv = secret->priv;

    virSecretFree(priv->handle);

    G_OBJECT_CLASS(gvir_secret_parent_class)->finalize(object);
}


static void gvir_secret_constructed(GObject *object)
{
    GVirSecret *secret = GVIR_SECRET(object);
    GVirSecretPrivate *priv = secret->priv;

    G_OBJECT_CLASS(gvir_secret_parent_class)->constructed(object);

    /* xxx we may want to turn this into an initable */
    if (virSecretGetUUIDString(priv->handle, priv->uuid) < 0)
        gvir_warning("Failed to get secret UUID on %p", priv->handle);
}


static void gvir_secret_class_init(GVirSecretClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_secret_finalize;
    object_class->get_property = gvir_secret_get_property;
    object_class->set_property = gvir_secret_set_property;
    object_class->constructed = gvir_secret_constructed;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The secret handle",
                                                       GVIR_TYPE_SECRET_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));
}


static void gvir_secret_init(GVirSecret *secret)
{
    secret->priv = GVIR_SECRET_GET_PRIVATE(secret);
}

typedef struct virSecret GVirSecretHandle;

static GVirSecretHandle*
gvir_secret_handle_copy(GVirSecretHandle *src)
{
    virSecretRef((virSecretPtr)src);
    return src;
}

static void
gvir_secret_handle_free(GVirSecretHandle *src)
{
    virSecretFree((virSecretPtr)src);
}

G_DEFINE_BOXED_TYPE(GVirSecretHandle, gvir_secret_handle,
                    gvir_secret_handle_copy, gvir_secret_handle_free)

const gchar *gvir_secret_get_uuid(GVirSecret *secret)
{
    g_return_val_if_fail(GVIR_IS_SECRET(secret), NULL);

    return secret->priv->uuid;
}


/**
 * gvir_secret_get_config:
 * @secret: the secret
 * @flags: the flags
 * @err: Place-holder for possible errors
 *
 * Returns: (transfer full): the config. The returned object should be
 * unreffed with g_object_unref() when no longer needed.

 */
GVirConfigSecret *gvir_secret_get_config(GVirSecret *secret,
                                         guint flags,
                                         GError **err)
{
    GVirSecretPrivate *priv;
    gchar *xml;

    g_return_val_if_fail(GVIR_IS_SECRET(secret), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = secret->priv;
    if (!(xml = virSecretGetXMLDesc(priv->handle, flags))) {
        gvir_set_error_literal(err, GVIR_SECRET_ERROR,
                               0,
                               "Unable to get secret XML config");
        return NULL;
    }

    GVirConfigSecret *conf = gvir_config_secret_new_from_xml(xml, err);

    free(xml);
    return conf;
}
