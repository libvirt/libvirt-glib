/*
 * libvirt-gconfig-domain-interface-bridge.c: libvirt domain interface configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_BRIDGE_H__
#define __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_BRIDGE_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_BRIDGE            (gvir_config_domain_interface_bridge_get_type ())
#define GVIR_CONFIG_DOMAIN_INTERFACE_BRIDGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfaceBridge))
#define GVIR_CONFIG_DOMAIN_INTERFACE_BRIDGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfaceBridgeClass))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE_BRIDGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE_BRIDGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE))
#define GVIR_CONFIG_DOMAIN_INTERFACE_BRIDGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfaceBridgeClass))

typedef struct _GVirConfigDomainInterfaceBridge GVirConfigDomainInterfaceBridge;
typedef struct _GVirConfigDomainInterfaceBridgePrivate GVirConfigDomainInterfaceBridgePrivate;
typedef struct _GVirConfigDomainInterfaceBridgeClass GVirConfigDomainInterfaceBridgeClass;

struct _GVirConfigDomainInterfaceBridge
{
    GVirConfigDomainInterface parent;

    GVirConfigDomainInterfaceBridgePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainInterfaceBridgeClass
{
    GVirConfigDomainInterfaceClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_interface_bridge_get_type(void);

GVirConfigDomainInterfaceBridge *gvir_config_domain_interface_bridge_new(void);
GVirConfigDomainInterfaceBridge *gvir_config_domain_interface_bridge_new_from_xml(const gchar *xml,
                                                                                  GError **error);
void gvir_config_domain_interface_bridge_set_source(GVirConfigDomainInterfaceBridge *interface,
                                                    const char *brname);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_BRIDGE_H__ */
