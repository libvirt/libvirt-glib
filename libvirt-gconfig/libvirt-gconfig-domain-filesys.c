/*
 * libvirt-gobject-config-domain-filesys.c: libvirt glib integration
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <string.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"

#define GVIR_CONFIG_DOMAIN_FILESYS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_FILESYS, GVirConfigDomainFilesysPrivate))

struct _GVirConfigDomainFilesysPrivate
{
    GVirConfigDomainFilesysType type;
};

G_DEFINE_TYPE(GVirConfigDomainFilesys, gvir_config_domain_filesys, GVIR_TYPE_CONFIG_DOMAIN_DEVICE);


static void gvir_config_domain_filesys_class_init(GVirConfigDomainFilesysClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainFilesysPrivate));
}


static void gvir_config_domain_filesys_init(GVirConfigDomainFilesys *filesys)
{
    g_debug("Init GVirConfigDomainFilesys=%p", filesys);

    filesys->priv = GVIR_CONFIG_DOMAIN_FILESYS_GET_PRIVATE(filesys);
}


GVirConfigDomainFilesys *gvir_config_domain_filesys_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_FILESYS,
                                    "filesystem", NULL);
    return GVIR_CONFIG_DOMAIN_FILESYS(object);
}

GVirConfigDomainFilesys *gvir_config_domain_filesys_new_from_xml(const gchar *xml,
                                                                 GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_FILESYS,
                                             "filesystem", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_FILESYS(object);
}

void gvir_config_domain_filesys_set_type(GVirConfigDomainFilesys *filesys,
                                         GVirConfigDomainFilesysType type)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_FILESYS(filesys));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(filesys),
                                               "type",
                                               GVIR_TYPE_CONFIG_DOMAIN_FILESYS_TYPE,
                                               type,
                                               NULL);
}

void gvir_config_domain_filesys_set_access_type(GVirConfigDomainFilesys *filesys,
                                                GVirConfigDomainFilesysAccessType type)
{
    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_FILESYS(filesys));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(filesys),
                                               "accessmode",
                                               GVIR_TYPE_CONFIG_DOMAIN_FILESYS_ACCESS_TYPE,
                                               type,
                                               NULL);
}

void gvir_config_domain_filesys_set_driver_type(GVirConfigDomainFilesys *filesys,
                                                GVirConfigDomainFilesysDriverType type)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_FILESYS(filesys));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(filesys), "driver");
    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(node));
    if (type != GVIR_CONFIG_DOMAIN_FILESYS_DRIVER_DEFAULT)
        gvir_config_object_set_attribute_with_type(
            node, "type",
            GVIR_TYPE_CONFIG_DOMAIN_FILESYS_DRIVER_TYPE,
            type, NULL);
    /* else XXX delete existing attribute ? */
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_filesys_set_source(GVirConfigDomainFilesys *filesys,
                                           const char *source)
{
    GVirConfigObject *source_node;
    const char *attribute_name;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_FILESYS(filesys));
    source_node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(filesys),
                                                   "source");
    g_return_if_fail(source_node != NULL);

    switch (filesys->priv->type) {
        case GVIR_CONFIG_DOMAIN_FILESYS_MOUNT:
            attribute_name = "dir";
            break;
        case GVIR_CONFIG_DOMAIN_FILESYS_FILE:
            attribute_name = "file";
            break;
        case GVIR_CONFIG_DOMAIN_FILESYS_BLOCK:
            attribute_name = "dev";
            break;
        case GVIR_CONFIG_DOMAIN_FILESYS_TEMPLATE:
            attribute_name = "name";
            break;
        default:
            g_return_if_reached();
    }

    gvir_config_object_set_attribute(source_node,
                                     attribute_name, source,
                                     NULL);
    g_object_unref(G_OBJECT(source_node));
}


void gvir_config_domain_filesys_set_target(GVirConfigDomainFilesys *filesys,
                                           const char *path)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_FILESYS(filesys));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(filesys), "target");
    g_return_if_fail(GVIR_IS_CONFIG_OBJECT(node));
    gvir_config_object_set_attribute(node, "dir", path, NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_filesys_set_readonly(GVirConfigDomainFilesys *filesys,
                                             gboolean readonly)
{
    if (readonly) {
        GVirConfigObject *node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(filesys), "readonly");
        g_object_unref(node);
    } else {
        gvir_config_object_delete_child(GVIR_CONFIG_OBJECT(filesys), "readonly");
    }
}
