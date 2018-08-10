/*
 * libvirt-gconfig-domain-interface-user.c: libvirt domain interface configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_INTERFACE_USER_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_USER, GVirConfigDomainInterfaceUserPrivate))

struct _GVirConfigDomainInterfaceUserPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainInterfaceUser, gvir_config_domain_interface_user, GVIR_CONFIG_TYPE_DOMAIN_INTERFACE);


static void gvir_config_domain_interface_user_class_init(GVirConfigDomainInterfaceUserClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_interface_user_init(GVirConfigDomainInterfaceUser *iface)
{
    iface->priv = GVIR_CONFIG_DOMAIN_INTERFACE_USER_GET_PRIVATE(iface);
}


GVirConfigDomainInterfaceUser *gvir_config_domain_interface_user_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_USER,
                                    "interface", NULL);
    gvir_config_object_set_attribute(object, "type", "user", NULL);
    return GVIR_CONFIG_DOMAIN_INTERFACE_USER(object);
}

GVirConfigDomainInterfaceUser *gvir_config_domain_interface_user_new_from_xml(const gchar *xml,
                                                                              GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_USER,
                                             "interface", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "user") != 0) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
    return GVIR_CONFIG_DOMAIN_INTERFACE_USER(object);
}
