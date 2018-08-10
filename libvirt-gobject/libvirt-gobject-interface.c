/*
 * libvirt-gobject-interface.c: libvirt glib integration
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

#define GVIR_INTERFACE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_INTERFACE, GVirInterfacePrivate))

struct _GVirInterfacePrivate
{
    virInterfacePtr handle;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirInterface, gvir_interface, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_INTERFACE_ERROR gvir_interface_error_quark()


static GQuark
gvir_interface_error_quark(void)
{
    return g_quark_from_static_string("gvir-interface");
}

static void gvir_interface_get_property(GObject *object,
                                        guint prop_id,
                                        GValue *value,
                                        GParamSpec *pspec)
{
    GVirInterface *iface = GVIR_INTERFACE(object);
    GVirInterfacePrivate *priv = iface->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_interface_set_property(GObject *object,
                                        guint prop_id,
                                        const GValue *value,
                                        GParamSpec *pspec)
{
    GVirInterface *iface = GVIR_INTERFACE(object);
    GVirInterfacePrivate *priv = iface->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virInterfaceFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_interface_finalize(GObject *object)
{
    GVirInterface *iface = GVIR_INTERFACE(object);
    GVirInterfacePrivate *priv = iface->priv;

    virInterfaceFree(priv->handle);

    G_OBJECT_CLASS(gvir_interface_parent_class)->finalize(object);
}


static void gvir_interface_class_init(GVirInterfaceClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_interface_finalize;
    object_class->get_property = gvir_interface_get_property;
    object_class->set_property = gvir_interface_set_property;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The interface handle",
                                                       GVIR_TYPE_INTERFACE_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));
}


static void gvir_interface_init(GVirInterface *iface)
{
    iface->priv = GVIR_INTERFACE_GET_PRIVATE(iface);
}

typedef struct virInterface GVirInterfaceHandle;

static GVirInterfaceHandle*
gvir_interface_handle_copy(GVirInterfaceHandle *src)
{
    virInterfaceRef((virInterfacePtr)src);
    return src;
}

static void
gvir_interface_handle_free(GVirInterfaceHandle *src)
{
    virInterfaceFree((virInterfacePtr)src);
}

G_DEFINE_BOXED_TYPE(GVirInterfaceHandle, gvir_interface_handle,
                    gvir_interface_handle_copy, gvir_interface_handle_free)

const gchar *gvir_interface_get_name(GVirInterface *iface)
{
    const char *name;

    g_return_val_if_fail(GVIR_IS_INTERFACE(iface), NULL);

    if (!(name = virInterfaceGetName(iface->priv->handle))) {
        gvir_warning("Failed to get interface name on %p", iface->priv->handle);
        return NULL;
    }

    return name;
}

const gchar *gvir_interface_get_mac(GVirInterface *iface)
{
    const char *mac;

    g_return_val_if_fail(GVIR_IS_INTERFACE(iface), NULL);

    if (!(mac = virInterfaceGetMACString(iface->priv->handle))) {
        gvir_warning("Failed to get interface mac on %p", iface->priv->handle);
        return NULL;
    }

    return mac;
}

/**
 * gvir_interface_get_config:
 * @iface: the interface
 * @flags: the flags
 * @err: Place-holder for possible errors
 *
 * Returns: (transfer full): the config. The returned object should be
 * unreffed with g_object_unref() when no longer needed.
 */
GVirConfigInterface *gvir_interface_get_config(GVirInterface *iface,
                                               guint flags,
                                               GError **err)
{
    GVirInterfacePrivate *priv;
    gchar *xml;

    g_return_val_if_fail(GVIR_IS_INTERFACE(iface), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = iface->priv;
    if (!(xml = virInterfaceGetXMLDesc(priv->handle, flags))) {
        gvir_set_error_literal(err, GVIR_INTERFACE_ERROR,
                               0,
                               "Unable to get interface XML config");
        return NULL;
    }

    GVirConfigInterface *conf = gvir_config_interface_new_from_xml(xml, err);

    free(xml);
    return conf;
}
