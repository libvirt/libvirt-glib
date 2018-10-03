/*
 * libvirt-gconfig-domain-smartcard.c: libvirt domain smartcard configuration
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

#define GVIR_CONFIG_DOMAIN_SMARTCARD_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD, GVirConfigDomainSmartcardPrivate))

struct _GVirConfigDomainSmartcardPrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GVirConfigDomainSmartcard, gvir_config_domain_smartcard, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_smartcard_class_init(GVirConfigDomainSmartcardClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_smartcard_init(GVirConfigDomainSmartcard *smartcard)
{
    smartcard->priv = GVIR_CONFIG_DOMAIN_SMARTCARD_GET_PRIVATE(smartcard);
}

/**
 * gvir_config_domain_smartcard_set_address:
 * @address: (allow-none):
 */
void gvir_config_domain_smartcard_set_address(GVirConfigDomainRedirdev *redirdev,
                                              GVirConfigDomainAddress *address)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_REDIRDEV(redirdev));
    g_return_if_fail(address == NULL || GVIR_CONFIG_IS_DOMAIN_ADDRESS(address));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(redirdev),
                                      "address",
                                      GVIR_CONFIG_OBJECT(address));
}

G_GNUC_INTERNAL GVirConfigDomainDevice *
gvir_config_domain_smartcard_new_from_tree(GVirConfigXmlDoc *doc,
                                           xmlNodePtr tree)
{
    const char *mode;
    GType gtype;

    mode = gvir_config_xml_get_attribute_content(tree, "mode");
    if (mode == NULL)
        return NULL;

    if (g_str_equal(mode, "host")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST;
    } else if (g_str_equal(mode, "host-certificates")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST_CERTIFICATES;
    } else if (g_str_equal(mode, "passthrough")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_PASSTHROUGH;
    } else {
        g_debug("Unknown domain smartcard node: %s", mode);
        return NULL;
    }

    return GVIR_CONFIG_DOMAIN_DEVICE(gvir_config_object_new_from_tree(gtype, doc, NULL, tree));
}
