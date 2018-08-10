/*
 * libvirt-gconfig-domain-device.c: libvirt domain device configuration
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

#define GVIR_CONFIG_DOMAIN_DEVICE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_DEVICE, GVirConfigDomainDevicePrivate))

struct _GVirConfigDomainDevicePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainDevice, gvir_config_domain_device, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_device_class_init(GVirConfigDomainDeviceClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_device_init(GVirConfigDomainDevice *device)
{
    device->priv = GVIR_CONFIG_DOMAIN_DEVICE_GET_PRIVATE(device);
}

G_GNUC_INTERNAL GVirConfigDomainDevice *
gvir_config_domain_device_new_from_tree(GVirConfigXmlDoc *doc,
                                        xmlNodePtr tree)
{
    GType type;

    g_return_val_if_fail(GVIR_CONFIG_IS_XML_DOC(doc), NULL);
    g_return_val_if_fail(tree != NULL, NULL);
    g_return_val_if_fail(tree->name != NULL, NULL);

    if (xmlStrEqual(tree->name, (xmlChar*)"disk")) {
        return gvir_config_domain_disk_new_from_tree(doc, tree);
    } else if (xmlStrEqual(tree->name, (xmlChar*)"filesystem")) {
        return gvir_config_domain_filesys_new_from_tree(doc, tree);
    } else if (xmlStrEqual(tree->name, (xmlChar*)"controller")) {
        return gvir_config_domain_controller_new_from_tree(doc, tree);
    } else if (xmlStrEqual(tree->name, (xmlChar*)"lease")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_DEVICE;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"hostdev")) {
        return gvir_config_domain_hostdev_new_from_tree(doc, tree);
    } else if (xmlStrEqual(tree->name, (xmlChar*)"redirdev")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"smartcard")) {
      return gvir_config_domain_smartcard_new_from_tree(doc, tree);
    } else if (xmlStrEqual(tree->name, (xmlChar*)"interface")) {
        return gvir_config_domain_interface_new_from_tree(doc, tree);
    } else if (xmlStrEqual(tree->name, (xmlChar*)"input")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_INPUT;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"hub")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_DEVICE;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"graphics")) {
        return gvir_config_domain_graphics_new_from_tree(doc, tree);
    } else if (xmlStrEqual(tree->name, (xmlChar*)"video")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_VIDEO;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"parallel")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_PARALLEL;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"serial")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_SERIAL;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"console")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_CONSOLE;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"channel")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_CHANNEL;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"watchdog")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_DEVICE;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"sound")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_SOUND;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"memballoon")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_MEMBALLOON;
    } else {
        g_debug("Unknown device node: %s", tree->name);
        type = GVIR_CONFIG_TYPE_DOMAIN_DEVICE;
    }

    g_return_val_if_fail(g_type_is_a(type, GVIR_CONFIG_TYPE_DOMAIN_DEVICE), NULL);

    if (type == GVIR_CONFIG_TYPE_DOMAIN_DEVICE)
        g_debug("Proper support for '%s' device nodes is not yet implemented", tree->name);

    return GVIR_CONFIG_DOMAIN_DEVICE(gvir_config_object_new_from_tree(type, doc, NULL, tree));
}


const gchar *gvir_config_domain_device_get_alias(GVirConfigDomainDevice *dev)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DEVICE(dev), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(dev),
                                            "alias", "name");
}
