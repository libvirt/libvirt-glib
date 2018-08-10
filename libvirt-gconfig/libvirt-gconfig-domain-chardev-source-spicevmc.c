/*
 * libvirt-gconfig-domain-chardev-source-spicevmc.c: libvirt domain chardev spicevmc configuration
 *
 * Copyright (C) 2012 Red Hat, Inc.
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

#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_VMC_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_VMC, GVirConfigDomainChardevSourceSpiceVmcPrivate))

struct _GVirConfigDomainChardevSourceSpiceVmcPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainChardevSourceSpiceVmc, gvir_config_domain_chardev_source_spicevmc, GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE);


static void gvir_config_domain_chardev_source_spicevmc_class_init(GVirConfigDomainChardevSourceSpiceVmcClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_chardev_source_spicevmc_init(GVirConfigDomainChardevSourceSpiceVmc *source)
{
    source->priv = GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_VMC_GET_PRIVATE(source);
}


GVirConfigDomainChardevSourceSpiceVmc *gvir_config_domain_chardev_source_spicevmc_new(void)
{
    GVirConfigObject *object;

    /* the name of the root node is just a placeholder, it will be
     * overwritten when the GVirConfigDomainChardevSourceSpiceVmc is attached to a
     * GVirConfigDomainChardevSourceSpiceVmc
     */
    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_VMC, "dummy", NULL);
    gvir_config_object_set_attribute(object, "type", "spicevmc", NULL);
    return GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_VMC(object);
}


GVirConfigDomainChardevSourceSpiceVmc *gvir_config_domain_chardev_source_spicevmc_new_from_xml(const gchar *xml,
                                                                              GError **error)
{
    GVirConfigObject *object;

    /* the name of the root node is just a placeholder, it will be
     * overwritten when the GVirConfigDomainChardevSourceSpiceVmc is attached to a
     * GVirConfigDomainChardevSourceSpiceVmc
     */
    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE_SPICE_VMC,
                                             "dummy", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "spicevmc") != 0) {
        g_object_unref(G_OBJECT(object));
        g_return_val_if_reached(NULL);
    }
    return GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_SPICE_VMC(object);
}
