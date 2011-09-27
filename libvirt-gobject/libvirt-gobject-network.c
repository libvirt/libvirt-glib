/*
 * libvirt-gobject-network.c: libvirt glib integration
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

#define GVIR_NETWORK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_NETWORK, GVirNetworkPrivate))

struct _GVirNetworkPrivate
{
    virNetworkPtr handle;
};

G_DEFINE_TYPE(GVirNetwork, gvir_network, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_NETWORK_ERROR gvir_network_error_quark()


static GQuark
gvir_network_error_quark(void)
{
    return g_quark_from_static_string("gvir-network");
}

static void gvir_network_get_property(GObject *object,
                                      guint prop_id,
                                      GValue *value,
                                      GParamSpec *pspec)
{
    GVirNetwork *conn = GVIR_NETWORK(object);
    GVirNetworkPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_network_set_property(GObject *object,
                                      guint prop_id,
                                      const GValue *value,
                                      GParamSpec *pspec)
{
    GVirNetwork *conn = GVIR_NETWORK(object);
    GVirNetworkPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virNetworkFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_network_finalize(GObject *object)
{
    GVirNetwork *conn = GVIR_NETWORK(object);
    GVirNetworkPrivate *priv = conn->priv;

    DEBUG("Finalize GVirNetwork=%p", conn);

    virNetworkFree(priv->handle);

    G_OBJECT_CLASS(gvir_network_parent_class)->finalize(object);
}


static void gvir_network_class_init(GVirNetworkClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_network_finalize;
    object_class->get_property = gvir_network_get_property;
    object_class->set_property = gvir_network_set_property;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The network handle",
                                                       GVIR_TYPE_NETWORK_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_NAME |
                                                       G_PARAM_STATIC_NICK |
                                                       G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirNetworkPrivate));
}


static void gvir_network_init(GVirNetwork *conn)
{
    GVirNetworkPrivate *priv;

    DEBUG("Init GVirNetwork=%p", conn);

    priv = conn->priv = GVIR_NETWORK_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}

static gpointer
gvir_network_handle_copy(gpointer src)
{
    virNetworkRef(src);
    return src;
}


GType gvir_network_handle_get_type(void)
{
    static GType handle_type = 0;

    if (G_UNLIKELY(handle_type == 0))
        handle_type = g_boxed_type_register_static
            ("GVirNetworkHandle",
             gvir_network_handle_copy,
             (GBoxedFreeFunc)virNetworkFree);

    return handle_type;
}


const gchar *gvir_network_get_name(GVirNetwork *network)
{
    GVirNetworkPrivate *priv = network->priv;
    const char *name;

    if (!(name = virNetworkGetName(priv->handle))) {
        g_error("Failed to get network name on %p", priv->handle);
    }

    return name;
}


gchar *gvir_network_get_uuid(GVirNetwork *network)
{
    GVirNetworkPrivate *priv = network->priv;
    char *uuid = g_new(gchar, VIR_UUID_STRING_BUFLEN);

    if (virNetworkGetUUIDString(priv->handle, uuid) < 0) {
        g_error("Failed to get network UUID on %p", priv->handle);
    }
    return uuid;
}

/**
 * gvir_network_get_config:
 * @network: the network
 * @flags: the flags
 * Returns: (transfer full): the config
 */
GVirConfigNetwork *gvir_network_get_config(GVirNetwork *network,
                                           guint64 flags,
                                           GError **err)
{
    GVirNetworkPrivate *priv = network->priv;
    gchar *xml;

    if (!(xml = virNetworkGetXMLDesc(priv->handle, flags))) {
        *err = gvir_error_new_literal(GVIR_NETWORK_ERROR,
                                      0,
                                      "Unable to get network XML config");
        return NULL;
    }

    GVirConfigNetwork *conf = gvir_config_network_new(xml);

    g_free(xml);
    return conf;
}
