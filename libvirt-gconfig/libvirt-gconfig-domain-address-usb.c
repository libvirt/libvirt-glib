/*
 * libvirt-gconfig-domain-address-usb.c: libvirt USB device address configuration
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

#define GVIR_CONFIG_DOMAIN_ADDRESS_USB_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_USB, GVirConfigDomainAddressUsbPrivate))

struct _GVirConfigDomainAddressUsbPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainAddressUsb, gvir_config_domain_address_usb, GVIR_CONFIG_TYPE_DOMAIN_ADDRESS);


static void gvir_config_domain_address_usb_class_init(GVirConfigDomainAddressUsbClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_address_usb_init(GVirConfigDomainAddressUsb *address)
{
    address->priv = GVIR_CONFIG_DOMAIN_ADDRESS_USB_GET_PRIVATE(address);
}


GVirConfigDomainAddressUsb *gvir_config_domain_address_usb_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_USB,
                                    "address", NULL);
    gvir_config_object_set_attribute(object, "type", "usb", NULL);
    return GVIR_CONFIG_DOMAIN_ADDRESS_USB(object);
}

GVirConfigDomainAddressUsb *gvir_config_domain_address_usb_new_from_xml(const gchar *xml,
                                                GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_USB,
                                             "address", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "usb") != 0) {
        g_object_unref(G_OBJECT(object));
        g_return_val_if_reached(NULL);
    }
    return GVIR_CONFIG_DOMAIN_ADDRESS_USB(object);
}

void gvir_config_domain_address_usb_set_bus(GVirConfigDomainAddressUsb *address,
                                            guint16 bus)
{
    gchar *bus_str;
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_ADDRESS_USB(address));
    g_return_if_fail(bus <= 0xfff);

    bus_str = g_strdup_printf("0x%03x", bus);
    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(address),
                                     "bus", bus_str, NULL);
    g_free(bus_str);
}

void gvir_config_domain_address_usb_set_port(GVirConfigDomainAddressUsb *address,
                                             const char *port)
{
    /* port is a dotted notation of up to four octets, such as 1.2 or 2.1.3.1,
     * that's why the argument is a char * and not an unsigned int */
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_ADDRESS_USB(address));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(address),
                                     "port", port, NULL);
}
