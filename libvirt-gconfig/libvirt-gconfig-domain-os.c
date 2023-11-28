/*
 * libvirt-gconfig-domain-os.c: libvirt domain OS configuration
 *
 * Copyright (C) 2011 Red Hat, Inc.
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
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_OS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_OS, GVirConfigDomainOsPrivate))

struct _GVirConfigDomainOsPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainOs, gvir_config_domain_os, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_os_class_init(GVirConfigDomainOsClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_os_init(GVirConfigDomainOs *os)
{
    os->priv = GVIR_CONFIG_DOMAIN_OS_GET_PRIVATE(os);
}


GVirConfigDomainOs *gvir_config_domain_os_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_OS, "os", NULL);
    return GVIR_CONFIG_DOMAIN_OS(object);
}

GVirConfigDomainOs *gvir_config_domain_os_new_from_xml(const gchar *xml, GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_OS, "os",
                                             NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_OS(object);
}

void gvir_config_domain_os_set_os_type(GVirConfigDomainOs *os,
                                       GVirConfigDomainOsType type)
{
    const char *type_str;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os));

    type_str = gvir_config_genum_get_nick(GVIR_CONFIG_TYPE_DOMAIN_OS_TYPE, type);
    g_return_if_fail(type_str != NULL);

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(os),
                                        "type", type_str);
}

GVirConfigDomainOsType gvir_config_domain_os_get_os_type(GVirConfigDomainOs *os)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os),
                         GVIR_CONFIG_DOMAIN_OS_TYPE_HVM);

    return gvir_config_object_get_node_content_genum
            (GVIR_CONFIG_OBJECT(os),
             "type",
             GVIR_CONFIG_TYPE_DOMAIN_OS_TYPE,
             GVIR_CONFIG_DOMAIN_OS_TYPE_HVM);
}

/**
 * gvir_config_domain_os_set_kernel:
 * @os: a #GVirConfigDomainOs
 * @kernel: (allow-none): The kernel path
 */
void gvir_config_domain_os_set_kernel(GVirConfigDomainOs *os,
                                      const char * kernel)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(os),
                                        "kernel", kernel);
}

/**
 * gvir_config_domain_os_set_ramdisk:
 * @os: a #GVirConfigDomainOs
 * @ramdisk: (allow-none): The ramdisk path
 */
void gvir_config_domain_os_set_ramdisk(GVirConfigDomainOs *os,
                                       const char * ramdisk)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(os),
                                        "initrd", ramdisk);
}

/**
 * gvir_config_domain_os_set_cmdline:
 * @os: a #GVirConfigDomainOs
 * @cmdline: (allow-none): The direct boot commandline
 */
void gvir_config_domain_os_set_cmdline(GVirConfigDomainOs *os,
                                       const char * cmdline)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(os),
                                        "cmdline", cmdline);
}

/**
 * gvir_config_domain_os_set_init:
 * @os: a #GVirConfigDomainOs
 * @init: (allow-none):
 */
void gvir_config_domain_os_set_init(GVirConfigDomainOs *os,
                                    const char * init)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(os),
                                        "init", init);
}

/**
 * gvir_config_domain_os_set_loader:
 * @os: a #GVirConfigDomainOs
 * @loader: (allow-none):
 */
void gvir_config_domain_os_set_loader(GVirConfigDomainOs *os,
                                      const char * loader)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(os),
                                        "loader", loader);
}

void gvir_config_domain_os_enable_boot_menu(GVirConfigDomainOs *os,
                                            gboolean enable)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(os), "bootmenu");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute_with_type(node, "enable",
                                               G_TYPE_BOOLEAN, enable,
                                               NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_os_bios_enable_serial(GVirConfigDomainOs *os,
                                              gboolean enable)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(os), "bios");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute_with_type(node, "useserial",
                                               G_TYPE_BOOLEAN, enable,
                                               NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_os_set_smbios_mode(GVirConfigDomainOs *os,
                                           GVirConfigDomainOsSmBiosMode mode)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(os), "smbios");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute_with_type(node, "mode",
                                               GVIR_CONFIG_TYPE_DOMAIN_OS_SM_BIOS_MODE,
                                               mode, NULL);
    g_object_unref(G_OBJECT(node));
}

/**
 * gvir_config_domain_os_set_boot_devices:
 * @os: a #GVirConfigDomainOs
 * @boot_devices: (in) (element-type LibvirtGConfig.DomainOsBootDevice):
 */
void gvir_config_domain_os_set_boot_devices(GVirConfigDomainOs *os, GList *boot_devices)
{
    GList *it;
    xmlNodePtr os_node;
    xmlNodePtr node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os));

    os_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(os));
    g_return_if_fail(os_node != NULL);

    node = os_node->children;
    while (node != NULL) {
        xmlNodePtr next_node;
        next_node = node->next;
        if (g_strcmp0("boot", (char *)node->name) == 0) {
            xmlUnlinkNode(node);
            xmlFreeNode(node);
        }
        node = next_node;
    }

    for (it = boot_devices; it != NULL; it = it->next) {
        const char *dev;

        dev = gvir_config_genum_get_nick(GVIR_CONFIG_TYPE_DOMAIN_OS_BOOT_DEVICE,
                                         GPOINTER_TO_INT(it->data));
        g_warn_if_fail(dev != NULL);
        if (dev != NULL) {
            node = xmlNewDocNode(NULL, NULL, (xmlChar*)"boot", NULL);
            xmlNewProp(node, (xmlChar*)"dev", (xmlChar*)dev);
            xmlAddChild(os_node, node);
        }
    }
}

