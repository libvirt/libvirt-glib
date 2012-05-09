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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_PTY, GVirConfigDomainChardevSourcePtyPrivate))

struct _GVirConfigDomainChardevSourcePtyPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainChardevSourcePty, gvir_config_domain_chardev_source_pty, GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE);


static void gvir_config_domain_chardev_source_pty_class_init(GVirConfigDomainChardevSourcePtyClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainChardevSourcePtyPrivate));
}


static void gvir_config_domain_chardev_source_pty_init(GVirConfigDomainChardevSourcePty *source)
{
    g_debug("Init GVirConfigDomainChardevSourcePty=%p", source);

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
    gvir_config_object_set_attribute(object, "type", "pty", NULL);
    return GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_PTY(object);
}

void gvir_config_domain_source_pty_set_path(GVirConfigDomainChardevSourcePty *pty,
                                            const char *path)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE_PTY(pty));

    gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(pty),
                                                    "source",
                                                    "path",
                                                    path);
}
