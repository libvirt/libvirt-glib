/*
 * libvirt-gconfig-domain.h: libvirt domain configuration
 *
 * Copyright (C) 2010-2011 Red Hat, Inc.
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

#include <libvirt-gconfig/libvirt-gconfig-domain-clock.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-os.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-device.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-seclabel.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-cpu.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-power-management.h>

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN            (gvir_config_domain_get_type ())
#define GVIR_CONFIG_DOMAIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN, GVirConfigDomain))
#define GVIR_CONFIG_DOMAIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN, GVirConfigDomainClass))
#define GVIR_CONFIG_IS_DOMAIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN))
#define GVIR_CONFIG_IS_DOMAIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN))
#define GVIR_CONFIG_DOMAIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN, GVirConfigDomainClass))

typedef struct _GVirConfigDomain GVirConfigDomain;
typedef struct _GVirConfigDomainPrivate GVirConfigDomainPrivate;
typedef struct _GVirConfigDomainClass GVirConfigDomainClass;

struct _GVirConfigDomain
{
    GVirConfigObject parent;

    GVirConfigDomainPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_VIRT_QEMU,
    GVIR_CONFIG_DOMAIN_VIRT_KQEMU,
    GVIR_CONFIG_DOMAIN_VIRT_KVM,
    GVIR_CONFIG_DOMAIN_VIRT_XEN,
    GVIR_CONFIG_DOMAIN_VIRT_LXC,
    GVIR_CONFIG_DOMAIN_VIRT_UML,
    GVIR_CONFIG_DOMAIN_VIRT_OPENVZ,
    GVIR_CONFIG_DOMAIN_VIRT_VSERVER,
    GVIR_CONFIG_DOMAIN_VIRT_LDOM,
    GVIR_CONFIG_DOMAIN_VIRT_TEST,
    GVIR_CONFIG_DOMAIN_VIRT_VMWARE,
    GVIR_CONFIG_DOMAIN_VIRT_HYPERV,
    GVIR_CONFIG_DOMAIN_VIRT_VBOX,
    GVIR_CONFIG_DOMAIN_VIRT_ONE,
    GVIR_CONFIG_DOMAIN_VIRT_PHYP
} GVirConfigDomainVirtType;

typedef enum {
    GVIR_CONFIG_DOMAIN_LIFECYCLE_ON_POWEROFF, /*< nick=on_poweroff >*/
    GVIR_CONFIG_DOMAIN_LIFECYCLE_ON_REBOOT, /*< nick=on_reboot >*/
    GVIR_CONFIG_DOMAIN_LIFECYCLE_ON_CRASH /*< nick=on_crash >*/
} GVirConfigDomainLifecycleEvent;

typedef enum {
    GVIR_CONFIG_DOMAIN_LIFECYCLE_DESTROY,
    GVIR_CONFIG_DOMAIN_LIFECYCLE_RESTART,
    GVIR_CONFIG_DOMAIN_LIFECYCLE_PRESERVE,
    GVIR_CONFIG_DOMAIN_LIFECYCLE_RENAME_RESTART,
    GVIR_CONFIG_DOMAIN_LIFECYCLE_COREDUMP_DESTROY,
    GVIR_CONFIG_DOMAIN_LIFECYCLE_COREDUMP_RESTART
} GVirConfigDomainLifecycleAction;

GType gvir_config_domain_get_type(void);

GVirConfigDomain *gvir_config_domain_new_from_xml(const gchar *xml, GError **error);
GVirConfigDomain *gvir_config_domain_new(void);

GVirConfigDomainVirtType gvir_config_domain_get_virt_type(GVirConfigDomain *domain);
void gvir_config_domain_set_virt_type(GVirConfigDomain *domain, GVirConfigDomainVirtType type);
const char *gvir_config_domain_get_name(GVirConfigDomain *domain);
void gvir_config_domain_set_name(GVirConfigDomain *domain, const char *name);
const char *gvir_config_domain_get_uuid(GVirConfigDomain *domain);
void gvir_config_domain_set_uuid(GVirConfigDomain *domain, const char *uuid);
const char *gvir_config_domain_get_description(GVirConfigDomain *domain);
void gvir_config_domain_set_description(GVirConfigDomain *domain, const char *description);
guint64 gvir_config_domain_get_memory(GVirConfigDomain *domain);
void gvir_config_domain_set_memory(GVirConfigDomain *domain, guint64 memory);
guint64 gvir_config_domain_get_current_memory(GVirConfigDomain *domain);
void gvir_config_domain_set_current_memory(GVirConfigDomain *domain, guint64 memory);
guint64 gvir_config_domain_get_vcpus(GVirConfigDomain *domain);
void gvir_config_domain_set_vcpus(GVirConfigDomain *domain,
                                  guint64 vcpu_count);
GStrv gvir_config_domain_get_features(GVirConfigDomain *domain);
void gvir_config_domain_set_features(GVirConfigDomain *domain,
                                     const GStrv features);
GVirConfigDomainClock *gvir_config_domain_get_clock(GVirConfigDomain *domain);
void gvir_config_domain_set_clock(GVirConfigDomain *domain,
                                  GVirConfigDomainClock *klock);
GVirConfigDomainOs *gvir_config_domain_get_os(GVirConfigDomain *domain);
void gvir_config_domain_set_os(GVirConfigDomain *domain,
                               GVirConfigDomainOs *os);
void gvir_config_domain_set_seclabel(GVirConfigDomain *domain,
                                     GVirConfigDomainSeclabel *seclabel);
const char *gvir_config_domain_get_title(GVirConfigDomain *domain);
void gvir_config_domain_set_title(GVirConfigDomain *domain, const char *title);
void gvir_config_domain_set_devices(GVirConfigDomain *domain,
                                    GList *devices);
void gvir_config_domain_add_device(GVirConfigDomain *domain,
                                   GVirConfigDomainDevice *device);
GList *gvir_config_domain_get_devices(GVirConfigDomain *domain);
void gvir_config_domain_set_lifecycle(GVirConfigDomain *domain,
                                      GVirConfigDomainLifecycleEvent event,
                                      GVirConfigDomainLifecycleAction action);
gboolean gvir_config_domain_set_custom_xml_ns_children(GVirConfigDomain *domain,
                                                       const gchar *xml,
                                                       const gchar *ns,
                                                       const gchar *ns_uri,
                                                       GError **error);
gboolean gvir_config_domain_set_custom_xml(GVirConfigDomain *domain,
                                           const gchar *xml,
                                           const gchar *ns,
                                           const gchar *ns_uri,
                                           GError **error);
gchar *gvir_config_domain_get_custom_xml(GVirConfigDomain *domain,
                                         const gchar *ns_uri);
GVirConfigDomainCpu *gvir_config_domain_get_cpu(GVirConfigDomain *domain);
void gvir_config_domain_set_cpu(GVirConfigDomain *domain,
                                GVirConfigDomainCpu *cpu);
void gvir_config_domain_set_power_management(GVirConfigDomain *domain,
                                             GVirConfigDomainPowerManagement *pm);

G_END_DECLS
