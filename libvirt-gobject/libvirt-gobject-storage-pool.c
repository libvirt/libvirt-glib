/*
 * libvirt-gobject-storage_pool.c: libvirt glib integration
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

#define GVIR_STORAGE_POOL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_STORAGE_POOL, GVirStoragePoolPrivate))

struct _GVirStoragePoolPrivate
{
    virStoragePoolPtr handle;
};

G_DEFINE_TYPE(GVirStoragePool, gvir_storage_pool, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_STORAGE_POOL_ERROR gvir_storage_pool_error_quark()


static GQuark
gvir_storage_pool_error_quark(void)
{
    return g_quark_from_static_string("gvir-storage-pool");
}

static void gvir_storage_pool_get_property(GObject *object,
                                           guint prop_id,
                                           GValue *value,
                                           GParamSpec *pspec)
{
    GVirStoragePool *conn = GVIR_STORAGE_POOL(object);
    GVirStoragePoolPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_storage_pool_set_property(GObject *object,
                                           guint prop_id,
                                           const GValue *value,
                                           GParamSpec *pspec)
{
    GVirStoragePool *conn = GVIR_STORAGE_POOL(object);
    GVirStoragePoolPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            g_object_unref(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        g_print("Set handle %p\n", priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_storage_pool_finalize(GObject *object)
{
    GVirStoragePool *conn = GVIR_STORAGE_POOL(object);
    GVirStoragePoolPrivate *priv = conn->priv;

    DEBUG("Finalize GVirStoragePool=%p", conn);

    virStoragePoolFree(priv->handle);

    G_OBJECT_CLASS(gvir_storage_pool_parent_class)->finalize(object);
}


static void gvir_storage_pool_class_init(GVirStoragePoolClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_storage_pool_finalize;
    object_class->get_property = gvir_storage_pool_get_property;
    object_class->set_property = gvir_storage_pool_set_property;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The storage_pool handle",
                                                       GVIR_TYPE_STORAGE_POOL_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_NAME |
                                                       G_PARAM_STATIC_NICK |
                                                       G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirStoragePoolPrivate));
}


static void gvir_storage_pool_init(GVirStoragePool *conn)
{
    GVirStoragePoolPrivate *priv;

    DEBUG("Init GVirStoragePool=%p", conn);

    priv = conn->priv = GVIR_STORAGE_POOL_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}

static gpointer
gvir_storage_pool_handle_copy(gpointer src)
{
    virStoragePoolRef(src);
    return src;
}


GType gvir_storage_pool_handle_get_type(void)
{
    static GType handle_type = 0;

    if (G_UNLIKELY(handle_type == 0))
        handle_type = g_boxed_type_register_static
            ("GVirStoragePoolHandle",
             gvir_storage_pool_handle_copy,
             (GBoxedFreeFunc)virStoragePoolFree);

    return handle_type;
}


const gchar *gvir_storage_pool_get_name(GVirStoragePool *dom)
{
    GVirStoragePoolPrivate *priv = dom->priv;
    const char *name;

    if (!(name = virStoragePoolGetName(priv->handle))) {
        g_error("Failed to get storage_pool name on %p", priv->handle);
    }

    return name;
}


gchar *gvir_storage_pool_get_uuid(GVirStoragePool *dom)
{
    GVirStoragePoolPrivate *priv = dom->priv;
    char *uuid = g_new(gchar, VIR_UUID_STRING_BUFLEN);

    if (virStoragePoolGetUUIDString(priv->handle, uuid) < 0) {
        g_error("Failed to get storage_pool UUID on %p", priv->handle);
    }
    return uuid;
}


/**
 * gvir_storage_pool_get_config:
 * @dom: the storage_pool
 * @flags: the flags
 * Returns: (transfer full): the config
 */
GVirConfigStoragePool *gvir_storage_pool_get_config(GVirStoragePool *dom,
                                                    guint64 flags,
                                                    GError **err)
{
    GVirStoragePoolPrivate *priv = dom->priv;
    gchar *xml;

    if (!(xml = virStoragePoolGetXMLDesc(priv->handle, flags))) {
        *err = gvir_error_new_literal(GVIR_STORAGE_POOL_ERROR,
                                      0,
                                      "Unable to get storage_pool XML config");
        return NULL;
    }

    GVirConfigStoragePool *conf = gvir_config_storage_pool_new(xml);

    g_free(xml);
    return conf;
}
