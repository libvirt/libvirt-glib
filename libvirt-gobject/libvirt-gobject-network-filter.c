/*
 * libvirt-gobject-network_filter.c: libvirt glib integration
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject-compat.h"

#define GVIR_NETWORK_FILTER_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_NETWORK_FILTER, GVirNetworkFilterPrivate))

struct _GVirNetworkFilterPrivate
{
    virNWFilterPtr handle;
    gchar uuid[VIR_UUID_STRING_BUFLEN];
};

G_DEFINE_TYPE(GVirNetworkFilter, gvir_network_filter, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_NETWORK_FILTER_ERROR gvir_network_filter_error_quark()


static GQuark
gvir_network_filter_error_quark(void)
{
    return g_quark_from_static_string("gvir-network_filter");
}

static void gvir_network_filter_get_property(GObject *object,
                                             guint prop_id,
                                             GValue *value,
                                             GParamSpec *pspec)
{
    GVirNetworkFilter *nf = GVIR_NETWORK_FILTER(object);
    GVirNetworkFilterPrivate *priv = nf->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_network_filter_set_property(GObject *object,
                                             guint prop_id,
                                             const GValue *value,
                                             GParamSpec *pspec)
{
    GVirNetworkFilter *nf = GVIR_NETWORK_FILTER(object);
    GVirNetworkFilterPrivate *priv = nf->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virNWFilterFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_network_filter_finalize(GObject *object)
{
    GVirNetworkFilter *nf = GVIR_NETWORK_FILTER(object);
    GVirNetworkFilterPrivate *priv = nf->priv;

    g_debug("Finalize GVirNetworkFilter=%p", nf);

    virNWFilterFree(priv->handle);

    G_OBJECT_CLASS(gvir_network_filter_parent_class)->finalize(object);
}


static void gvir_network_filter_constructed(GObject *object)
{
    GVirNetworkFilter *nf = GVIR_NETWORK_FILTER(object);
    GVirNetworkFilterPrivate *priv = nf->priv;

    G_OBJECT_CLASS(gvir_network_filter_parent_class)->constructed(object);

    /* xxx we may want to turn this into an initable */
    if (virNWFilterGetUUIDString(priv->handle, priv->uuid) < 0) {
        virErrorPtr verr = virGetLastError();
        if (verr) {
            g_warning("Failed to get network filter UUID on %p: %s",
                      priv->handle, verr->message);
        } else {
            g_warning("Failed to get network filter UUID on %p",
                      priv->handle);
        }
    }
}


static void gvir_network_filter_class_init(GVirNetworkFilterClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_network_filter_finalize;
    object_class->get_property = gvir_network_filter_get_property;
    object_class->set_property = gvir_network_filter_set_property;
    object_class->constructed = gvir_network_filter_constructed;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The network_filter handle",
                                                       GVIR_TYPE_NETWORK_FILTER_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));

    g_type_class_add_private(klass, sizeof(GVirNetworkFilterPrivate));
}


static void gvir_network_filter_init(GVirNetworkFilter *conn)
{
    g_debug("Init GVirNetworkFilter=%p", conn);

    conn->priv = GVIR_NETWORK_FILTER_GET_PRIVATE(conn);
}

typedef struct virNWFilter GVirNetworkFilterHandle;

static GVirNetworkFilterHandle*
gvir_network_filter_handle_copy(GVirNetworkFilterHandle *src)
{
    virNWFilterRef((virNWFilterPtr)src);
    return src;
}

static void
gvir_network_filter_handle_free(GVirNetworkFilterHandle *src)
{
    virNWFilterFree((virNWFilterPtr)src);
}

G_DEFINE_BOXED_TYPE(GVirNetworkFilterHandle, gvir_network_filter_handle,
                    gvir_network_filter_handle_copy, gvir_network_filter_handle_free)

const gchar *gvir_network_filter_get_name(GVirNetworkFilter *filter)
{
    GVirNetworkFilterPrivate *priv = filter->priv;
    const char *name;

    if (!(name = virNWFilterGetName(priv->handle))) {
        g_warning("Failed to get network_filter name on %p", priv->handle);
        return NULL;
    }

    return name;
}


const gchar *gvir_network_filter_get_uuid(GVirNetworkFilter *filter)
{
    g_return_val_if_fail(GVIR_IS_NETWORK_FILTER(filter), NULL);

    return filter->priv->uuid;
}


/**
 * gvir_network_filter_get_config:
 * @filter: the network_filter
 * @flags: the flags
 * Returns: (transfer full): the config
 */
GVirConfigNetworkFilter *gvir_network_filter_get_config
                                (GVirNetworkFilter *filter,
                                 guint flags,
                                 GError **err)
{
    GVirNetworkFilterPrivate *priv = filter->priv;
    gchar *xml;

    if (!(xml = virNWFilterGetXMLDesc(priv->handle, flags))) {
        gvir_set_error_literal(err, GVIR_NETWORK_FILTER_ERROR,
                               0,
                               "Unable to get network_filter XML config");
        return NULL;
    }

    GVirConfigNetworkFilter *conf = gvir_config_network_filter_new_from_xml(xml, err);

    free(xml);
    return conf;
}
