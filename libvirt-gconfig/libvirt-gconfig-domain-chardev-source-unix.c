/*
 * libvirt-gconfig-domain-chardev-source-unix.c: libvirt domain chardev unix configuration
 *
 * Copyright (C) 2012 Red Hat, Inc.
 * Copyright (C) 2015 T A Mahadevan
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
 * Author: T A Mahadevan <ta.mahadevan@gmail.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_UNIX_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_UNIX, GVirConfigDomainChardevSourceUnixPrivate))

struct _GVirConfigDomainChardevSourceUnixPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainChardevSourceUnix, gvir_config_domain_chardev_source_unix, GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE);


static void gvir_config_domain_chardev_source_unix_class_init(GVirConfigDomainChardevSourceUnixClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_chardev_source_unix_init(GVirConfigDomainChardevSourceUnix *source)
{
    source->priv = GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_UNIX_GET_PRIVATE(source);
}


GVirConfigDomainChardevSourceUnix *gvir_config_domain_chardev_source_unix_new(void)
{
    GVirConfigObject *object;

    /* the name of the root node is just a placeholder, it will be
     * overwritten when the GVirConfigDomainChardevSourceUnix is attached to a
     * GVirConfigDomainChardev
     */
    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_UNIX, "dummy", NULL);
    gvir_config_object_set_attribute(object, "type", "unix", NULL);
    return GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_UNIX(object);
}


GVirConfigDomainChardevSourceUnix *gvir_config_domain_chardev_source_unix_new_from_xml(const gchar *xml,
                                                                                       GError **error)
{
    GVirConfigObject *object;

    /* the name of the root node is just a placeholder, it will be
     * overwritten when the GVirConfigDomainChardevSourceUnix is attached to a
     * GVirConfigDomainChardev
     */
    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_UNIX,
                                             "dummy", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "unix") != 0) {
        g_object_unref(G_OBJECT(object));
        g_return_val_if_reached(NULL);
    }
    return GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_UNIX(object);
}
