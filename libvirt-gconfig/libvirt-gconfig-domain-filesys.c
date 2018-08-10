/*
 * libvirt-gconfig-domain-filesys.c: libvirt domain filesystem configuration
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_FILESYS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_FILESYS, GVirConfigDomainFilesysPrivate))

struct _GVirConfigDomainFilesysPrivate
{
    GVirConfigDomainFilesysType type;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainFilesys, gvir_config_domain_filesys, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_filesys_class_init(GVirConfigDomainFilesysClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_filesys_init(GVirConfigDomainFilesys *filesys)
{
    filesys->priv = GVIR_CONFIG_DOMAIN_FILESYS_GET_PRIVATE(filesys);
}


GVirConfigDomainFilesys *gvir_config_domain_filesys_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_FILESYS,
                                    "filesystem", NULL);
    return GVIR_CONFIG_DOMAIN_FILESYS(object);
}

GVirConfigDomainFilesys *gvir_config_domain_filesys_new_from_xml(const gchar *xml,
                                                                 GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_FILESYS,
                                             "filesystem", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_FILESYS(object);
}

GVirConfigDomainDevice *
gvir_config_domain_filesys_new_from_tree(GVirConfigXmlDoc *doc,
                                         xmlNodePtr tree)
{
    GVirConfigObject *object;
    GVirConfigDomainFilesys *filesys;
    GVirConfigDomainFilesysType type;
    const char *type_str;

    type_str = gvir_config_xml_get_attribute_content(tree, "type");
    if (type_str == NULL)
        return NULL;

    type = gvir_config_genum_get_value(GVIR_CONFIG_TYPE_DOMAIN_FILESYS_TYPE,
                                       type_str,
                                       GVIR_CONFIG_DOMAIN_FILESYS_FILE);

    object = gvir_config_object_new_from_tree(GVIR_CONFIG_TYPE_DOMAIN_FILESYS,
                                              doc, NULL, tree);
    filesys = GVIR_CONFIG_DOMAIN_FILESYS(object);
    filesys->priv->type = type;

    return GVIR_CONFIG_DOMAIN_DEVICE(object);
}

void gvir_config_domain_filesys_set_type(GVirConfigDomainFilesys *filesys,
                                         GVirConfigDomainFilesysType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_FILESYS(filesys));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(filesys),
                                               "type",
                                               GVIR_CONFIG_TYPE_DOMAIN_FILESYS_TYPE,
                                               type,
                                               NULL);
    filesys->priv->type = type;
}

void gvir_config_domain_filesys_set_access_type(GVirConfigDomainFilesys *filesys,
                                                GVirConfigDomainFilesysAccessType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_FILESYS(filesys));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(filesys),
                                               "accessmode",
                                               GVIR_CONFIG_TYPE_DOMAIN_FILESYS_ACCESS_TYPE,
                                               type,
                                               NULL);
}

void gvir_config_domain_filesys_set_driver_type(GVirConfigDomainFilesys *filesys,
                                                GVirConfigDomainFilesysDriverType type)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_FILESYS(filesys));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(filesys), "driver");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    if (type != GVIR_CONFIG_DOMAIN_FILESYS_DRIVER_DEFAULT)
        gvir_config_object_set_attribute_with_type(
            node, "type",
            GVIR_CONFIG_TYPE_DOMAIN_FILESYS_DRIVER_TYPE,
            type, NULL);
    else
        gvir_config_object_remove_attribute(node, "type");
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_filesys_set_driver_format(GVirConfigDomainFilesys *filesys,
                                                  GVirConfigDomainDiskFormat format)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_FILESYS(filesys));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(filesys), "driver");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));

    gvir_config_object_set_attribute_with_type(
        node,
        "format", GVIR_CONFIG_TYPE_DOMAIN_DISK_FORMAT, format,
        NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_filesys_set_source(GVirConfigDomainFilesys *filesys,
                                           const char *source)
{
    const char *attribute_name;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_FILESYS(filesys));

    switch (filesys->priv->type) {
        case GVIR_CONFIG_DOMAIN_FILESYS_MOUNT:
        case GVIR_CONFIG_DOMAIN_FILESYS_BIND:
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
        case GVIR_CONFIG_DOMAIN_FILESYS_RAM:
            g_return_if_reached();

        default:
            g_return_if_reached();
    }

    gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(filesys),
                                                    "source",
                                                    attribute_name, source);
}

void gvir_config_domain_filesys_set_ram_usage(GVirConfigDomainFilesys *filesys,
                                              guint64 bytes)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_FILESYS(filesys));
    g_return_if_fail(filesys->priv->type == GVIR_CONFIG_DOMAIN_FILESYS_RAM);

    GVirConfigObject *src;

    src = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(filesys),
                                           "source");

    gvir_config_object_set_attribute_with_type(src,
                                               "usage", G_TYPE_UINT64, bytes,
                                               "units", G_TYPE_STRING, "bytes",
                                               NULL);
    g_object_unref(G_OBJECT(src));
}

void gvir_config_domain_filesys_set_target(GVirConfigDomainFilesys *filesys,
                                           const char *path)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_FILESYS(filesys));
    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(filesys),
                                                "target", "dir", path);
}

void gvir_config_domain_filesys_set_readonly(GVirConfigDomainFilesys *filesys,
                                             gboolean readonly)
{
    if (readonly) {
        GVirConfigObject *node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(filesys), "readonly");
        g_object_unref(node);
    } else {
        gvir_config_object_delete_child(GVIR_CONFIG_OBJECT(filesys), "readonly", NULL);
    }
}