static gboolean add_boot_device(xmlNodePtr node, gpointer opaque)
{
    GList **devices = (GList **)opaque;
    const gchar *value;

    if (g_strcmp0((const gchar *)node->name, "boot") != 0)
        return TRUE;

    value = gvir_config_xml_get_attribute_content(node, "dev");
    if (value != NULL) {
        GVirConfigDomainOsBootDevice device;

        device = gvir_config_genum_get_value
                        (GVIR_CONFIG_TYPE_DOMAIN_OS_BOOT_DEVICE,
                         value,
                         GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_HD);
        *devices = g_list_append(*devices, GINT_TO_POINTER(device));
    } else
        g_debug("Failed to parse attribute 'dev' of node 'boot'");

    return TRUE;
}

/**
 * gvir_config_domain_os_get_boot_devices:
 * @os: a #GVirConfigDomainOs
 *
 * Gets the list of devices attached to @os. The returned list should be
 * freed with g_list_free().
 *
 * Returns: (element-type LibvirtGConfig.DomainOsBootDevice) (transfer container):
 * a newly allocated #GList of #GVirConfigDomainOsBootDevice.
 */
GList *gvir_config_domain_os_get_boot_devices(GVirConfigDomainOs *os)
{
    GList *devices = NULL;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os), NULL);

    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(os),
                                     NULL,
                                     add_boot_device,
                                     &devices);

    return devices;
}

const char *gvir_config_domain_os_get_arch(GVirConfigDomainOs *os)
{
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(os),
                                            "type",
                                            "arch");
}

void gvir_config_domain_os_set_arch(GVirConfigDomainOs *os, const char *arch)
{
    xmlNodePtr os_node;
    xmlNodePtr os_type_node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os));

    os_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(os));
    g_return_if_fail(os_node != NULL);

    os_type_node = gvir_config_xml_get_element(os_node, "type", NULL);
    g_return_if_fail(os_type_node != NULL);

    xmlNewProp(os_type_node, (xmlChar*)"arch", (xmlChar*)arch);
}

const char *gvir_config_domain_os_get_machine(GVirConfigDomainOs *os)
{
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(os),
                                            "type",
                                            "machine");
}

void gvir_config_domain_os_set_machine(GVirConfigDomainOs *os, const char *machine)
{
    xmlNodePtr os_node;
    xmlNodePtr os_type_node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os));

    os_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(os));
    g_return_if_fail(os_node != NULL);

    os_type_node = gvir_config_xml_get_element(os_node, "type", NULL);
    g_return_if_fail(os_type_node != NULL);

    xmlNewProp(os_type_node, (xmlChar*)"machine", (xmlChar*)machine);
}

void gvir_config_domain_os_set_firmware(GVirConfigDomainOs *os, GVirConfigDomainOsFirmware firmware)
{
    xmlNodePtr node;
    const gchar *firmware_str;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os));

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(os));
    g_return_if_fail(node != NULL);

    firmware_str = gvir_config_genum_get_nick(GVIR_CONFIG_TYPE_DOMAIN_OS_FIRMWARE,
                   firmware);
    g_return_if_fail(firmware_str != NULL);

    xmlNewProp(node, (xmlChar*)"firmware", (xmlChar*)firmware_str);
}

GVirConfigDomainOsFirmware gvir_config_domain_os_get_firmware(GVirConfigDomainOs *os)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os),
                         GVIR_CONFIG_DOMAIN_OS_FIRMWARE_BIOS);

    return gvir_config_object_get_attribute_genum
            (GVIR_CONFIG_OBJECT(os),
             NULL, "firmware",
             GVIR_CONFIG_TYPE_DOMAIN_OS_FIRMWARE,
             GVIR_CONFIG_DOMAIN_OS_FIRMWARE_BIOS);
}

void gvir_config_domain_os_enable_firmware_feature(GVirConfigDomainOs *os, const char *name, gboolean enable)
{
    GVirConfigDomainOsFirmware firmware;
    xmlNodePtr os_node;
    xmlNodePtr firmware_node;
    xmlNodePtr node;
    const gchar *firmware_str;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_OS(os));

    os_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(os));
    g_return_if_fail(os_node != NULL);

    firmware = gvir_config_domain_os_get_firmware(os);
    firmware_str = gvir_config_genum_get_nick(GVIR_CONFIG_TYPE_DOMAIN_OS_FIRMWARE,
                   firmware);
    g_return_if_fail(firmware_str != NULL);

    firmware_node = xmlNewDocNode(NULL, NULL, (xmlChar*)"firmware", NULL);
    node = xmlNewDocNode(NULL, NULL, (xmlChar*)"feature", NULL);
    xmlNewProp(node, (xmlChar*)"enabled", (xmlChar*)(enable ? "yes" : "no"));
    xmlNewProp(node, (xmlChar*)"name", (xmlChar*)name);

    xmlAddChild(firmware_node, node);
    xmlAddChild(os_node, firmware_node);
}
