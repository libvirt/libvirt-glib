/*
 * libvirt-gconfig-domain-chardev-source-pty.c: libvirt domain chardev pty configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"
#include "libvirt-gconfig/libvirt-gconfig-domain-chardev-source-private.h"

#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_PTY, GVirConfigDomainChardevSourcePtyPrivate))

struct _GVirConfigDomainChardevSourcePtyPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainChardevSourcePty, gvir_config_domain_chardev_source_pty, GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE);


static void gvir_config_domain_chardev_source_pty_class_init(GVirConfigDomainChardevSourcePtyClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_chardev_source_pty_init(GVirConfigDomainChardevSourcePty *source)
{
    source->priv = GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY_GET_PRIVATE(source);
}


GVirConfigDomainChardevSourcePty *gvir_config_domain_chardev_source_pty_new(void)
{
    GVirConfigObject *object;

    /* the name of the root node is just a placeholder, it will be
     * overwritten when the GVirConfigDomainChardevSourcePty is attached to a
     * GVirConfigDomainChardevSourcePty
     */
    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_PTY, "dummy", NULL);
    gvir_config_object_set_attribute(object, "type", "pty", NULL);
    return GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY(object);
}


GVirConfigDomainChardevSourcePty *gvir_config_domain_chardev_source_pty_new_from_xml(const gchar *xml,
                                                                              GError **error)
{
    GVirConfigObject *object;

    /* the name of the root node is just a placeholder, it will be
     * overwritten when the GVirConfigDomainChardevSourcePty is attached to a
     * GVirConfigDomainChardevSourcePty
     */
    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_PTY,
                                             "dummy", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "pty") != 0) {
        g_object_unref(G_OBJECT(object));
        g_return_val_if_reached(NULL);
    }
    return GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY(object);
}

GVirConfigDomainChardevSource *
gvir_config_domain_chardev_source_pty_new_from_tree(GVirConfigXmlDoc *doc,
                                                    xmlNodePtr tree)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_tree(GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_PTY,
                                              doc, NULL, tree);

    return GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE(object);
}


void gvir_config_domain_chardev_source_pty_set_path(GVirConfigDomainChardevSourcePty *pty,
                                                    const char *path)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE_PTY(pty));

    gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(pty),
                                                    "source",
                                                    "path",
                                                    path);
}


const gchar * gvir_config_domain_chardev_source_pty_get_path(GVirConfigDomainChardevSourcePty *pty)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE_PTY(pty), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(pty),
                                            "source", "path");
}
