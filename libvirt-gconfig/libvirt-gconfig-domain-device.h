/*
 * libvirt-gconfig-domain-device.h: libvirt domain device configuration
 *
 * Copyright (C) 2011 Red Hat, Inc.
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
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_DEVICE            (gvir_config_domain_device_get_type ())
#define GVIR_CONFIG_DOMAIN_DEVICE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_DEVICE, GVirConfigDomainDevice))
#define GVIR_CONFIG_DOMAIN_DEVICE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_DEVICE, GVirConfigDomainDeviceClass))
#define GVIR_CONFIG_IS_DOMAIN_DEVICE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_DEVICE))
#define GVIR_CONFIG_IS_DOMAIN_DEVICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_DEVICE))
#define GVIR_CONFIG_DOMAIN_DEVICE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_DEVICE, GVirConfigDomainDeviceClass))

typedef struct _GVirConfigDomainDevice GVirConfigDomainDevice;
typedef struct _GVirConfigDomainDevicePrivate GVirConfigDomainDevicePrivate;
typedef struct _GVirConfigDomainDeviceClass GVirConfigDomainDeviceClass;

struct _GVirConfigDomainDevice
{
    GVirConfigObject parent;

    GVirConfigDomainDevicePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainDeviceClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_domain_device_get_type(void);

const gchar *gvir_config_domain_device_get_alias(GVirConfigDomainDevice *dev);

G_END_DECLS
