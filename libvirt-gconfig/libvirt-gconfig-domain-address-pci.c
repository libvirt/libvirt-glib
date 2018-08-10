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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
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

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainAddressPci, gvir_config_domain_address_pci, GVIR_CONFIG_TYPE_DOMAIN_ADDRESS);


static void gvir_config_domain_address_pci_class_init(GVirConfigDomainAddressPciClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_address_pci_init(GVirConfigDomainAddressPci *address)
{
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
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "pci") != 0) {
        g_object_unref(G_OBJECT(object));
        g_return_val_if_reached(NULL);
    }
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

int gvir_config_domain_address_pci_get_domain(GVirConfigDomainAddressPci *address)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_ADDRESS_PCI(address), -1);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(address),
                                                   NULL, "domain",
                                                   -1);
}

void gvir_config_domain_address_pci_set_domain(GVirConfigDomainAddressPci *address,
                                               guint16 pci_domain)
{
    set_attribute_hex(address, "domain", pci_domain, 0, G_MAXUINT16, 4);
}

int gvir_config_domain_address_pci_get_bus(GVirConfigDomainAddressPci *address)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_ADDRESS_PCI(address), -1);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(address),
                                                   NULL, "bus",
                                                   -1);
}

void gvir_config_domain_address_pci_set_bus(GVirConfigDomainAddressPci *address,
                                            guchar bus)
{
    set_attribute_hex(address, "bus", bus, 0, G_MAXUINT8, 2);
}

int gvir_config_domain_address_pci_get_slot(GVirConfigDomainAddressPci *address)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_ADDRESS_PCI(address), -1);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(address),
                                                   NULL, "slot",
                                                   -1);
}

void gvir_config_domain_address_pci_set_slot(GVirConfigDomainAddressPci *address,
                                             guchar slot)
{
    set_attribute_hex(address, "slot", slot, 0, 0x1f, 2);
}

int gvir_config_domain_address_pci_get_function(GVirConfigDomainAddressPci *address)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_ADDRESS_PCI(address), -1);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(address),
                                                   NULL, "function",
                                                   -1);
}

void gvir_config_domain_address_pci_set_function(GVirConfigDomainAddressPci *address,
                                                 guchar function)
{
    set_attribute_hex(address, "function", function, 0, 7, 1);
}

gboolean gvir_config_domain_address_pci_get_multifunction(GVirConfigDomainAddressPci *address)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_ADDRESS_PCI(address), FALSE);

    return gvir_config_object_get_attribute_boolean(GVIR_CONFIG_OBJECT(address),
                                                    NULL, "multifunction",
                                                    FALSE);
}

void gvir_config_domain_address_pci_set_multifunction(GVirConfigDomainAddressPci *address,
                                                      gboolean multifunction)
{
    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(address),
                                     "multifunction",
                                     multifunction?"on":"off", NULL);
}
