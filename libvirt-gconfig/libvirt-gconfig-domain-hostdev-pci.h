/*
 * libvirt-gconfig-domain-hostdev.h: libvirt domain hostdev configuration
 *
 * Copyright (C) 2016 Red Hat, Inc.
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
 * Authors: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *          Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_HOSTDEV_PCI_H__
#define __LIBVIRT_GCONFIG_DOMAIN_HOSTDEV_PCI_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV_PCI            (gvir_config_domain_hostdev_pci_get_type ())
#define GVIR_CONFIG_DOMAIN_HOSTDEV_PCI(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV_PCI, GVirConfigDomainHostdevPci))
#define GVIR_CONFIG_DOMAIN_HOSTDEV_PCI_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV_PCI, GVirConfigDomainHostdevPciClass))
#define GVIR_CONFIG_IS_DOMAIN_HOSTDEV_PCI(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV_PCI))
#define GVIR_CONFIG_IS_DOMAIN_HOSTDEV_PCI_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV_PCI))
#define GVIR_CONFIG_DOMAIN_HOSTDEV_PCI_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV_PCI, GVirConfigDomainHostdevPciClass))

typedef struct _GVirConfigDomainHostdevPci GVirConfigDomainHostdevPci;
typedef struct _GVirConfigDomainHostdevPciPrivate GVirConfigDomainHostdevPciPrivate;
typedef struct _GVirConfigDomainHostdevPciClass GVirConfigDomainHostdevPciClass;

struct _GVirConfigDomainHostdevPci
{
    GVirConfigDomainHostdev parent;

    GVirConfigDomainHostdevPciPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainHostdevPciClass
{
    GVirConfigDomainHostdevClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_hostdev_pci_get_type(void);

GVirConfigDomainHostdevPci *gvir_config_domain_hostdev_pci_new(void);
GVirConfigDomainHostdevPci *gvir_config_domain_hostdev_pci_new_from_xml(const gchar *xml,
                                                                        GError **error);
void gvir_config_domain_hostdev_pci_set_address(GVirConfigDomainHostdevPci *hostdev,
                                                GVirConfigDomainAddressPci *address);
GVirConfigDomainAddressPci *gvir_config_domain_hostdev_pci_get_address(GVirConfigDomainHostdevPci *hostdev);

void gvir_config_domain_hostdev_pci_set_managed(GVirConfigDomainHostdevPci *hostdev,
                                                gboolean managed);
gboolean gvir_config_domain_hostdev_pci_get_managed(GVirConfigDomainHostdevPci *hostdev);
void gvir_config_domain_hostdev_pci_set_rom_file(GVirConfigDomainHostdevPci *hostdev,
                                                 const gchar *file);
const gchar *gvir_config_domain_hostdev_pci_get_rom_file(GVirConfigDomainHostdevPci *hostdev);
void gvir_config_domain_hostdev_pci_set_rom_bar(GVirConfigDomainHostdevPci *hostdev,
                                                gboolean bar);
gboolean gvir_config_domain_hostdev_pci_get_rom_bar(GVirConfigDomainHostdevPci *hostdev);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_HOSTDEV_PCI_H__ */
