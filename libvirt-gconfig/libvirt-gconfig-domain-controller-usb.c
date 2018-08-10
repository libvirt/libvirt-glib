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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

/**
 * SECTION:libvirt-gconfig-domain-controller-usb
 * @title: USB Controller Configuration
 * @short_description: configuration of USB controllers
 *
 * A #GVirConfigDomainControllerUsb represents an USB controller device.
 * A #GVirConfigDomain with #GVirConfigDomainControllerUsb devices will
 * be able to use USB devices.
 *
 * Several USB controllers can be added to the same domain, for example
 * to have an USB1 and an USB2 controller.
 *
 * When using SPICE (see #GVirConfigGraphicsSpice), USB devices plugged
 * on the client can be forwarded to the guest through the use of
 * #GVirConfigDomainRedirDev.
 *
 * <example>
 * <title>Adding USB controllers to a standard x86 domain</title>
 * <para>
 * This example shows the recommended USB setup to get a virtual machine
 * looking like your usual x86 desktop or laptop.
 * </para>
 * <programlisting>
 * static GVirConfigDomainControllerUsb *
 * create_usb_controller(GVirConfigDomainControllerUsbModel model, guint index,
 *                       GVirConfigDomainControllerUsb *master, guint start_port)
 * {
 *     GVirConfigDomainControllerUsb *controller;
 *
 *     controller = gvir_config_domain_controller_usb_new();
 *     gvir_config_domain_controller_usb_set_model(controller, model);
 *     gvir_config_domain_controller_set_index(GVIR_CONFIG_DOMAIN_CONTROLLER(controller), index);
 *     if (master)
 *         gvir_config_domain_controller_usb_set_master(controller, master, start_port);
 *
 *     return controller;
 * }
 *
 * void setup_default_usb_controllers(GVirConfigDomain *domain)
 * {
 *      GVirConfigDomainControllerUsb *ehci;
 *      GVirConfigDomainControllerUsb *uhci1;
 *      GVirConfigDomainControllerUsb *uhci2;
 *      GVirConfigDomainControllerUsb *uhci3;
 *
 *      ehci = create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_EHCI1,
 *                                   0, NULL, 0);
 *      gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(ehci));
 *      uhci1 = create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI1,
 *                                    0, ehci, 0);
 *      gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(uhci1));
 *      g_object_unref(G_OBJECT(uhci1));
 *      uhci2 = create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI2,
 *                                    0, ehci, 2);
 *      gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(uhci2));
 *      g_object_unref(G_OBJECT(uhci2));
 *      uhci3 = create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI3,
 *                                    0, ehci, 4);
 *      gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(uhci3));
 *      g_object_unref(G_OBJECT(uhci3));
 *      g_object_unref(G_OBJECT(ehci));
 *}
 * </programlisting>
 * </example>
 *
 * This class models libvirt XML nodes located at
 * <ulink url="http://libvirt.org/formatdomain.html#elementsControllers">
 * /domain/devices/controller[@type="usb"]</ulink>
 */

/**
 * GVirConfigDomainControllerUsb:
 *
 * The #GVirConfigDomainControllerUsb struct is an opaque data structure
 * which is used to configure USB controllers on a domain. It should only
 * be accessed via the following functions.
 */

#define GVIR_CONFIG_DOMAIN_CONTROLLER_USB_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB, GVirConfigDomainControllerUsbPrivate))

struct _GVirConfigDomainControllerUsbPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainControllerUsb, gvir_config_domain_controller_usb, GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER);


static void gvir_config_domain_controller_usb_class_init(GVirConfigDomainControllerUsbClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_controller_usb_init(GVirConfigDomainControllerUsb *controller_usb)
{
    controller_usb->priv = GVIR_CONFIG_DOMAIN_CONTROLLER_USB_GET_PRIVATE(controller_usb);
}


/**
 * gvir_config_domain_controller_usb_new:
 *
 * Creates a new #GVirConfigDomainControllerUsb with a reference count of 1.
 * gvir_config_domain_controller_set_index() must be called before
 * this controller is usable.
 *
 * Returns: a new #GVirConfigDomainControllerUsb
 */
GVirConfigDomainControllerUsb *gvir_config_domain_controller_usb_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB,
                                    "controller", NULL);
    gvir_config_object_set_attribute(object, "type", "usb", NULL);
    return GVIR_CONFIG_DOMAIN_CONTROLLER_USB(object);
}

/**
 * gvir_config_domain_controller_usb_new_from_xml:
 * @xml: xml data to create the controller from
 * @error: return location for a #GError, or NULL
 *
 * Creates a new #GVirConfigDomainControllerUsb with a reference count of 1.
 * The controller object will be created using the XML description stored
 * in @xml. This is a fragment of libvirt domain XML whose root node is
 * &lt;controller&gt;.
 *
 * Returns: a new #GVirConfigDomainControllerUsb, or NULL if @xml failed to
 * be parsed.
 */
GVirConfigDomainControllerUsb *gvir_config_domain_controller_usb_new_from_xml(const gchar *xml,
                                                                              GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB,
                                             "controller", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "usb") != 0) {
        g_object_unref(G_OBJECT(object));
        g_return_val_if_reached(NULL);
    }
    return GVIR_CONFIG_DOMAIN_CONTROLLER_USB(object);
}

/**
 * gvir_config_domain_controller_usb_set_model:
 * @controller: a #GVirConfigDomainControllerUsb
 * @model: the USB controller model
 *
 * Sets the model of @controller to @model.
 */
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

/**
 * gvir_config_domain_controller_usb_set_master:
 * @controller: a #GVirConfigDomainControllerUsb
 * @master: the master #GVirConfigDomainControllerUsb
 * @startport: the start port number
 *
 * Sets @controller to be a companion controller of @master. @controller
 * will be exposed from port @startport on @master in the guest.
 * After this call, @controller's index will be set to @master's index.
 */
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
