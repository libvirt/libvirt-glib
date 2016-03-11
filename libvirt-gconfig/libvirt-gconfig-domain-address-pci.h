/*
 * libvirt-gconfig-domain-address-pci.h: libvirt PCI device address configuration
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

#ifndef __LIBVIRT_GCONFIG_DOMAIN_ADDRESS_PCI_H__
#define __LIBVIRT_GCONFIG_DOMAIN_ADDRESS_PCI_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_PCI            (gvir_config_domain_address_pci_get_type ())
#define GVIR_CONFIG_DOMAIN_ADDRESS_PCI(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_PCI, GVirConfigDomainAddressPci))
#define GVIR_CONFIG_DOMAIN_ADDRESS_PCI_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_PCI, GVirConfigDomainAddressPciClass))
#define GVIR_CONFIG_IS_DOMAIN_ADDRESS_PCI(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_PCI))
#define GVIR_CONFIG_IS_DOMAIN_ADDRESS_PCI_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_PCI))
#define GVIR_CONFIG_DOMAIN_ADDRESS_PCI_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_PCI, GVirConfigDomainAddressPciClass))

typedef struct _GVirConfigDomainAddressPci GVirConfigDomainAddressPci;
typedef struct _GVirConfigDomainAddressPciPrivate GVirConfigDomainAddressPciPrivate;
typedef struct _GVirConfigDomainAddressPciClass GVirConfigDomainAddressPciClass;

struct _GVirConfigDomainAddressPci
{
    GVirConfigDomainAddress parent;

    GVirConfigDomainAddressPciPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainAddressPciClass
{
    GVirConfigDomainAddressClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_address_pci_get_type(void);

GVirConfigDomainAddressPci *gvir_config_domain_address_pci_new(void);
GVirConfigDomainAddressPci *gvir_config_domain_address_pci_new_from_xml(const gchar *xml,
                                                                        GError **error);

int gvir_config_domain_address_pci_get_domain(GVirConfigDomainAddressPci *address);
void gvir_config_domain_address_pci_set_domain(GVirConfigDomainAddressPci *address,
                                               guint16 pci_domain);
int gvir_config_domain_address_pci_get_bus(GVirConfigDomainAddressPci *address);
void gvir_config_domain_address_pci_set_bus(GVirConfigDomainAddressPci *address,
                                            guchar bus);
int gvir_config_domain_address_pci_get_slot(GVirConfigDomainAddressPci *address);
void gvir_config_domain_address_pci_set_slot(GVirConfigDomainAddressPci *address,
                                             guchar slot);
int gvir_config_domain_address_pci_get_function(GVirConfigDomainAddressPci *address);
void gvir_config_domain_address_pci_set_function(GVirConfigDomainAddressPci *address,
                                                 guchar function);
gboolean gvir_config_domain_address_pci_get_multifunction(GVirConfigDomainAddressPci *address);
void gvir_config_domain_address_pci_set_multifunction(GVirConfigDomainAddressPci *address,
                                                      gboolean multifunction);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_ADDRESS_PCI_H__ */
