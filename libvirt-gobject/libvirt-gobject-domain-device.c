/*
 * libvirt-gobject-domain-device.c: libvirt gobject integration
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
 * Author: Marc-André Lureau <marcandre.lureau@redhat.com>
 */

#include <config.h>

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-compat.h"

#include "libvirt-gobject/libvirt-gobject-domain-device-private.h"

#define GVIR_DOMAIN_DEVICE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_DOMAIN_DEVICE, GVirDomainDevicePrivate))

struct _GVirDomainDevicePrivate
{
    GVirDomain *domain;
    GVirConfigDomainDevice *config;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GVirDomainDevice, gvir_domain_device, G_TYPE_OBJECT);

enum {
    PROP_0,
    PROP_DOMAIN,
    PROP_CONFIG,
};

static void gvir_domain_device_get_property(GObject *object,
                                            guint prop_id,
                                            GValue *value,
                                            GParamSpec *pspec)
{
    GVirDomainDevice *self = GVIR_DOMAIN_DEVICE(object);
    GVirDomainDevicePrivate *priv = self->priv;

    switch (prop_id) {
    case PROP_DOMAIN:
        g_value_set_object(value, priv->domain);
        break;

    case PROP_CONFIG:
        g_value_set_object(value, priv->config);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_device_set_property(GObject *object,
                                            guint prop_id,
                                            const GValue *value,
                                            GParamSpec *pspec)
{
    GVirDomainDevice *self = GVIR_DOMAIN_DEVICE(object);
    GVirDomainDevicePrivate *priv = self->priv;

    switch (prop_id) {
    case PROP_DOMAIN:
        g_clear_object(&priv->domain);
        priv->domain = g_value_dup_object(value);
        break;

    case PROP_CONFIG:
        g_clear_object(&priv->config);
        priv->config = g_value_dup_object(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_device_finalize(GObject *object)
{
    GVirDomainDevice *self = GVIR_DOMAIN_DEVICE(object);
    GVirDomainDevicePrivate *priv = self->priv;

    g_clear_object(&priv->domain);
    g_clear_object(&priv->config);

    G_OBJECT_CLASS(gvir_domain_device_parent_class)->finalize(object);
}

static void gvir_domain_device_class_init(GVirDomainDeviceClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_domain_device_finalize;
    object_class->get_property = gvir_domain_device_get_property;
    object_class->set_property = gvir_domain_device_set_property;

    g_object_class_install_property(object_class,
                                    PROP_DOMAIN,
                                    g_param_spec_object("domain",
                                                        "domain",
                                                        "The associated domain",
                                                        GVIR_TYPE_DOMAIN,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));

    g_object_class_install_property(object_class,
                                    PROP_CONFIG,
                                    g_param_spec_object("config",
                                                        "Config",
                                                        "The configuration",
                                                        GVIR_CONFIG_TYPE_DOMAIN_DEVICE,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));
}

static void gvir_domain_device_init(GVirDomainDevice *self)
{
    self->priv = GVIR_DOMAIN_DEVICE_GET_PRIVATE(self);
}

G_GNUC_INTERNAL
virDomainPtr gvir_domain_device_get_domain_handle(GVirDomainDevice *self)
{
    virDomainPtr handle;

    g_object_get(self->priv->domain, "handle", &handle, NULL);

    return handle;
}

/**
 * gvir_domain_device_get_domain:
 * @device: the domain device
 *
 * Returns: (transfer full): the associated domain. The returned object
 * should be unreffed with g_object_unref() when no longer needed.

 */
GVirDomain *gvir_domain_device_get_domain(GVirDomainDevice *device)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN_DEVICE(device), NULL);

    return g_object_ref (device->priv->domain);
}

/**
 * gvir_domain_device_get_config:
 * @device: the domain device
 *
 * Returns: (transfer full): the config. The returned object should be
 * unreffed with g_object_unref() when no longer needed.

 */
GVirConfigDomainDevice *gvir_domain_device_get_config(GVirDomainDevice *device)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN_DEVICE(device), NULL);

    return g_object_ref (device->priv->config);
}

G_GNUC_INTERNAL GVirDomainDevice *gvir_domain_device_new(GVirDomain *domain,
                                                         GVirConfigDomainDevice *config)
{
    GType type;

    g_return_val_if_fail(GVIR_IS_DOMAIN(domain), NULL);
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DEVICE(config), NULL);

    if (GVIR_CONFIG_IS_DOMAIN_DISK(config)) {
        type = GVIR_TYPE_DOMAIN_DISK;
    } else if (GVIR_CONFIG_IS_DOMAIN_INTERFACE(config)) {
        type = GVIR_TYPE_DOMAIN_INTERFACE;
    } else {
        g_debug("Unknown device type: %s", G_OBJECT_TYPE_NAME(config));
        return NULL;
    }

    return GVIR_DOMAIN_DEVICE(g_object_new(type,
                                           "config", config,
                                           "domain", domain, NULL));
}
