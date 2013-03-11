/*
 * libvirt-gconfig-domain-interface-user.c: libvirt domain interface configuration
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_USER_H__
#define __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_USER_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_USER            (gvir_config_domain_interface_user_get_type ())
#define GVIR_CONFIG_DOMAIN_INTERFACE_USER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfaceUser))
#define GVIR_CONFIG_DOMAIN_INTERFACE_USER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfaceUserClass))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE_USER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE_USER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE))
#define GVIR_CONFIG_DOMAIN_INTERFACE_USER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfaceUserClass))

typedef struct _GVirConfigDomainInterfaceUser GVirConfigDomainInterfaceUser;
typedef struct _GVirConfigDomainInterfaceUserPrivate GVirConfigDomainInterfaceUserPrivate;
typedef struct _GVirConfigDomainInterfaceUserClass GVirConfigDomainInterfaceUserClass;

struct _GVirConfigDomainInterfaceUser
{
    GVirConfigDomainInterface parent;

    GVirConfigDomainInterfaceUserPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainInterfaceUserClass
{
    GVirConfigDomainInterfaceClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_interface_user_get_type(void);

GVirConfigDomainInterfaceUser *gvir_config_domain_interface_user_new(void);
GVirConfigDomainInterfaceUser *gvir_config_domain_interface_user_new_from_xml(const gchar *xml,
                                                                              GError **error);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_USER_H__ */
