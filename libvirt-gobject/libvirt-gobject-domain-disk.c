/*
 * libvirt-gobject-domain-disk.c: libvirt gobject integration
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

#define GVIR_DOMAIN_DISK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_DOMAIN_DISK, GVirDomainDiskPrivate))

struct _GVirDomainDiskPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirDomainDisk, gvir_domain_disk, GVIR_TYPE_DOMAIN_DEVICE);

#define GVIR_DOMAIN_DISK_ERROR gvir_domain_disk_error_quark()

static GQuark
gvir_domain_disk_error_quark(void)
{
    return g_quark_from_static_string("gvir-domain-disk");
}

static void gvir_domain_disk_class_init(GVirDomainDiskClass *klass G_GNUC_UNUSED)
{
}

static void gvir_domain_disk_init(GVirDomainDisk *self)
{
    self->priv = GVIR_DOMAIN_DISK_GET_PRIVATE(self);
}

static GVirDomainDiskStats *
gvir_domain_disk_stats_copy(GVirDomainDiskStats *stats)
{
    return g_slice_dup(GVirDomainDiskStats, stats);
}


static void
gvir_domain_disk_stats_free(GVirDomainDiskStats *stats)
{
    g_slice_free(GVirDomainDiskStats, stats);
}


G_DEFINE_BOXED_TYPE(GVirDomainDiskStats, gvir_domain_disk_stats,
                    gvir_domain_disk_stats_copy, gvir_domain_disk_stats_free)

static const gchar *gvir_domain_disk_get_path(GVirDomainDisk *self)
{
    GVirConfigDomainDevice *config;
    const gchar *path;

    config = gvir_domain_device_get_config(GVIR_DOMAIN_DEVICE(self));
    path = gvir_config_domain_disk_get_target_dev(GVIR_CONFIG_DOMAIN_DISK(config));

    g_object_unref(config);

    return path;
}

/**
 * gvir_domain_disk_get_stats:
 * @self: the domain disk
 * @err: an error
 *
 * This function returns network disk stats. Individual fields
 * within the stats structure may be returned as -1, which indicates
 * that the hypervisor does not support that particular statistic.
 *
 * Returns: (transfer full): the stats or %NULL in case of error.The
 * returned object should be unreffed with g_object_unref() when no longer
 * needed.
 **/
GVirDomainDiskStats *gvir_domain_disk_get_stats(GVirDomainDisk *self, GError **err)
{
    GVirDomainDiskStats *ret = NULL;
    virDomainBlockStatsStruct stats;
    virDomainPtr handle;
    const gchar *path;

    g_return_val_if_fail(GVIR_IS_DOMAIN_DISK(self), NULL);
    g_return_val_if_fail((err == NULL) || (*err == NULL), NULL);

    handle = gvir_domain_device_get_domain_handle(GVIR_DOMAIN_DEVICE(self));
    path = gvir_domain_disk_get_path (self);

    if (virDomainBlockStats(handle, path, &stats, sizeof (stats)) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_DISK_ERROR,
                               0,
                               "Unable to get domain disk stats");
        goto end;
    }

    ret = g_slice_new(GVirDomainDiskStats);
    ret->rd_req = stats.rd_req;
    ret->rd_bytes = stats.rd_bytes;
    ret->wr_req = stats.wr_req;
    ret->wr_bytes = stats.wr_bytes;
    ret->errs = stats.errs;

end:
    virDomainFree(handle);
    return ret;
}

/**
 * gvir_domain_disk_resize:
 * @self: the domain disk
 * @size: new size of the block image in kilobytes
 * @flags: flags, currently unused. Pass '0'.
 * @err: placeholder for an error, or NULL
 *
 * This function resize the disk of a running domain.
 *
 * Returns: TRUE if size was successfully changed, FALSE otherwise.
 **/
gboolean gvir_domain_disk_resize(GVirDomainDisk *self,
                                 guint64 size,
                                 guint flags,
                                 GError **err)
{
    gboolean ret = FALSE;
    virDomainPtr handle;
    const gchar *path;

    g_return_val_if_fail(GVIR_IS_DOMAIN_DISK(self), FALSE);
    g_return_val_if_fail(err == NULL || *err == NULL, FALSE);

    handle = gvir_domain_device_get_domain_handle(GVIR_DOMAIN_DEVICE(self));
    path = gvir_domain_disk_get_path (self);

    if (virDomainBlockResize(handle, path, size, flags) < 0) {
        gvir_set_error_literal(err, GVIR_DOMAIN_DISK_ERROR,
                               0,
                               "Failed to resize domain disk");
        goto end;
    }

    ret = TRUE;

end:
    virDomainFree(handle);
    return ret;
}
