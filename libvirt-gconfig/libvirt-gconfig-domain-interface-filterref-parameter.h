/*
 * libvirt-gconfig-domain-interface-filterref-parameter.h:
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF_PARAMETER            (gvir_config_domain_interface_filterref_parameter_get_type ())
#define GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF_PARAMETER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF_PARAMETER, GVirConfigDomainInterfaceFilterrefParameter))
#define GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF_PARAMETER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF_PARAMETER, GVirConfigDomainInterfaceFilterrefParameterClass))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE_FILTERREF_PARAMETER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF_PARAMETER))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE_FILTERREF_PARAMETER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF_PARAMETER))
#define GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF_PARAMETER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF_PARAMETER, GVirConfigDomainInterfaceFilterrefParameterClass))

typedef struct _GVirConfigDomainInterfaceFilterrefParameter GVirConfigDomainInterfaceFilterrefParameter;
typedef struct _GVirConfigDomainInterfaceFilterrefParameterPrivate GVirConfigDomainInterfaceFilterrefParameterPrivate;
typedef struct _GVirConfigDomainInterfaceFilterrefParameterClass GVirConfigDomainInterfaceFilterrefParameterClass;

struct _GVirConfigDomainInterfaceFilterrefParameter
{
    GVirConfigObject parent;

    GVirConfigDomainInterfaceFilterrefParameterPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainInterfaceFilterrefParameterClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_interface_filterref_parameter_get_type(void);

GVirConfigDomainInterfaceFilterrefParameter *gvir_config_domain_interface_filterref_parameter_new(void);

GVirConfigDomainInterfaceFilterrefParameter *
gvir_config_domain_interface_filterref_parameter_new_from_xml(const gchar *xml, GError **error);

void gvir_config_domain_interface_filterref_parameter_set_name(GVirConfigDomainInterfaceFilterrefParameter *parameter, const gchar *name);
void gvir_config_domain_interface_filterref_parameter_set_value(GVirConfigDomainInterfaceFilterrefParameter *parameter, const gchar *value);
const gchar *gvir_config_domain_interface_filterref_parameter_get_name(GVirConfigDomainInterfaceFilterrefParameter *parameter);
const gchar *gvir_config_domain_interface_filterref_parameter_get_value(GVirConfigDomainInterfaceFilterrefParameter *parameter);

G_END_DECLS
