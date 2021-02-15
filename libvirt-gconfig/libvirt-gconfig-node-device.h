/*
 * libvirt-gconfig-node-device.h: libvirt node device configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_NODE_DEVICE            (gvir_config_node_device_get_type ())
#define GVIR_CONFIG_NODE_DEVICE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_NODE_DEVICE, GVirConfigNodeDevice))
#define GVIR_CONFIG_NODE_DEVICE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_NODE_DEVICE, GVirConfigNodeDeviceClass))
#define GVIR_CONFIG_IS_NODE_DEVICE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_NODE_DEVICE))
#define GVIR_CONFIG_IS_NODE_DEVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_NODE_DEVICE))
#define GVIR_CONFIG_NODE_DEVICE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_NODE_DEVICE, GVirConfigNodeDeviceClass))

typedef struct _GVirConfigNodeDevice GVirConfigNodeDevice;
typedef struct _GVirConfigNodeDevicePrivate GVirConfigNodeDevicePrivate;
typedef struct _GVirConfigNodeDeviceClass GVirConfigNodeDeviceClass;

struct _GVirConfigNodeDevice
{
    GVirConfigObject parent;

    GVirConfigNodeDevicePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigNodeDeviceClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_node_device_get_type(void);

GVirConfigNodeDevice *gvir_config_node_device_new(void);
GVirConfigNodeDevice *gvir_config_node_device_new_from_xml(const gchar *xml,
                                                           GError **error);

G_END_DECLS
