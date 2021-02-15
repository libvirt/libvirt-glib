/*
 * libvirt-gconfig-domain-redirdev.h: libvirt domain redirdev configuration
 *
 * Copyright (C) 2012 Red Hat, Inc.
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV            (gvir_config_domain_redirdev_get_type ())
#define GVIR_CONFIG_DOMAIN_REDIRDEV(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV, GVirConfigDomainRedirdev))
#define GVIR_CONFIG_DOMAIN_REDIRDEV_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV, GVirConfigDomainRedirdevClass))
#define GVIR_CONFIG_IS_DOMAIN_REDIRDEV(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV))
#define GVIR_CONFIG_IS_DOMAIN_REDIRDEV_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV))
#define GVIR_CONFIG_DOMAIN_REDIRDEV_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV, GVirConfigDomainRedirdevClass))

typedef struct _GVirConfigDomainRedirdev GVirConfigDomainRedirdev;
typedef struct _GVirConfigDomainRedirdevPrivate GVirConfigDomainRedirdevPrivate;
typedef struct _GVirConfigDomainRedirdevClass GVirConfigDomainRedirdevClass;

struct _GVirConfigDomainRedirdev
{
    GVirConfigDomainChardev parent;

    GVirConfigDomainRedirdevPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainRedirdevClass
{
    GVirConfigDomainChardevClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_REDIRDEV_BUS_USB
} GVirConfigDomainRedirdevBus;

GType gvir_config_domain_redirdev_get_type(void);

GVirConfigDomainRedirdev *gvir_config_domain_redirdev_new(void);
GVirConfigDomainRedirdev *gvir_config_domain_redirdev_new_from_xml(const gchar *xml,
                                                                   GError **error);
void gvir_config_domain_redirdev_set_bus(GVirConfigDomainRedirdev *redirdev,
                                         GVirConfigDomainRedirdevBus bus);
void gvir_config_domain_redirdev_set_address(GVirConfigDomainRedirdev *redirdev,
                                             GVirConfigDomainAddress *address);

G_END_DECLS
