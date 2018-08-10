/*
 * libvirt-gconfig-domain-interface-network-filterref.c:
 * libvirt filter reference config.
 *
 * Copyright (C) 2013 Red Hat, Inc.
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
 * Author: Ian Main <imain@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF, GVirConfigDomainInterfaceFilterrefPrivate))

struct _GVirConfigDomainInterfaceFilterrefPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainInterfaceFilterref, gvir_config_domain_interface_filterref, GVIR_CONFIG_TYPE_OBJECT);

static void gvir_config_domain_interface_filterref_class_init(GVirConfigDomainInterfaceFilterrefClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_domain_interface_filterref_init(GVirConfigDomainInterfaceFilterref *filterref)
{
    filterref->priv = GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF_GET_PRIVATE(filterref);
}


GVirConfigDomainInterfaceFilterref *gvir_config_domain_interface_filterref_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF,
                                    "filterref", NULL);
    return GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF(object);
}

GVirConfigDomainInterfaceFilterref *gvir_config_domain_interface_filterref_new_from_xml(const gchar *xml,
                                                                                        GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF,
                                             "filterref", NULL, xml, error);
    if (gvir_config_object_get_attribute(object, NULL, "filter") == NULL) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
    return GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF(object);
}

void gvir_config_domain_interface_filterref_set_name(GVirConfigDomainInterfaceFilterref *filterref,
                                                       const char *filter)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE_FILTERREF(filterref));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(filterref),
                                     "filter", filter, NULL);
}

const char *gvir_config_domain_interface_filterref_get_name(GVirConfigDomainInterfaceFilterref *filterref)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE_FILTERREF(filterref), NULL);
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(filterref),
                                            NULL,
                                            "filter");
}

void gvir_config_domain_interface_filterref_add_parameter(GVirConfigDomainInterfaceFilterref *filterref,
                                                          GVirConfigDomainInterfaceFilterrefParameter *parameter)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE_FILTERREF(filterref));
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE_FILTERREF_PARAMETER(parameter));

    gvir_config_object_attach_add(GVIR_CONFIG_OBJECT(filterref),
                                  GVIR_CONFIG_OBJECT(parameter));
}

struct GetParameterData {
    GVirConfigXmlDoc *doc;
    GList *parameters;
};


static gboolean add_filterref_parameter(xmlNodePtr node, gpointer opaque)
{
    struct GetParameterData* data = (struct GetParameterData*)opaque;
    GVirConfigObject *parameter;

    if (g_strcmp0((const gchar *)node->name, "parameter") != 0) {
        g_debug("unexpected node %s", node->name);
        return TRUE;
    }

    parameter = gvir_config_object_new_from_tree(GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF_PARAMETER,
                                                 data->doc, NULL, node);
    if (parameter != NULL)
        data->parameters = g_list_append(data->parameters, parameter);
    else
        g_debug("Failed to parse %s node", node->name);

    return TRUE;
}

/**
 * gvir_config_domain_interface_filterref_get_parameters:
 * @filterref: a #GVirConfigDomainInterfaceFilterref
 *
 * Gets the list of parameters attached to @filterref. The returned list should be
 * freed with g_list_free(), after its elements have been unreffed with
 * g_object_unref().
 *
 * Returns: (element-type LibvirtGConfig.DomainInterfaceFilterrefParameter) (transfer full):
 * a newly allocated #GList of #GVirConfigDomainInterfaceFilterrefParameter.
 */
GList *gvir_config_domain_interface_filterref_get_parameters(GVirConfigDomainInterfaceFilterref *filterref)
{
    struct GetParameterData data;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE_FILTERREF(filterref), NULL);

    g_object_get(G_OBJECT(filterref), "doc", &data.doc, NULL);
    data.parameters = NULL;

    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(filterref),
                                     NULL,
                                     add_filterref_parameter,
                                     &data);

    if (data.doc != NULL) {
        g_object_unref(G_OBJECT(data.doc));
    }

    return data.parameters;
}
