/*
 * libvirt-gconfig-domain-address-pci.c: libvirt pci device address configuration
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

#define GVIR_CONFIG_DOMAIN_ADDRESS_PCI_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_PCI, GVirConfigDomainAddressPciPrivate))

struct _GVirConfigDomainAddressPciPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainAddressPci, gvir_config_domain_address_pci, GVIR_CONFIG_TYPE_DOMAIN_ADDRESS);


static void gvir_config_domain_address_pci_class_init(GVirConfigDomainAddressPciClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainAddressPciPrivate));
}


static void gvir_config_domain_address_pci_init(GVirConfigDomainAddressPci *address)
{
    g_debug("Init GVirConfigDomainAddressPci=%p", address);

    address->priv = GVIR_CONFIG_DOMAIN_ADDRESS_PCI_GET_PRIVATE(address);
}


GVirConfigDomainAddressPci *gvir_config_domain_address_pci_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_PCI,
                                    "address", NULL);
    gvir_config_object_set_attribute(object, "type", "pci", NULL);
    return GVIR_CONFIG_DOMAIN_ADDRESS_PCI(object);
}

GVirConfigDomainAddressPci *gvir_config_domain_address_pci_new_from_xml(const gchar *xml,
                                                                        GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_PCI,
                                             "address", NULL, xml, error);
    gvir_config_object_set_attribute(object, "type", "pci", NULL);
    return GVIR_CONFIG_DOMAIN_ADDRESS_PCI(object);
}

static void set_attribute_hex(GVirConfigDomainAddressPci *address,
                              const char *attr_name,
                              guint attr_val,
                              guint min_val,
                              guint max_val,
                              guint size)
{
    char *hex_str;
    char *format;
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_ADDRESS_PCI(address));
    g_return_if_fail((attr_val >= min_val) && (attr_val <= max_val));

    format = g_strdup_printf("0x%%0%dx", size);
    hex_str = g_strdup_printf(format, attr_val);
    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(address),
                                     attr_name, hex_str, NULL);
    g_free(hex_str);
    g_free(format);
}

void gvir_config_domain_address_pci_set_domain(GVirConfigDomainAddressPci *address,
                                               guint16 pci_domain)
{
    set_attribute_hex(address, "domain", pci_domain, 0, G_MAXUINT16, 4);
}

void gvir_config_domain_address_pci_set_bus(GVirConfigDomainAddressPci *address,
                                            guchar bus)
{
    set_attribute_hex(address, "bus", bus, 0, G_MAXUINT8, 2);
}

void gvir_config_domain_address_pci_set_slot(GVirConfigDomainAddressPci *address,
                                             guchar slot)
{
    set_attribute_hex(address, "slot", slot, 0, 0x1f, 2);
}

void gvir_config_domain_address_pci_set_function(GVirConfigDomainAddressPci *address,
                                                 guchar function)
{
    set_attribute_hex(address, "function", function, 0, 7, 1);
}

void gvir_config_domain_address_pci_set_multifunction(GVirConfigDomainAddressPci *address,
                                                      gboolean multifunction)
{
    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(address),
                                     "multifunction",
                                     multifunction?"on":"off", NULL);
}
