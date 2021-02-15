/*
 * libvirt-gconfig-domain-interface-network.c: libvirt domain interface configuration
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

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_NETWORK            (gvir_config_domain_interface_network_get_type ())
#define GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfaceNetwork))
#define GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfaceNetworkClass))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE_NETWORK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE_NETWORK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE))
#define GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE, GVirConfigDomainInterfaceNetworkClass))

typedef struct _GVirConfigDomainInterfaceNetwork GVirConfigDomainInterfaceNetwork;
typedef struct _GVirConfigDomainInterfaceNetworkPrivate GVirConfigDomainInterfaceNetworkPrivate;
typedef struct _GVirConfigDomainInterfaceNetworkClass GVirConfigDomainInterfaceNetworkClass;

struct _GVirConfigDomainInterfaceNetwork
{
    GVirConfigDomainInterface parent;

    GVirConfigDomainInterfaceNetworkPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainInterfaceNetworkClass
{
    GVirConfigDomainInterfaceClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_interface_network_get_type(void);

GVirConfigDomainInterfaceNetwork *gvir_config_domain_interface_network_new(void);
GVirConfigDomainInterfaceNetwork *gvir_config_domain_interface_network_new_from_xml(const gchar *xml,
                                                                       GError **error);
void gvir_config_domain_interface_network_set_source(GVirConfigDomainInterfaceNetwork *interface,
                                                     const char *source);

G_END_DECLS
