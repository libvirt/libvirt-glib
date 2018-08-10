/*
 * libvirt-gconfig-domain-power-management.c: libvirt domain power management configuration
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
 * Authors:
 *
 * Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 * Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_POWER_MANAGEMENT_GET_PRIVATE(obj) \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj),                  \
         GVIR_CONFIG_TYPE_DOMAIN_POWER_MANAGEMENT,           \
         GVirConfigDomainPowerManagementPrivate))

struct _GVirConfigDomainPowerManagementPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainPowerManagement,
              gvir_config_domain_power_management,
              GVIR_CONFIG_TYPE_OBJECT);

static void gvir_config_domain_power_management_class_init (GVirConfigDomainPowerManagementClass *klass G_GNUC_UNUSED)
{
}


static void
gvir_config_domain_power_management_init(GVirConfigDomainPowerManagement *pm)
{
    pm->priv = GVIR_CONFIG_DOMAIN_POWER_MANAGEMENT_GET_PRIVATE(pm);
}


GVirConfigDomainPowerManagement *gvir_config_domain_power_management_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_POWER_MANAGEMENT,
                                    "pm", NULL);
    return GVIR_CONFIG_DOMAIN_POWER_MANAGEMENT(object);
}

GVirConfigDomainPowerManagement *gvir_config_domain_power_management_new_from_xml(const gchar *xml,
                                                                                  GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_POWER_MANAGEMENT,
                                             "pm", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_POWER_MANAGEMENT(object);
}

void gvir_config_domain_power_management_set_mem_suspend_enabled(GVirConfigDomainPowerManagement *pm,
                                                                 gboolean enabled)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_POWER_MANAGEMENT(pm));

    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(pm),
                                                "suspend-to-mem",
                                                "enabled",
                                                enabled? "yes" : "no");
}

void gvir_config_domain_power_management_set_disk_suspend_enabled(GVirConfigDomainPowerManagement *pm,
                                                                  gboolean enabled)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_POWER_MANAGEMENT(pm));

    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(pm),
                                                "suspend-to-disk",
                                                "enabled",
                                                enabled? "yes" : "no");
}
