/*
 * libvirt-gconfig-domain-address-usb.h: libvirt USB device address configuration
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

#define GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_USB            (gvir_config_domain_address_usb_get_type ())
#define GVIR_CONFIG_DOMAIN_ADDRESS_USB(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_USB, GVirConfigDomainAddressUsb))
#define GVIR_CONFIG_DOMAIN_ADDRESS_USB_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_USB, GVirConfigDomainAddressUsbClass))
#define GVIR_CONFIG_IS_DOMAIN_ADDRESS_USB(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_USB))
#define GVIR_CONFIG_IS_DOMAIN_ADDRESS_USB_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_USB))
#define GVIR_CONFIG_DOMAIN_ADDRESS_USB_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_USB, GVirConfigDomainAddressUsbClass))

typedef struct _GVirConfigDomainAddressUsb GVirConfigDomainAddressUsb;
typedef struct _GVirConfigDomainAddressUsbPrivate GVirConfigDomainAddressUsbPrivate;
typedef struct _GVirConfigDomainAddressUsbClass GVirConfigDomainAddressUsbClass;

struct _GVirConfigDomainAddressUsb
{
    GVirConfigDomainAddress parent;

    GVirConfigDomainAddressUsbPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainAddressUsbClass
{
    GVirConfigDomainAddressClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_address_usb_get_type(void);

GVirConfigDomainAddressUsb *gvir_config_domain_address_usb_new(void);
GVirConfigDomainAddressUsb *gvir_config_domain_address_usb_new_from_xml(const gchar *xml,
                                                                        GError **error);
void gvir_config_domain_address_usb_set_bus(GVirConfigDomainAddressUsb *address,
                                            guint16 bus);
void gvir_config_domain_address_usb_set_port(GVirConfigDomainAddressUsb *address,
                                             const char *port);

G_END_DECLS
