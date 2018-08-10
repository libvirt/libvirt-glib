/*
 * libvirt-gobject-domain-interface.c: libvirt gobject integration
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

#define GVIR_DOMAIN_INTERFACE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_DOMAIN_INTERFACE, GVirDomainInterfacePrivate))

struct _GVirDomainInterfacePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirDomainInterface, gvir_domain_interface, GVIR_TYPE_DOMAIN_DEVICE);

#define GVIR_DOMAIN_INTERFACE_ERROR gvir_domain_interface_error_quark()

static GQuark
gvir_domain_interface_error_quark(void)
{
    return g_quark_from_static_string("gvir-domain-interface");
}

static void gvir_domain_interface_class_init(GVirDomainInterfaceClass *klass G_GNUC_UNUSED)
{
}

static void gvir_domain_interface_init(GVirDomainInterface *self)
{
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

static const gchar *gvir_domain_interface_get_path(GVirDomainInterface *self)
{
    GVirConfigDomainDevice *config;
    const gchar *path = NULL;

    config = gvir_domain_device_get_config(GVIR_DOMAIN_DEVICE(self));
    if (GVIR_CONFIG_IS_DOMAIN_INTERFACE_USER(self))
        /* FIXME: One of the limitations of user-mode networking of libvirt */
        g_debug("Statistics gathering for user-mode network not yet supported");
    else
        path = gvir_config_domain_interface_get_ifname(GVIR_CONFIG_DOMAIN_INTERFACE (config));

    g_object_unref (config);

    return path;
}

/**
 * gvir_domain_interface_get_stats:
 * @self: the domain interface
 * @err: an error
 *
 * This function returns network interface stats. Individual fields
 * within the stats structure may be returned as -1, which indicates
 * that the hypervisor does not support that particular statistic.
 *
 * Returns: (transfer full): the stats or %NULL in case of error. The
 * returned object should be unreffed with g_object_unref() when no longer
 * needed.
 **/
GVirDomainInterfaceStats *gvir_domain_interface_get_stats(GVirDomainInterface *self, GError **err)
{
    GVirDomainInterfaceStats *ret = NULL;
    virDomainInterfaceStatsStruct stats;
    virDomainPtr handle;
    const gchar *path;

    g_return_val_if_fail(GVIR_IS_DOMAIN_INTERFACE(self), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    handle = gvir_domain_device_get_domain_handle(GVIR_DOMAIN_DEVICE(self));
    path = gvir_domain_interface_get_path (self);
    if (path == NULL) {
        ret = g_slice_new0(GVirDomainInterfaceStats);
        goto end;
    }

    if (virDomainInterfaceStats(handle, path, &stats, sizeof (stats)) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_INTERFACE_ERROR,
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
