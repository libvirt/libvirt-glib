/*
 * libvirt-gobject-nwfilter-config.c: libvirt gobject integration
 *
 * Copyright (C) 2010 Red Hat
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_NETWORK_FILTER_H__
#define __LIBVIRT_GCONFIG_NETWORK_FILTER_H__

G_BEGIN_DECLS

#define GVIR_TYPE_CONFIG_NETWORK_FILTER            (gvir_config_network_filter_get_type ())
#define GVIR_CONFIG_NETWORK_FILTER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONFIG_NETWORK_FILTER, GVirConfigNetworkFilter))
#define GVIR_CONFIG_NETWORK_FILTER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONFIG_NETWORK_FILTER, GVirConfigNetworkFilterClass))
#define GVIR_IS_CONFIG_NETWORK_FILTER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONFIG_NETWORK_FILTER))
#define GVIR_IS_CONFIG_NETWORK_FILTER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONFIG_NETWORK_FILTER))
#define GVIR_CONFIG_NETWORK_FILTER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONFIG_NETWORK_FILTER, GVirConfigNetworkFilterClass))

typedef struct _GVirConfigNetworkFilter GVirConfigNetworkFilter;
typedef struct _GVirConfigNetworkFilterPrivate GVirConfigNetworkFilterPrivate;
typedef struct _GVirConfigNetworkFilterClass GVirConfigNetworkFilterClass;

struct _GVirConfigNetworkFilter
{
    GVirConfigObject parent;

    GVirConfigNetworkFilterPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigNetworkFilterClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_network_filter_get_type(void);

GVirConfigNetworkFilter *gvir_config_network_filter_new(const gchar *xml);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_NETWORK_FILTER_H__ */
