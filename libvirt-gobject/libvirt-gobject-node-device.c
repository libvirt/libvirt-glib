/*
 * libvirt-gobject-node_device.c: libvirt glib integration
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

#define GVIR_NODE_DEVICE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_NODE_DEVICE, GVirNodeDevicePrivate))

struct _GVirNodeDevicePrivate
{
    virNodeDevicePtr handle;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirNodeDevice, gvir_node_device, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_NODE_DEVICE_ERROR gvir_node_device_error_quark()


static GQuark
gvir_node_device_error_quark(void)
{
    return g_quark_from_static_string("gvir-node_device");
}

static void gvir_node_device_get_property(GObject *object,
                                          guint prop_id,
                                          GValue *value,
                                          GParamSpec *pspec)
{
    GVirNodeDevice *device = GVIR_NODE_DEVICE(object);
    GVirNodeDevicePrivate *priv = device->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_node_device_set_property(GObject *object,
                                          guint prop_id,
                                          const GValue *value,
                                          GParamSpec *pspec)
{
    GVirNodeDevice *device = GVIR_NODE_DEVICE(object);
    GVirNodeDevicePrivate *priv = device->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virNodeDeviceFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_node_device_finalize(GObject *object)
{
    GVirNodeDevice *device = GVIR_NODE_DEVICE(object);
    GVirNodeDevicePrivate *priv = device->priv;

    virNodeDeviceFree(priv->handle);

    G_OBJECT_CLASS(gvir_node_device_parent_class)->finalize(object);
}


static void gvir_node_device_class_init(GVirNodeDeviceClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_node_device_finalize;
    object_class->get_property = gvir_node_device_get_property;
    object_class->set_property = gvir_node_device_set_property;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The node_device handle",
                                                       GVIR_TYPE_NODE_DEVICE_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));
}


static void gvir_node_device_init(GVirNodeDevice *device)
{
    device->priv = GVIR_NODE_DEVICE_GET_PRIVATE(device);
}

typedef struct virNodeDevice GVirNodeDeviceHandle;

static GVirNodeDeviceHandle*
gvir_node_device_handle_copy(GVirNodeDeviceHandle *src)
{
    virNodeDeviceRef((virNodeDevicePtr)src);
    return src;
}

static void
gvir_node_device_handle_free(GVirNodeDeviceHandle *src)
{
    virNodeDeviceFree((virNodeDevicePtr)src);
}

G_DEFINE_BOXED_TYPE(GVirNodeDeviceHandle, gvir_node_device_handle,
                    gvir_node_device_handle_copy, gvir_node_device_handle_free)

const gchar *gvir_node_device_get_name(GVirNodeDevice *device)
{
    const char *name;

    g_return_val_if_fail(GVIR_IS_NODE_DEVICE(device), NULL);

    if (!(name = virNodeDeviceGetName(device->priv->handle))) {
        gvir_warning("Failed to get node_device name on %p",
                     device->priv->handle);
        return NULL;
    }

    return name;
}


/**
 * gvir_node_device_get_config:
 * @device: the node_device
 * @flags: the flags
 * @err: Place-holder for possible errors
 *
 * Returns: (transfer full): the config. The returned object should be
 * unreffed with g_object_unref() when no longer needed.

 */
GVirConfigNodeDevice *gvir_node_device_get_config(GVirNodeDevice *device,
                                                  guint flags,
                                                  GError **err)
{
    GVirNodeDevicePrivate *priv;
    gchar *xml;

    g_return_val_if_fail(GVIR_IS_NODE_DEVICE(device), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = device->priv;
    if (!(xml = virNodeDeviceGetXMLDesc(priv->handle, flags))) {
        gvir_set_error_literal(err, GVIR_NODE_DEVICE_ERROR,
                               0,
                               "Unable to get node_device XML config");
        return NULL;
    }

    GVirConfigNodeDevice *conf = gvir_config_node_device_new_from_xml(xml, err);

    free(xml);
    return conf;
}
