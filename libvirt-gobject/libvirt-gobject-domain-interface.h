/*
 * libvirt-gobject-domain-interface.h: libvirt gobject integration
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

#ifndef __LIBVIRT_GOBJECT_DOMAIN_INTERFACE_H__
#define __LIBVIRT_GOBJECT_DOMAIN_INTERFACE_H__

G_BEGIN_DECLS

#define GVIR_TYPE_DOMAIN_INTERFACE            (gvir_domain_interface_get_type ())
#define GVIR_DOMAIN_INTERFACE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_DOMAIN_INTERFACE, GVirDomainInterface))
#define GVIR_DOMAIN_INTERFACE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_DOMAIN_INTERFACE, GVirDomainInterfaceClass))
#define GVIR_IS_DOMAIN_INTERFACE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_DOMAIN_INTERFACE))
#define GVIR_IS_DOMAIN_INTERFACE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_DOMAIN_INTERFACE))
#define GVIR_DOMAIN_INTERFACE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_DOMAIN_INTERFACE, GVirDomainInterfaceClass))

#define GVIR_TYPE_DOMAIN_INTERFACE_STATS       (gvir_domain_interface_stats_get_type())

typedef struct _GVirDomainInterfaceStats GVirDomainInterfaceStats;
struct _GVirDomainInterfaceStats
{
    gint64 rx_bytes;
    gint64 rx_packets;
    gint64 rx_errs;
    gint64 rx_drop;
    gint64 tx_bytes;
    gint64 tx_packets;
    gint64 tx_errs;
    gint64 tx_drop;
};

typedef struct _GVirDomainInterface GVirDomainInterface;
typedef struct _GVirDomainInterfacePrivate GVirDomainInterfacePrivate;
typedef struct _GVirDomainInterfaceClass GVirDomainInterfaceClass;

struct _GVirDomainInterface
{
    GVirDomainDevice parent;

    GVirDomainInterfacePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirDomainInterfaceClass
{
    GVirDomainDeviceClass parent_class;

    gpointer padding[20];
};

GType gvir_domain_interface_get_type(void);
GType gvir_domain_interface_stats_get_type(void);

GVirDomainInterfaceStats *gvir_domain_interface_get_stats(GVirDomainInterface *self, GError **err);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_DOMAIN_INTERFACE_H__ */
