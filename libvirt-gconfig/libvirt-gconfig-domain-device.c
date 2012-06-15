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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
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

G_DEFINE_ABSTRACT_TYPE(GVirConfigDomainDevice, gvir_config_domain_device, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_device_class_init(GVirConfigDomainDeviceClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainDevicePrivate));
}


static void gvir_config_domain_device_init(GVirConfigDomainDevice *device)
{
    g_debug("Init GVirConfigDomainDevice=%p", device);

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
        goto unimplemented;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"hostdev")) {
        goto unimplemented;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"redirdev")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_REDIRDEV;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"smartcard")) {
        goto unimplemented;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"interface")) {
        return gvir_config_domain_interface_new_from_tree(doc, tree);
    } else if (xmlStrEqual(tree->name, (xmlChar*)"input")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_INPUT;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"hub")) {
        goto unimplemented;
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
        goto unimplemented;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"sound")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_SOUND;
    } else if (xmlStrEqual(tree->name, (xmlChar*)"memballoon")) {
        type = GVIR_CONFIG_TYPE_DOMAIN_MEMBALLOON;
    } else {
        g_debug("Unknown device node: %s", tree->name);
        return NULL;
    }

    g_return_val_if_fail(g_type_is_a(type, GVIR_CONFIG_TYPE_DOMAIN_DEVICE), NULL);

    return GVIR_CONFIG_DOMAIN_DEVICE(gvir_config_object_new_from_tree(type, doc, NULL, tree));
unimplemented:
    g_debug("Parsing of '%s' device nodes is unimplemented", tree->name);
    return NULL;
}
