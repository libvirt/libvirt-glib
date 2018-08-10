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

#define GVIR_NETWORK_FILTER_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_NETWORK_FILTER, GVirNetworkFilterPrivate))

struct _GVirNetworkFilterPrivate
{
    virNWFilterPtr handle;
    gchar uuid[VIR_UUID_STRING_BUFLEN];
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirNetworkFilter, gvir_network_filter, G_TYPE_OBJECT);


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

    virNWFilterFree(priv->handle);

    G_OBJECT_CLASS(gvir_network_filter_parent_class)->finalize(object);
}


static void gvir_network_filter_constructed(GObject *object)
{
    GVirNetworkFilter *nf = GVIR_NETWORK_FILTER(object);
    GVirNetworkFilterPrivate *priv = nf->priv;

    G_OBJECT_CLASS(gvir_network_filter_parent_class)->constructed(object);

    /* xxx we may want to turn this into an initable */
    if (virNWFilterGetUUIDString(priv->handle, priv->uuid) < 0)
        gvir_warning("Failed to get network filter UUID on %p",
                     priv->handle);
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
}


static void gvir_network_filter_init(GVirNetworkFilter *filter)
{
    filter->priv = GVIR_NETWORK_FILTER_GET_PRIVATE(filter);
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
    const char *name;

    g_return_val_if_fail(GVIR_IS_NETWORK_FILTER(filter), NULL);

    if (!(name = virNWFilterGetName(filter->priv->handle))) {
        gvir_warning("Failed to get network_filter name on %p",
                     filter->priv->handle);
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
 * @err: Place-holder for possible errors
 *
 * Returns: (transfer full): the config. The returned object should be
 * unreffed with g_object_unref() when no longer needed.

 */
GVirConfigNetworkFilter *gvir_network_filter_get_config
                                (GVirNetworkFilter *filter,
                                 guint flags,
                                 GError **err)
{
    GVirNetworkFilterPrivate *priv;
    gchar *xml;

    g_return_val_if_fail(GVIR_IS_NETWORK_FILTER(filter), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = filter->priv;
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
