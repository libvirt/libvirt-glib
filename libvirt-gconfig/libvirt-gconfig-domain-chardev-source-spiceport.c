/*
 * libvirt-gconfig-domain-chardev-source-spiceport.c: libvirt domain chardev spiceport configuration
 *
 * Copyright (C) 2014 Red Hat, Inc.
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
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_PORT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_PORT, GVirConfigDomainChardevSourceSpicePortPrivate))

struct _GVirConfigDomainChardevSourceSpicePortPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainChardevSourceSpicePort, gvir_config_domain_chardev_source_spiceport, GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE);


static void gvir_config_domain_chardev_source_spiceport_class_init(GVirConfigDomainChardevSourceSpicePortClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_chardev_source_spiceport_init(GVirConfigDomainChardevSourceSpicePort *source)
{
    source->priv = GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_PORT_GET_PRIVATE(source);
}


GVirConfigDomainChardevSourceSpicePort *gvir_config_domain_chardev_source_spiceport_new(void)
{
    GVirConfigObject *object;

    /* the name of the root node is just a placeholder, it will be
     * overwritten when the GVirConfigDomainChardevSourceSpicePort is attached to a
     * GVirConfigDomainChardev
     */
    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_PORT, "dummy", NULL);
    gvir_config_object_set_attribute(object, "type", "spiceport", NULL);
    return GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_PORT(object);
}


GVirConfigDomainChardevSourceSpicePort *gvir_config_domain_chardev_source_spiceport_new_from_xml(const gchar *xml,
                                                                              GError **error)
{
    GVirConfigObject *object;

    /* the name of the root node is just a placeholder, it will be
     * overwritten when the GVirConfigDomainChardevSourceSpicePort is attached to a
     * GVirConfigDomainChardev
     */
    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_PORT,
                                             "dummy", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "spiceport") != 0) {
        g_object_unref(G_OBJECT(object));
        g_return_val_if_reached(NULL);
    }
    return GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_PORT(object);
}

void gvir_config_domain_chardev_source_spiceport_set_channel(GVirConfigDomainChardevSourceSpicePort *port,
                                                             const char *channel)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE_SPICE_PORT(port));

    gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(port),
                                                    "source",
                                                    "channel",
                                                    channel);

}

const gchar * gvir_config_domain_chardev_source_spiceport_get_channel(GVirConfigDomainChardevSourceSpicePort *port)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE_SPICE_PORT(port), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(port),
                                            "source", "channel");
}
