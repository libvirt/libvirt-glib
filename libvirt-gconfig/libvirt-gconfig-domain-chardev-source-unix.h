/*
 * libvirt-gconfig-domain-chardev-source-unix.h: libvirt domain chardev unix configuration
 *
 * Copyright (C) 2012 Red Hat, Inc.
 * Copyright (C) 2015 T A Mahadevan.
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
 * Author: T A Mahadevan <ta.mahadevan@gmail.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_SOURCE_UNIX_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_SOURCE_UNIX_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_UNIX            (gvir_config_domain_chardev_source_unix_get_type ())
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_UNIX(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_UNIX, GVirConfigDomainChardevSourceUnix))
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_UNIX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_UNIX, GVirConfigDomainChardevSourceUnixClass))
#define GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE_UNIX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_UNIX))
#define GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE_UNIX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_UNIX))
#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_UNIX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_UNIX, GVirConfigDomainChardevSourceUnixClass))

typedef struct _GVirConfigDomainChardevSourceUnix GVirConfigDomainChardevSourceUnix;
typedef struct _GVirConfigDomainChardevSourceUnixPrivate GVirConfigDomainChardevSourceUnixPrivate;
typedef struct _GVirConfigDomainChardevSourceUnixClass GVirConfigDomainChardevSourceUnixClass;

struct _GVirConfigDomainChardevSourceUnix
{
    GVirConfigDomainChardevSource parent;

    GVirConfigDomainChardevSourceUnixPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainChardevSourceUnixClass
{
    GVirConfigDomainChardevSourceClass parent_class;

    gpointer padding[20];
};


GType gvir_config_domain_chardev_source_unix_get_type(void);

GVirConfigDomainChardevSourceUnix *gvir_config_domain_chardev_source_unix_new(void);
GVirConfigDomainChardevSourceUnix *gvir_config_domain_chardev_source_unix_new_from_xml(const gchar *xml,
                                                                                       GError **error);
G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CHARDEV_SOURCE_UNIX_H__ */
