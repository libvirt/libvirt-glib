/*
 * libvirt-gconfig-domain-hostdev.h: libvirt domain hostdev configuration
 *
 * Copyright (C) 2016 Red Hat, Inc.
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
 * Authors: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *          Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV            (gvir_config_domain_hostdev_get_type ())
#define GVIR_CONFIG_DOMAIN_HOSTDEV(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV, GVirConfigDomainHostdev))
#define GVIR_CONFIG_DOMAIN_HOSTDEV_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV, GVirConfigDomainHostdevClass))
#define GVIR_CONFIG_IS_DOMAIN_HOSTDEV(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV))
#define GVIR_CONFIG_IS_DOMAIN_HOSTDEV_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV))
#define GVIR_CONFIG_DOMAIN_HOSTDEV_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV, GVirConfigDomainHostdevClass))

typedef struct _GVirConfigDomainHostdev GVirConfigDomainHostdev;
typedef struct _GVirConfigDomainHostdevPrivate GVirConfigDomainHostdevPrivate;
typedef struct _GVirConfigDomainHostdevClass GVirConfigDomainHostdevClass;

struct _GVirConfigDomainHostdev
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainHostdevPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainHostdevClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_hostdev_get_type(void);

void gvir_config_domain_hostdev_set_boot_order(GVirConfigDomainHostdev *hostdev,
                                               gint order);
gint gvir_config_domain_hostdev_get_boot_order(GVirConfigDomainHostdev *hostdev);

void gvir_config_domain_hostdev_set_readonly(GVirConfigDomainHostdev *hostdev,
                                             gboolean readonly);
gboolean gvir_config_domain_hostdev_get_readonly(GVirConfigDomainHostdev *hostdev);

void gvir_config_domain_hostdev_set_shareable(GVirConfigDomainHostdev *hostdev,
                                              gboolean shareable);
gboolean gvir_config_domain_hostdev_get_shareable(GVirConfigDomainHostdev *hostdev);

G_END_DECLS
