/*
 * libvirt-gobject-config-interface-network.c: libvirt glib integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2011 Red Hat
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include <string.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_INTERFACE_NETWORK, GVirConfigDomainInterfaceNetworkPrivate))

struct _GVirConfigDomainInterfaceNetworkPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainInterfaceNetwork, gvir_config_domain_interface_network, GVIR_TYPE_CONFIG_DOMAIN_INTERFACE);


static void gvir_config_domain_interface_network_class_init(GVirConfigDomainInterfaceNetworkClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainInterfaceNetworkPrivate));
}


static void gvir_config_domain_interface_network_init(GVirConfigDomainInterfaceNetwork *conn)
{
    GVirConfigDomainInterfaceNetworkPrivate *priv;

    DEBUG("Init GVirConfigDomainInterfaceNetwork=%p", conn);

    priv = conn->priv = GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}


GVirConfigDomainInterfaceNetwork *gvir_config_domain_interface_network_new(void)
{
    xmlDocPtr doc;
    xmlNodePtr node;

    doc = xmlNewDoc((xmlChar *)"1.0");
    node= xmlNewDocNode(doc, NULL, (xmlChar *)"interface", NULL);
    xmlNewProp(doc->children, (xmlChar*)"type", (xmlChar*)"network");
    xmlDocSetRootElement(doc, node);
    return GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK(g_object_new(GVIR_TYPE_CONFIG_DOMAIN_INTERFACE_NETWORK,
                                                "node", node,
                                                NULL));
}

GVirConfigDomainInterfaceNetwork *gvir_config_domain_interface_network_new_from_xml(const gchar *xml,
                                                                             GError **error)
{
    xmlNodePtr node;

    node = gvir_config_xml_parse(xml, "interface", error);
    if ((error != NULL) && (*error != NULL))
        return NULL;
    xmlNewProp(node, (xmlChar*)"type", (xmlChar*)"network");
    return GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK(g_object_new(GVIR_TYPE_CONFIG_DOMAIN_INTERFACE_NETWORK,
                                                "node", node,
                                                NULL));
}

void gvir_config_domain_interface_network_set_source(GVirConfigDomainInterfaceNetwork *interface,
                                                     const char *source)
{
    xmlNodePtr source_node;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_INTERFACE_NETWORK(interface));

    source_node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(interface),
                                                   "source");
    g_return_if_fail(source_node != NULL);
    xmlNewProp(source_node, (xmlChar*)"network", (xmlChar*)source);
}
