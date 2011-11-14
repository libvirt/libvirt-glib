/*
 * libvirt-gobject-domain-disk.c: libvirt gobject integration
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

#define GVIR_DOMAIN_DISK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_DOMAIN_DISK, GVirDomainDiskPrivate))

struct _GVirDomainDiskPrivate
{
    gchar *path;
};

G_DEFINE_TYPE(GVirDomainDisk, gvir_domain_disk, GVIR_TYPE_DOMAIN_DEVICE);

enum {
    PROP_0,
    PROP_PATH,
};

#define GVIR_DOMAIN_DISK_ERROR gvir_domain_disk_error_quark()


static GQuark
gvir_domain_disk_error_quark(void)
{
    return g_quark_from_static_string("gvir-domain-disk");
}

static void gvir_domain_disk_get_property(GObject *object,
                                          guint prop_id,
                                          GValue *value,
                                          GParamSpec *pspec)
{
    GVirDomainDisk *self = GVIR_DOMAIN_DISK(object);
    GVirDomainDiskPrivate *priv = self->priv;

    switch (prop_id) {
    case PROP_PATH:
        g_value_set_string(value, priv->path);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_disk_set_property(GObject *object,
                                          guint prop_id,
                                          const GValue *value,
                                          GParamSpec *pspec)
{
    GVirDomainDisk *self = GVIR_DOMAIN_DISK(object);
    GVirDomainDiskPrivate *priv = self->priv;

    switch (prop_id) {
    case PROP_PATH:
        g_free(priv->path);
        priv->path = g_value_dup_string(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_disk_finalize(GObject *object)
{
    GVirDomainDisk *self = GVIR_DOMAIN_DISK(object);
    GVirDomainDiskPrivate *priv = self->priv;

    DEBUG("Finalize GVirDomainDisk=%p", self);

    g_free(priv->path);

    G_OBJECT_CLASS(gvir_domain_disk_parent_class)->finalize(object);
}

static void gvir_domain_disk_class_init(GVirDomainDiskClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_domain_disk_finalize;
    object_class->get_property = gvir_domain_disk_get_property;
    object_class->set_property = gvir_domain_disk_set_property;

    g_object_class_install_property(object_class,
                                    PROP_PATH,
                                    g_param_spec_string("path",
                                                        "Path",
                                                        "The disk path",
                                                        NULL,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));

    g_type_class_add_private(klass, sizeof(GVirDomainDiskPrivate));
}

static void gvir_domain_disk_init(GVirDomainDisk *self)
{
    DEBUG("Init GVirDomainDisk=%p", self);

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

/**
 * gvir_domain_disk_get_stats:
 * @self: the domain disk
 * @err: an error
 *
 * This function returns network disk stats. Individual fields
 * within the stats structure may be returned as -1, which indicates
 * that the hypervisor does not support that particular statistic.
 *
 * Returns: (transfer full): the stats or %NULL in case of error
 **/
GVirDomainDiskStats *gvir_domain_disk_get_stats(GVirDomainDisk *self, GError **err)
{
    GVirDomainDiskStats *ret = NULL;
    virDomainBlockStatsStruct stats;
    GVirDomainDiskPrivate *priv;
    virDomainPtr handle;

    g_return_val_if_fail(GVIR_IS_DOMAIN_DISK(self), NULL);

    priv = self->priv;
    handle = gvir_domain_device_get_domain_handle(GVIR_DOMAIN_DEVICE(self));

    if (virDomainBlockStats(handle, priv->path, &stats, sizeof (stats)) < 0) {
        if (err)
            *err = gvir_error_new_literal(GVIR_DOMAIN_DISK_ERROR,
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
