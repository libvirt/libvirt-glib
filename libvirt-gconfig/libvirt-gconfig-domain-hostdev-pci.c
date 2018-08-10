/*
 * libvirt-gconfig-domain-hostdev.c: libvirt domain hostdev configuration
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_HOSTDEV_PCI_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV_PCI, GVirConfigDomainHostdevPciPrivate))

struct _GVirConfigDomainHostdevPciPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainHostdevPci, gvir_config_domain_hostdev_pci, GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV);

static void gvir_config_domain_hostdev_pci_class_init(GVirConfigDomainHostdevPciClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_hostdev_pci_init(GVirConfigDomainHostdevPci *hostdev)
{
    hostdev->priv = GVIR_CONFIG_DOMAIN_HOSTDEV_PCI_GET_PRIVATE(hostdev);
}

/**
 * gvir_config_domain_hostdev_pci_new:
 *
 * Creates a new #GVirConfigDomainHostdevPci.
 *
 * Returns: (transfer full): a new #GVirConfigDomainHostdevPci. The returned
 * object should be unreffed with g_object_unref() when no longer needed.
 */
GVirConfigDomainHostdevPci *gvir_config_domain_hostdev_pci_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV_PCI,
                                    "hostdev", NULL);
    gvir_config_object_set_attribute(object, "mode", "subsystem", NULL);
    gvir_config_object_set_attribute(object, "type", "pci", NULL);

    return GVIR_CONFIG_DOMAIN_HOSTDEV_PCI(object);
}

/**
 * gvir_config_domain_hostdev_pci_new_from_xml:
 * @xml: xml data to create the host device from
 * @error: return location for a #GError, or NULL
 *
 * Creates a new #GVirConfigDomainHostdevPci. The host device object will be
 * created using the XML description stored in @xml. This is a fragment of
 * libvirt domain XML whose root node is &lt;hostdev&gt;.
 *
 * Returns: (transfer full): a new #GVirConfigDomainHostdevPci, or NULL if @xml
 * failed to be parsed. The returned object should be unreffed with
 * g_object_unref() when no longer needed.
 */
GVirConfigDomainHostdevPci *gvir_config_domain_hostdev_pci_new_from_xml(const gchar *xml,
                                                                        GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV_PCI,
                                             "hostdev", NULL, xml, error);
    if (object == NULL)
        return NULL;

    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "pci") != 0) {
        g_object_unref(G_OBJECT(object));
        g_return_val_if_reached(NULL);
    }

    return GVIR_CONFIG_DOMAIN_HOSTDEV_PCI(object);
}

void gvir_config_domain_hostdev_pci_set_address(GVirConfigDomainHostdevPci *hostdev,
                                                GVirConfigDomainAddressPci *address)
{
    GVirConfigObject *source;
    xmlNodePtr node;
    xmlAttrPtr attr;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_HOSTDEV_PCI(hostdev));
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_ADDRESS_PCI(address));
    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(address));
    g_return_if_fail(node != NULL);

    source = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(hostdev),
                                              "source");
    /* Because of https://bugzilla.redhat.com/show_bug.cgi?id=1327577, we can't
     * just use GVirConfigDomainAddressPci's node, as is, since it contains
     * a 'type' attribute, which is not accepted by libvirt. So we create a
     * copy for our use and just delete the 'type' attribute from it.
     */
    node = xmlCopyNode(node, 1);
    for (attr = node->properties; attr; attr = attr->next) {
        if (g_strcmp0 ("type", (char *)attr->name) == 0) {
            xmlRemoveProp (attr);
            break;
        }
    }
    gvir_config_object_set_child(source, node);
    g_object_unref(source);
}

/**
 * gvir_config_domain_hostdev_pci_get_address:
 * @hostdev: A #GVirConfigDomainHostdevPci object.
 *
 * Gets the address associated with @hostdev.
 *
 * Returns: (transfer full): a new #GVirConfigDomainAddressPci, or NULL if no
 * address is associated with @hostdev. The returned object should be unreffed
 * with g_object_unref() when no longer needed.
 */
GVirConfigDomainAddressPci *gvir_config_domain_hostdev_pci_get_address(GVirConfigDomainHostdevPci *hostdev)
{
    GVirConfigObject *source;
    GVirConfigObject* address;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_HOSTDEV_PCI(hostdev), NULL);

    source = gvir_config_object_get_child(GVIR_CONFIG_OBJECT(hostdev), "source");
    if (source == NULL)
        return NULL;

    address = gvir_config_object_get_child_with_type(source,
                                                     "address",
                                                     GVIR_CONFIG_TYPE_DOMAIN_ADDRESS_PCI);
    g_object_unref(source);
    return GVIR_CONFIG_DOMAIN_ADDRESS_PCI(address);
}

void gvir_config_domain_hostdev_pci_set_managed(GVirConfigDomainHostdevPci *hostdev,
                                                gboolean managed)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_HOSTDEV_PCI(hostdev));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(hostdev),
                                               "managed",
                                               G_TYPE_BOOLEAN,
                                               managed,
                                               NULL);
}

gboolean gvir_config_domain_hostdev_pci_get_managed(GVirConfigDomainHostdevPci *hostdev)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_HOSTDEV_PCI(hostdev), FALSE);

    return gvir_config_object_get_attribute_boolean(GVIR_CONFIG_OBJECT(hostdev),
                                                    NULL,
                                                    "managed",
                                                    FALSE);
}

void gvir_config_domain_hostdev_pci_set_rom_file(GVirConfigDomainHostdevPci *hostdev,
                                                 const gchar *file)
{
    GVirConfigObject *rom;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_HOSTDEV_PCI(hostdev));

    rom = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(hostdev), "rom");
    gvir_config_object_set_attribute(rom,
                                     "file", file,
                                     NULL);
    g_object_unref(rom);
}

void gvir_config_domain_hostdev_pci_set_rom_bar(GVirConfigDomainHostdevPci *hostdev,
                                                gboolean bar)
{
    GVirConfigObject *rom;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_HOSTDEV_PCI(hostdev));

    rom = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(hostdev), "rom");
    gvir_config_object_set_attribute(rom,
                                     "bar", bar? "on" : "off",
                                     NULL);
    g_object_unref(rom);
}

const gchar *gvir_config_domain_hostdev_pci_get_rom_file(GVirConfigDomainHostdevPci *hostdev)
{
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(hostdev), "rom", "file");
}

gboolean gvir_config_domain_hostdev_pci_get_rom_bar(GVirConfigDomainHostdevPci *hostdev)
{
    const gchar *bar_str;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_HOSTDEV_PCI(hostdev), FALSE);

    bar_str = gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(hostdev), "rom", "bar");
    return (g_strcmp0(bar_str, "on") == 0);
}
