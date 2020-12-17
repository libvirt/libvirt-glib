/*
 * libvirt-gobject-network.c: libvirt glib integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
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

#include <config.h>

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-compat.h"
#include "libvirt-gobject/libvirt-gobject-network-dhcp-lease-private.h"

#define GVIR_NETWORK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_NETWORK, GVirNetworkPrivate))

struct _GVirNetworkPrivate
{
    virNetworkPtr handle;
    gchar uuid[VIR_UUID_STRING_BUFLEN];
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirNetwork, gvir_network, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_NETWORK_ERROR gvir_network_error_quark()


static GQuark
gvir_network_error_quark(void)
{
    return g_quark_from_static_string("gvir-network");
}

static void gvir_network_get_property(GObject *object,
                                      guint prop_id,
                                      GValue *value,
                                      GParamSpec *pspec)
{
    GVirNetwork *network = GVIR_NETWORK(object);
    GVirNetworkPrivate *priv = network->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_network_set_property(GObject *object,
                                      guint prop_id,
                                      const GValue *value,
                                      GParamSpec *pspec)
{
    GVirNetwork *network = GVIR_NETWORK(object);
    GVirNetworkPrivate *priv = network->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virNetworkFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_network_finalize(GObject *object)
{
    GVirNetwork *network = GVIR_NETWORK(object);
    GVirNetworkPrivate *priv = network->priv;

    virNetworkFree(priv->handle);

    G_OBJECT_CLASS(gvir_network_parent_class)->finalize(object);
}

static void gvir_network_constructed(GObject *object)
{
    GVirNetwork *net = GVIR_NETWORK(object);
    GVirNetworkPrivate *priv = net->priv;

    G_OBJECT_CLASS(gvir_network_parent_class)->constructed(object);

    /* xxx we may want to turn this into an initable */
    if (virNetworkGetUUIDString(priv->handle, priv->uuid) < 0)
        gvir_warning("Failed to get network UUID on %p", priv->handle);
}

static void gvir_network_class_init(GVirNetworkClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_network_finalize;
    object_class->get_property = gvir_network_get_property;
    object_class->set_property = gvir_network_set_property;
    object_class->constructed = gvir_network_constructed;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The network handle",
                                                       GVIR_TYPE_NETWORK_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));
}


static void gvir_network_init(GVirNetwork *network)
{
    network->priv = GVIR_NETWORK_GET_PRIVATE(network);
}

typedef struct virNetwork GVirNetworkHandle;

static GVirNetworkHandle*
gvir_network_handle_copy(GVirNetworkHandle *src)
{
    virNetworkRef((virNetworkPtr)src);
    return src;
}

static void
gvir_network_handle_free(GVirNetworkHandle *src)
{
    virNetworkFree((virNetworkPtr)src);
}

G_DEFINE_BOXED_TYPE(GVirNetworkHandle, gvir_network_handle,
                    gvir_network_handle_copy, gvir_network_handle_free)

const gchar *gvir_network_get_name(GVirNetwork *network)
{
    const char *name;

    g_return_val_if_fail(GVIR_IS_NETWORK(network), NULL);

    if (!(name = virNetworkGetName(network->priv->handle))) {
        gvir_warning("Failed to get network name on %p", network->priv->handle);
        return NULL;
    }

    return name;
}


const gchar *gvir_network_get_uuid(GVirNetwork *network)
{
    g_return_val_if_fail(GVIR_IS_NETWORK(network), NULL);

    return network->priv->uuid;
}

/**
 * gvir_network_get_config:
 * @network: the network
 * @flags: the flags
 * @err: Place-holder for possible errors
 *
 * Returns: (transfer full): the config. The returned object should be
 * unreffed with g_object_unref() when no longer needed.
 */
GVirConfigNetwork *gvir_network_get_config(GVirNetwork *network,
                                           guint flags,
                                           GError **err)
{
    GVirNetworkPrivate *priv;
    gchar *xml;

    g_return_val_if_fail(GVIR_IS_NETWORK(network), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = network->priv;
    if (!(xml = virNetworkGetXMLDesc(priv->handle, flags))) {
        gvir_set_error_literal(err, GVIR_NETWORK_ERROR,
                               0,
                               "Unable to get network XML config");
        return NULL;
    }

    GVirConfigNetwork *conf = gvir_config_network_new_from_xml(xml, err);

    free(xml);
    return conf;
}

/**
 * gvir_network_get_dhcp_leases:
 * @network: the network
 * @mac: (allow-none): The optional ASCII formatted MAC address of an interface
 * @flags: placeholder for flags, must be 0
 * @err: Place-holder for possible errors
 *
 * This function fetches leases info of guests in the specified network. If the
 * optional parameter @mac is specified, the returned list will contain only
 * lease info about a specific guest interface with @mac. There can be multiple
 * leases for a single @mac because this API supports DHCPv6 too.
 *
 * Returns:  (element-type LibvirtGObject.NetworkDHCPLease) (transfer full): the
 * list of network leases. Each object in the returned list should be unreffed
 * with g_object_unref() and the list itself using g_list_free, when no longer
 * needed.
 */
GList *gvir_network_get_dhcp_leases(GVirNetwork *network,
                                    const char *mac G_GNUC_UNUSED,
                                    guint flags,
                                    GError **err)
{
    virNetworkDHCPLeasePtr *leases;
    GList *ret = NULL;
    int num_leases, i;

    g_return_val_if_fail(GVIR_IS_NETWORK(network), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);
    g_return_val_if_fail(flags == 0, NULL);

    num_leases = virNetworkGetDHCPLeases(network->priv->handle, mac, &leases, flags);
    if (num_leases < 0) {
        gvir_set_error_literal(err, GVIR_NETWORK_ERROR,
                               0,
                               "Unable to get network DHCP leases");
        return NULL;
    }

    if (num_leases == 0)
        return NULL;

    for (i = 0; i < num_leases; i++) {
        GVirNetworkDHCPLease *lease;

        lease = gvir_network_dhcp_lease_new(leases[i]);
        ret = g_list_prepend(ret, lease);
    }
    free(leases);

    return g_list_reverse(ret);
}
