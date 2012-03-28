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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
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

G_DEFINE_ABSTRACT_TYPE(GVirConfigDomainController, gvir_config_domain_controller, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_controller_class_init(GVirConfigDomainControllerClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainControllerPrivate));
}


static void gvir_config_domain_controller_init(GVirConfigDomainController *controller)
{
    g_debug("Init GVirConfigDomainController=%p", controller);

    controller->priv = GVIR_CONFIG_DOMAIN_CONTROLLER_GET_PRIVATE(controller);
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

void gvir_config_domain_controller_set_address(GVirConfigDomainController *controller,
                                               GVirConfigDomainAddress *address)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CONTROLLER(controller));
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_ADDRESS(address));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(controller),
                                      GVIR_CONFIG_OBJECT(address));
}
