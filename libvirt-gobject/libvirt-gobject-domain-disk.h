/*
 * libvirt-gobject-domain-disk.h: libvirt gobject integration
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

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#ifndef __LIBVIRT_GOBJECT_DOMAIN_DISK_H__
#define __LIBVIRT_GOBJECT_DOMAIN_DISK_H__

G_BEGIN_DECLS

#define GVIR_TYPE_DOMAIN_DISK            (gvir_domain_disk_get_type ())
#define GVIR_DOMAIN_DISK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_DOMAIN_DISK, GVirDomainDisk))
#define GVIR_DOMAIN_DISK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_DOMAIN_DISK, GVirDomainDiskClass))
#define GVIR_IS_DOMAIN_DISK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_DOMAIN_DISK))
#define GVIR_IS_DOMAIN_DISK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_DOMAIN_DISK))
#define GVIR_DOMAIN_DISK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_DOMAIN_DISK, GVirDomainDiskClass))

#define GVIR_TYPE_DOMAIN_DISK_STATS       (gvir_domain_disk_stats_get_type())

typedef struct _GVirDomainDiskStats GVirDomainDiskStats;
struct _GVirDomainDiskStats
{
  gint64 rd_req; /* number of read requests */
  gint64 rd_bytes; /* number of read bytes */
  gint64 wr_req; /* number of write requests */
  gint64 wr_bytes; /* number of written bytes */
  gint64 errs;   /* In Xen this returns the mysterious 'oo_req'. */
};

typedef struct _GVirDomainDisk GVirDomainDisk;
typedef struct _GVirDomainDiskPrivate GVirDomainDiskPrivate;
typedef struct _GVirDomainDiskClass GVirDomainDiskClass;

struct _GVirDomainDisk
{
    GVirDomainDevice parent;

    GVirDomainDiskPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirDomainDiskClass
{
    GVirDomainDeviceClass parent_class;

    gpointer padding[20];
};

GType gvir_domain_disk_get_type(void);
GType gvir_domain_disk_stats_get_type(void);

GVirDomainDiskStats *gvir_domain_disk_get_stats(GVirDomainDisk *self, GError **err);
gboolean gvir_domain_disk_resize(GVirDomainDisk *self,
                                 guint64 size,
                                 guint flags,
                                 GError **err);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_DOMAIN_DISK_H__ */
