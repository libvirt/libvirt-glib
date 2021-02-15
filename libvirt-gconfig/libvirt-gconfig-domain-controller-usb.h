/*
 * libvirt-gconfig-domain-controller-usb.h: libvirt domain USB controller configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB            (gvir_config_domain_controller_usb_get_type ())
#define GVIR_CONFIG_DOMAIN_CONTROLLER_USB(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB, GVirConfigDomainControllerUsb))
#define GVIR_CONFIG_DOMAIN_CONTROLLER_USB_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB, GVirConfigDomainControllerUsbClass))
#define GVIR_CONFIG_IS_DOMAIN_CONTROLLER_USB(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB))
#define GVIR_CONFIG_IS_DOMAIN_CONTROLLER_USB_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB))
#define GVIR_CONFIG_DOMAIN_CONTROLLER_USB_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER_USB, GVirConfigDomainControllerUsbClass))

typedef struct _GVirConfigDomainControllerUsb GVirConfigDomainControllerUsb;
typedef struct _GVirConfigDomainControllerUsbPrivate GVirConfigDomainControllerUsbPrivate;
typedef struct _GVirConfigDomainControllerUsbClass GVirConfigDomainControllerUsbClass;

struct _GVirConfigDomainControllerUsb
{
    GVirConfigDomainController parent;

    GVirConfigDomainControllerUsbPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainControllerUsbClass
{
    GVirConfigDomainControllerClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_PIIX3_UHCI,
    GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_PIIX4_UHCI,
    GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_EHCI,
    GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_EHCI1,
    GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI1,
    GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI2,
    GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI3,
    GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_VT82C686B_UHCI,
    GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_PCI_OHCI,
    GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_QEMU_XHCI
} GVirConfigDomainControllerUsbModel;

GType gvir_config_domain_controller_usb_get_type(void);

GVirConfigDomainControllerUsb *gvir_config_domain_controller_usb_new(void);
GVirConfigDomainControllerUsb *gvir_config_domain_controller_usb_new_from_xml(const gchar *xml,
                                                                              GError **error);
void gvir_config_domain_controller_usb_set_model(GVirConfigDomainControllerUsb *controller,
                                                 GVirConfigDomainControllerUsbModel model);
void gvir_config_domain_controller_usb_set_master(GVirConfigDomainControllerUsb *controller,
                                                  GVirConfigDomainControllerUsb *master,
                                                  guint startport);

G_END_DECLS
