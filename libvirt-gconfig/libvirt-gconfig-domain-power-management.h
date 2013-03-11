/*
 * libvirt-gconfig-domain-power-management.h: libvirt domain power management configuration
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
 * Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 * Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_POWER_MANAGEMENT_H__
#define __LIBVIRT_GCONFIG_DOMAIN_POWER_MANAGEMENT_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_POWER_MANAGEMENT            (gvir_config_domain_power_management_get_type ())
#define GVIR_CONFIG_DOMAIN_POWER_MANAGEMENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_POWER_MANAGEMENT, GVirConfigDomainPowerManagement))
#define GVIR_CONFIG_DOMAIN_POWER_MANAGEMENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_POWER_MANAGEMENT, GVirConfigDomainPowerManagementClass))
#define GVIR_CONFIG_IS_DOMAIN_POWER_MANAGEMENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_POWER_MANAGEMENT))
#define GVIR_CONFIG_IS_DOMAIN_POWER_MANAGEMENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_POWER_MANAGEMENT))
#define GVIR_CONFIG_DOMAIN_POWER_MANAGEMENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_POWER_MANAGEMENT, GVirConfigDomainPowerManagementClass))

typedef struct _GVirConfigDomainPowerManagement GVirConfigDomainPowerManagement;
typedef struct _GVirConfigDomainPowerManagementPrivate GVirConfigDomainPowerManagementPrivate;
typedef struct _GVirConfigDomainPowerManagementClass GVirConfigDomainPowerManagementClass;

struct _GVirConfigDomainPowerManagement
{
    GVirConfigObject parent;

    GVirConfigDomainPowerManagementPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainPowerManagementClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_power_management_get_type(void);

GVirConfigDomainPowerManagement *gvir_config_domain_power_management_new(void);
GVirConfigDomainPowerManagement *gvir_config_domain_power_management_new_from_xml(const gchar *xml,
                                                                                  GError **error);
void gvir_config_domain_power_management_set_mem_suspend_enabled(GVirConfigDomainPowerManagement *pm,
                                                                 gboolean enabled);
void gvir_config_domain_power_management_set_disk_suspend_enabled(GVirConfigDomainPowerManagement *pm,
                                                                  gboolean enabled);


G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_POWER_MANAGEMENT_H__ */
