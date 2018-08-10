/*
 * libvirt-gconfig-domain-interface-filterref-parameter.c:
 * libvirt filterref parameters
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
 * Authors: Ian Main <imain@redhat.com>
 *          Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF_PARAMETER_GET_PRIVATE(obj) \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF_PARAMETER, GVirConfigDomainInterfaceFilterrefParameterPrivate))

struct _GVirConfigDomainInterfaceFilterrefParameterPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainInterfaceFilterrefParameter, gvir_config_domain_interface_filterref_parameter, GVIR_CONFIG_TYPE_OBJECT);

static void gvir_config_domain_interface_filterref_parameter_class_init(GVirConfigDomainInterfaceFilterrefParameterClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_domain_interface_filterref_parameter_init(GVirConfigDomainInterfaceFilterrefParameter *parameter)
{
    parameter->priv = GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF_PARAMETER_GET_PRIVATE(parameter);
}

GVirConfigDomainInterfaceFilterrefParameter *gvir_config_domain_interface_filterref_parameter_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF_PARAMETER,
                                    "parameter", NULL);
    return GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF_PARAMETER(object);
}

GVirConfigDomainInterfaceFilterrefParameter *
gvir_config_domain_interface_filterref_parameter_new_from_xml(const gchar *xml, GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF_PARAMETER,
                                             "parameter",
                                             NULL,
                                             xml,
                                             error);

    return GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF_PARAMETER(object);
}

void gvir_config_domain_interface_filterref_parameter_set_name(GVirConfigDomainInterfaceFilterrefParameter *parameter, const gchar *name)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE_FILTERREF_PARAMETER(parameter));
    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(parameter),
                                     "name", name, NULL);
}

void gvir_config_domain_interface_filterref_parameter_set_value(GVirConfigDomainInterfaceFilterrefParameter *parameter, const gchar *value)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INTERFACE_FILTERREF_PARAMETER(parameter));
    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(parameter),
                                     "value", value, NULL);
}

const gchar *gvir_config_domain_interface_filterref_parameter_get_name(GVirConfigDomainInterfaceFilterrefParameter *parameter)
{
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(parameter),
                                            NULL,
                                            "name");
}
const gchar *gvir_config_domain_interface_filterref_parameter_get_value(GVirConfigDomainInterfaceFilterrefParameter *parameter)
{
    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(parameter),
                                            NULL,
                                            "value");
}
