/*
 * libvirt-gobject-config-domain-os.c: libvirt glib integration
 *
 * Copyright (C) 2011 Red Hat
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

#include <string.h>

#include <libxml/tree.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_CONFIG_DOMAIN_OS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_OS, GVirConfigDomainOsPrivate))

struct _GVirConfigDomainOsPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainOs, gvir_config_domain_os, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_domain_os_class_init(GVirConfigDomainOsClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainOsPrivate));
}


static void gvir_config_domain_os_init(GVirConfigDomainOs *os)
{
    GVirConfigDomainOsPrivate *priv;

    DEBUG("Init GVirConfigDomainOs=%p", os);

    priv = os->priv = GVIR_CONFIG_DOMAIN_OS_GET_PRIVATE(os);

    memset(priv, 0, sizeof(*priv));
}


GVirConfigDomainOs *gvir_config_domain_os_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_OS, "os", NULL);
    return GVIR_CONFIG_DOMAIN_OS(object);
}

GVirConfigDomainOs *gvir_config_domain_os_new_from_xml(const gchar *xml, GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_OS, "os",
                                             NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_OS(object);
}

void gvir_config_domain_os_set_os_type(GVirConfigDomainOs *os,
                                       GVirConfigDomainOsType type)
{
    xmlNodePtr node;
    const char *type_str;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_OS(os));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(os), "type");
    g_return_if_fail(node != NULL);
    type_str = gvir_config_genum_get_nick(GVIR_TYPE_CONFIG_DOMAIN_OS_TYPE, type);
    g_return_if_fail(type_str != NULL);
    xmlNodeSetContent(node, (xmlChar*)type_str);
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
    xmlNodePtr node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_OS(os));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(os), "bootmenu");
    g_return_if_fail(node != NULL);
    if (enable)
        xmlNewProp(node, (xmlChar*)"enable", (xmlChar*)"yes");
    else
        xmlNewProp(node, (xmlChar*)"enable", (xmlChar*)"no");
}

void gvir_config_domain_os_bios_enable_serial(GVirConfigDomainOs *os,
                                              gboolean enable)
{
    xmlNodePtr node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_OS(os));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(os), "bios");
    g_return_if_fail(node != NULL);
    if (enable)
        xmlNewProp(node, (xmlChar*)"useserial", (xmlChar*)"yes");
    else
        xmlNewProp(node, (xmlChar*)"useserial", (xmlChar*)"no");
}

void gvir_config_domain_os_set_smbios_mode(GVirConfigDomainOs *os,
                                    GVirConfigDomainOsSmBiosMode mode)
{
    xmlNodePtr node;
    const char *mode_str;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_OS(os));

    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(os), "smbios");
    g_return_if_fail(node != NULL);
    mode_str = gvir_config_genum_get_nick(GVIR_TYPE_CONFIG_DOMAIN_OS_SM_BIOS_MODE,
                                          mode);
    if (mode_str != NULL)
        xmlNewProp(node, (xmlChar*)"mode", (xmlChar*)mode_str);
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

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_OS(os));

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

        dev = gvir_config_genum_get_nick(GVIR_TYPE_CONFIG_DOMAIN_OS_BOOT_DEVICE,
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

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_OS(os));

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

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_OS(os));

    os_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(os));
    g_return_if_fail(os_node != NULL);

    os_type_node = gvir_config_xml_get_element(os_node, "type", NULL);
    g_return_if_fail(os_type_node != NULL);

    xmlNewProp(os_type_node, (xmlChar*)"machine", (xmlChar*)machine);
}
