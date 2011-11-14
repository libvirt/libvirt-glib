/*
 * libvirt-gobject-domain-interface.c: libvirt gobject integration
 *
 * Copyright (C) 2011 Red Hat
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
 * Author: Marc-André Lureau <marcandre.lureau@redhat.com>
 */

#include <config.h>

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-compat.h"

#include "libvirt-gobject/libvirt-gobject-domain-device-private.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_DOMAIN_INTERFACE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_DOMAIN_INTERFACE, GVirDomainInterfacePrivate))

struct _GVirDomainInterfacePrivate
{
    gchar *path;
};

G_DEFINE_TYPE(GVirDomainInterface, gvir_domain_interface, GVIR_TYPE_DOMAIN_DEVICE);

enum {
    PROP_0,
    PROP_PATH,
};

#define GVIR_DOMAIN_INTERFACE_ERROR gvir_domain_interface_error_quark()


static GQuark
gvir_domain_interface_error_quark(void)
{
    return g_quark_from_static_string("gvir-domain-interface");
}

static void gvir_domain_interface_get_property(GObject *object,
                                               guint prop_id,
                                               GValue *value,
                                               GParamSpec *pspec)
{
    GVirDomainInterface *self = GVIR_DOMAIN_INTERFACE(object);
    GVirDomainInterfacePrivate *priv = self->priv;

    switch (prop_id) {
    case PROP_PATH:
        g_value_set_string(value, priv->path);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_interface_set_property(GObject *object,
                                          guint prop_id,
                                          const GValue *value,
                                          GParamSpec *pspec)
{
    GVirDomainInterface *self = GVIR_DOMAIN_INTERFACE(object);
    GVirDomainInterfacePrivate *priv = self->priv;

    switch (prop_id) {
    case PROP_PATH:
        g_free(priv->path);
        priv->path = g_value_dup_string(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_interface_finalize(GObject *object)
{
    GVirDomainInterface *self = GVIR_DOMAIN_INTERFACE(object);
    GVirDomainInterfacePrivate *priv = self->priv;

    DEBUG("Finalize GVirDomainInterface=%p", self);

    g_free(priv->path);

    G_OBJECT_CLASS(gvir_domain_interface_parent_class)->finalize(object);
}

static void gvir_domain_interface_class_init(GVirDomainInterfaceClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_domain_interface_finalize;
    object_class->get_property = gvir_domain_interface_get_property;
    object_class->set_property = gvir_domain_interface_set_property;

    g_object_class_install_property(object_class,
                                    PROP_PATH,
                                    g_param_spec_string("path",
                                                        "Path",
                                                        "The interface path",
                                                        NULL,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));

    g_type_class_add_private(klass, sizeof(GVirDomainInterfacePrivate));
}

static void gvir_domain_interface_init(GVirDomainInterface *self)
{
    DEBUG("Init GVirDomainInterface=%p", self);

    self->priv = GVIR_DOMAIN_INTERFACE_GET_PRIVATE(self);
}

static GVirDomainInterfaceStats *
gvir_domain_interface_stats_copy(GVirDomainInterfaceStats *stats)
{
    return g_slice_dup(GVirDomainInterfaceStats, stats);
}


static void
gvir_domain_interface_stats_free(GVirDomainInterfaceStats *stats)
{
    g_slice_free(GVirDomainInterfaceStats, stats);
}


G_DEFINE_BOXED_TYPE(GVirDomainInterfaceStats, gvir_domain_interface_stats,
                    gvir_domain_interface_stats_copy, gvir_domain_interface_stats_free)

/**
 * gvir_domain_interface_get_stats:
 * @self: the domain interface
 * @err: an error
 *
 * This function returns network interface stats. Individual fields
 * within the stats structure may be returned as -1, which indicates
 * that the hypervisor does not support that particular statistic.
 *
 * Returns: (transfer full): the stats or %NULL in case of error
 **/
GVirDomainInterfaceStats *gvir_domain_interface_get_stats(GVirDomainInterface *self, GError **err)
{
    GVirDomainInterfaceStats *ret = NULL;
    virDomainInterfaceStatsStruct stats;
    GVirDomainInterfacePrivate *priv;
    virDomainPtr handle;

    g_return_val_if_fail(GVIR_IS_DOMAIN_INTERFACE(self), NULL);

    priv = self->priv;
    handle = gvir_domain_device_get_domain_handle(GVIR_DOMAIN_DEVICE(self));

    if (virDomainInterfaceStats(handle, priv->path, &stats, sizeof (stats)) < 0) {
        if (err)
            *err = gvir_error_new_literal(GVIR_DOMAIN_INTERFACE_ERROR,
                                          0,
                                          "Unable to get domain interface stats");
        goto end;
    }

    ret = g_slice_new(GVirDomainInterfaceStats);
    ret->rx_bytes = stats.rx_bytes;
    ret->rx_packets = stats.rx_packets;
    ret->rx_errs = stats.rx_errs;
    ret->rx_drop = stats.rx_drop;
    ret->tx_bytes = stats.tx_bytes;
    ret->tx_packets = stats.tx_packets;
    ret->tx_errs = stats.tx_errs;
    ret->tx_drop = stats.tx_drop;

end:
    virDomainFree(handle);
    return ret;
}
