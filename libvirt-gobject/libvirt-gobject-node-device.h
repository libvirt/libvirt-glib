/*
 * libvirt-gobject-node_device.c: libvirt gobject integration
 *
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

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#ifndef __LIBVIRT_GOBJECT_NODE_DEVICE_H__
#define __LIBVIRT_GOBJECT_NODE_DEVICE_H__

G_BEGIN_DECLS

#define GVIR_TYPE_NODE_DEVICE            (gvir_node_device_get_type ())
#define GVIR_NODE_DEVICE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_NODE_DEVICE, GVirNodeDevice))
#define GVIR_NODE_DEVICE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_NODE_DEVICE, GVirNodeDeviceClass))
#define GVIR_IS_NODE_DEVICE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_NODE_DEVICE))
#define GVIR_IS_NODE_DEVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_NODE_DEVICE))
#define GVIR_NODE_DEVICE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_NODE_DEVICE, GVirNodeDeviceClass))

#define GVIR_TYPE_NODE_DEVICE_HANDLE     (gvir_node_device_handle_get_type())

typedef struct _GVirNodeDevice GVirNodeDevice;
typedef struct _GVirNodeDevicePrivate GVirNodeDevicePrivate;
typedef struct _GVirNodeDeviceClass GVirNodeDeviceClass;

struct _GVirNodeDevice
{
    GObject parent;

    GVirNodeDevicePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirNodeDeviceClass
{
    GObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_node_device_get_type(void);
GType gvir_node_device_handle_get_type(void);

const gchar *gvir_node_device_get_name(GVirNodeDevice *device);

GVirConfigNodeDevice *gvir_node_device_get_config(GVirNodeDevice *device,
                                                  guint flags,
                                                  GError **err);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_NODE_DEVICE_H__ */
