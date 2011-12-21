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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
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

G_DEFINE_TYPE(GVirConfigDomainOs, gvir_config_domain_os, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_os_class_init(GVirConfigDomainOsClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainOsPrivate));
}


static void gvir_config_domain_os_init(GVirConfigDomainOs *os)
{
    g_debug("Init GVirConfigDomainOs=%p", os);

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

void gvir_config_domain_os_set_kernel(GVirConfigDomainOs *os,
                                      const char * kernel)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(os),
                                        "kernel", kernel);
}

void gvir_config_domain_os_set_ramdisk(GVirConfigDomainOs *os,
                                       const char * ramdisk)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(os),
                                        "initrd", ramdisk);
}

void gvir_config_domain_os_set_cmdline(GVirConfigDomainOs *os,
                                       const char * cmdline)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(os),
                                        "cmdline", cmdline);
}

void gvir_config_domain_os_set_init(GVirConfigDomainOs *os,
                                    const char * init)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(os),
                                        "init", init);
}

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
