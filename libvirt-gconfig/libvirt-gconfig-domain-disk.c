/*
 * libvirt-gobject-config-domain-disk.c: libvirt glib integration
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

#define GVIR_CONFIG_DOMAIN_DISK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_DISK, GVirConfigDomainDiskPrivate))

struct _GVirConfigDomainDiskPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainDisk, gvir_config_domain_disk, GVIR_TYPE_CONFIG_DOMAIN_DEVICE);


static void gvir_config_domain_disk_class_init(GVirConfigDomainDiskClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainDiskPrivate));
}


static void gvir_config_domain_disk_init(GVirConfigDomainDisk *disk)
{
    GVirConfigDomainDiskPrivate *priv;

    DEBUG("Init GVirConfigDomainDisk=%p", disk);

    priv = disk->priv = GVIR_CONFIG_DOMAIN_DISK_GET_PRIVATE(disk);

    memset(priv, 0, sizeof(*priv));
}


GVirConfigDomainDisk *gvir_config_domain_disk_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_DISK,
                                    "disk", NULL);
    return GVIR_CONFIG_DOMAIN_DISK(object);
}

GVirConfigDomainDisk *gvir_config_domain_disk_new_from_xml(const gchar *xml,
                                                           GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_DISK,
                                             "disk", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_DISK(object);
}

void gvir_config_domain_disk_set_type(GVirConfigDomainDisk *disk,
                                      GVirConfigDomainDiskType type)
{
    xmlNodePtr node;
    const char *type_str;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_DISK(disk));

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(disk));
    g_return_if_fail(node != NULL);
    type_str = gvir_config_genum_get_nick(GVIR_TYPE_CONFIG_DOMAIN_DISK_TYPE,
                                          type);
    g_return_if_fail(type_str != NULL);
    xmlNewProp(node, (xmlChar*)"type", (xmlChar*)type_str);
}

void gvir_config_domain_disk_set_guest_device_type(GVirConfigDomainDisk *disk,
                                                   GVirConfigDomainDiskGuestDeviceType type)
{
    xmlNodePtr node;
    const char *type_str;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_DISK(disk));

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(disk));
    g_return_if_fail(node != NULL);
    type_str = gvir_config_genum_get_nick(GVIR_TYPE_CONFIG_DOMAIN_DISK_GUEST_DEVICE_TYPE,
                                          type);
    g_return_if_fail(type_str != NULL);
    xmlNewProp(node, (xmlChar*)"device", (xmlChar*)type_str);
}

void gvir_config_domain_disk_set_snapshot_type(GVirConfigDomainDisk *disk,
                                               GVirConfigDomainDiskSnapshotType type)
{
    xmlNodePtr node;
    const char *type_str;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_DISK(disk));

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(disk));
    g_return_if_fail(node != NULL);
    type_str = gvir_config_genum_get_nick(GVIR_TYPE_CONFIG_DOMAIN_DISK_SNAPSHOT_TYPE,
                                          type);
    g_return_if_fail(type_str != NULL);
    xmlNewProp(node, (xmlChar*)"snapshot", (xmlChar*)type_str);
}
