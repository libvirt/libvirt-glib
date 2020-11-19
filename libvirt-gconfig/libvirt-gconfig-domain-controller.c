/*
 * libvirt-gconfig-domain-controller.c: libvirt domain controller configuration
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_CONTROLLER_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER, GVirConfigDomainControllerPrivate))

struct _GVirConfigDomainControllerPrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GVirConfigDomainController, gvir_config_domain_controller, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_controller_class_init(GVirConfigDomainControllerClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_controller_init(GVirConfigDomainController *controller)
{
    controller->priv = GVIR_CONFIG_DOMAIN_CONTROLLER_GET_PRIVATE(controller);
}

G_GNUC_INTERNAL GVirConfigDomainDevice *
gvir_config_domain_controller_new_from_tree(GVirConfigXmlDoc *doc,
                                            xmlNodePtr tree)
{
    const char *type;
    GType gtype;

    type = gvir_config_xml_get_attribute_content(tree, "type");
    if (type == NULL)
        return NULL;

    if (g_str_equal(type, "ide")) {
        goto unimplemented;
    } else if (g_str_equal(type, "fdc")) {
        goto unimplemented;
    } else if (g_str_equal(type, "scsi")) {
        goto unimplemented;
    } else if (g_str_equal(type, "sata")) {
        goto unimplemented;
    } else if (g_str_equal(type, "usb")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB;;
    } else if (g_str_equal(type, "ccid")) {
        goto unimplemented;
    } else if (g_str_equal(type, "virtio-serial")) {
        goto unimplemented;
    } else {
        g_debug("Unknown domain controller node: %s", type);
        return NULL;
    }

    return GVIR_CONFIG_DOMAIN_DEVICE(gvir_config_object_new_from_tree(gtype, doc, NULL, tree));

unimplemented:
    g_debug("Parsing of '%s' domain controller nodes is unimplemented", type);
    return NULL;
}

void gvir_config_domain_controller_set_ports(GVirConfigDomainController *controller,
                                             guint ports)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CONTROLLER(controller));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(controller),
                                               "ports", G_TYPE_UINT,
                                               ports, NULL);
}

guint gvir_config_domain_controller_get_ports(GVirConfigDomainController *controller)
{
    const char *ports_str;
    char *end;
    guint ports;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_CONTROLLER(controller), 0);

    ports_str = gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(controller),
                                                 NULL, "ports");
    if (ports_str == 0)
        return 0;

    ports = strtoul(ports_str, &end, 0);
    g_return_val_if_fail(*end == '\0', 0);

    return ports;
}

void gvir_config_domain_controller_set_index(GVirConfigDomainController *controller,
                                             guint index)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CONTROLLER(controller));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(controller),
                                               "index", G_TYPE_UINT,
                                               index, NULL);
}

guint gvir_config_domain_controller_get_index(GVirConfigDomainController *controller)
{
    const char *index_str;
    char *end;
    guint index;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_CONTROLLER(controller), 0);

    index_str = gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(controller),
                                                 NULL, "index");
    if (index_str == 0)
        return 0;

    index = strtoul(index_str, &end, 0);
    g_return_val_if_fail(*end == '\0', 0);

    return index;
}

/**
 * gvir_config_domain_controller_set_address:
 * @address: (allow-none):
 */
void gvir_config_domain_controller_set_address(GVirConfigDomainController *controller,
                                               GVirConfigDomainAddress *address)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CONTROLLER(controller));
    g_return_if_fail(address == NULL || GVIR_CONFIG_IS_DOMAIN_ADDRESS(address));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(controller),
                                      "address",
                                      GVIR_CONFIG_OBJECT(address));
}
