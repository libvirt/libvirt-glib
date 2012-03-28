/*
 * libvirt-gconfig-domain-controller-usb.c: libvirt domain USB controller configuration
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

#define GVIR_CONFIG_DOMAIN_CONTROLLER_USB_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB, GVirConfigDomainControllerUsbPrivate))

struct _GVirConfigDomainControllerUsbPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainControllerUsb, gvir_config_domain_controller_usb, GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER);


static void gvir_config_domain_controller_usb_class_init(GVirConfigDomainControllerUsbClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainControllerUsbPrivate));
}


static void gvir_config_domain_controller_usb_init(GVirConfigDomainControllerUsb *controller_usb)
{
    g_debug("Init GVirConfigDomainControllerUsb=%p", controller_usb);

    controller_usb->priv = GVIR_CONFIG_DOMAIN_CONTROLLER_USB_GET_PRIVATE(controller_usb);
}


GVirConfigDomainControllerUsb *gvir_config_domain_controller_usb_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB,
                                    "controller", NULL);
    gvir_config_object_set_attribute(object, "type", "usb", NULL);
    return GVIR_CONFIG_DOMAIN_CONTROLLER_USB(object);
}

GVirConfigDomainControllerUsb *gvir_config_domain_controller_usb_new_from_xml(const gchar *xml,
                                                                       GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB,
                                             "controller", NULL, xml, error);
    gvir_config_object_set_attribute(object, "type", "usb", NULL);
    return GVIR_CONFIG_DOMAIN_CONTROLLER_USB(object);
}

void gvir_config_domain_controller_usb_set_model(GVirConfigDomainControllerUsb *controller,
                                                 GVirConfigDomainControllerUsbModel model)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CONTROLLER_USB(controller));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(controller),
                                               "model",
                                               GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB_MODEL,
                                               model,
                                               NULL);

}

void gvir_config_domain_controller_usb_set_master(GVirConfigDomainControllerUsb *controller,
                                                  GVirConfigDomainControllerUsb *master,
                                                  guint startport)
{
    guint index;
    char *startport_str;


    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CONTROLLER_USB(controller));
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CONTROLLER_USB(master));

    index = gvir_config_domain_controller_get_index(GVIR_CONFIG_DOMAIN_CONTROLLER(master));
    gvir_config_domain_controller_set_index(GVIR_CONFIG_DOMAIN_CONTROLLER(controller), index);
    startport_str = g_strdup_printf("%d", startport);
    gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(controller),
                                                    "master",
                                                    "startport",
                                                    startport_str);
    g_free(startport_str);
}
