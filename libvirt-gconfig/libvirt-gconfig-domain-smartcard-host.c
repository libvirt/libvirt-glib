/*
 * libvirt-gconfig-domain-smartcard-host.c: libvirt domain smartcard host configuration
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
 * Author: Alexander Larsson <alexl@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_SMARTCARD_HOST_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST, GVirConfigDomainSmartcardHostPrivate))

struct _GVirConfigDomainSmartcardHostPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainSmartcardHost, gvir_config_domain_smartcard_host, GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD);


static void gvir_config_domain_smartcard_host_class_init(GVirConfigDomainSmartcardHostClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_smartcard_host_init(GVirConfigDomainSmartcardHost *smartcard)
{
    smartcard->priv = GVIR_CONFIG_DOMAIN_SMARTCARD_HOST_GET_PRIVATE(smartcard);
}


GVirConfigDomainSmartcardHost *gvir_config_domain_smartcard_host_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST,
                                    "smartcard", NULL);
    gvir_config_object_set_attribute(object, "mode", "host", NULL);
    return GVIR_CONFIG_DOMAIN_SMARTCARD_HOST(object);
}

GVirConfigDomainSmartcardHost *gvir_config_domain_smartcard_host_new_from_xml(const gchar *xml,
                                                                              GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST,
                                             "smartcard", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "mode"), "host") != 0) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
    return GVIR_CONFIG_DOMAIN_SMARTCARD_HOST(object);
}
