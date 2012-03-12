/*
 * libvirt-gconfig-domain-interface.c: libvirt domain interface configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010-2011 Red Hat, Inc.
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
 *         Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_INTERFACE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfacePrivate))

struct _GVirConfigDomainInterfacePrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE(GVirConfigDomainInterface, gvir_config_domain_interface, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_interface_class_init(GVirConfigDomainInterfaceClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainInterfacePrivate));
}


static void gvir_config_domain_interface_init(GVirConfigDomainInterface *interface)
{
    g_debug("Init GVirConfigDomainInterface=%p", interface);

    interface->priv = GVIR_CONFIG_DOMAIN_INTERFACE_GET_PRIVATE(interface);
}

void gvir_config_domain_interface_set_ifname(GVirConfigDomainInterface *interface,
                                             const char *ifname)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE(interface));

    gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(interface),
                                                    "target", "dev", ifname);
}

void gvir_config_domain_interface_set_link_state(GVirConfigDomainInterface *interface,
                                                 GVirConfigDomainInterfaceLinkState state)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE(interface));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(interface),
                                            "link");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute_with_type(node,
                                               "state",
                                               GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_LINK_STATE,
                                               state,
                                               NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_interface_set_mac(GVirConfigDomainInterface *interface,
                                          const char *mac_address)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE(interface));

    gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(interface),
                                                    "mac", "address", mac_address);
}

void gvir_config_domain_interface_set_model(GVirConfigDomainInterface *interface,
                                            const char *model)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE(interface));

    gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(interface),
                                                    "model", "type", model);
}

const char *gvir_config_domain_interface_get_ifname(GVirConfigDomainInterface *interface)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE(interface), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(interface),
                                            "target", "dev");
}

GVirConfigDomainInterfaceLinkState gvir_config_domain_interface_get_link_state(GVirConfigDomainInterface *interface)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE(interface),
                         GVIR_CONFIG_DOMAIN_INTERFACE_LINK_STATE_DEFAULT);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(interface),
                                                  "link", "state",
                                                  GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_LINK_STATE,
                                                  GVIR_CONFIG_DOMAIN_INTERFACE_LINK_STATE_DEFAULT);
}

const char *gvir_config_domain_interface_get_mac(GVirConfigDomainInterface *interface)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE(interface), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(interface),
                                            "mac", "address");
}

const char *gvir_config_domain_interface_get_model(GVirConfigDomainInterface *interface)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE(interface), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(interface),
                                            "model", "type");
}

G_GNUC_INTERNAL GVirConfigDomainDevice *
gvir_config_domain_interface_new_from_tree(GVirConfigXmlDoc *doc,
                                           xmlNodePtr tree)
{
    const char *type;
    GType gtype;

    type = gvir_config_xml_get_attribute_content(tree, "type");
    if (type == NULL)
        return NULL;

    if (g_str_equal(type, "network")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_NETWORK;
    } else if (g_str_equal(type, "user")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_USER;
    } else if (g_str_equal(type, "bridge")) {
        goto unimplemented;
    } else if (g_str_equal(type, "direct")) {
        goto unimplemented;
    } else if (g_str_equal(type, "server")) {
        goto unimplemented;
    } else if (g_str_equal(type, "mcast")) {
        goto unimplemented;
    } else if (g_str_equal(type, "ethernet")) {
        goto unimplemented;
    } else {
        g_debug("Unknown domain interface node: %s", type);
        return NULL;
    }

    return GVIR_CONFIG_DOMAIN_DEVICE(gvir_config_object_new_from_tree(gtype, doc, NULL, tree));

unimplemented:
    g_debug("Parsing of '%s' domain interface nodes is unimplemented", type);
    return NULL;
}
