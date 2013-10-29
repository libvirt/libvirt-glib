/*
 * libvirt-gconfig-domain-interface-network-filterref.h: libvirt filter reference config
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#include <libvirt-gconfig/libvirt-gconfig-domain-interface-filterref-parameter.h>

#ifndef __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_FILTERREF_H__
#define __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_FILTERREF_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF            (gvir_config_domain_interface_filterref_get_type ())
#define GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF, GVirConfigDomainInterfaceFilterref))
#define GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF, GVirConfigDomainInterfaceFilterrefClass))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE_FILTERREF(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF))
#define GVIR_CONFIG_IS_DOMAIN_INTERFACE_FILTERREF_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF))
#define GVIR_CONFIG_DOMAIN_INTERFACE_FILTERREF_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_INTERFACE_FILTERREF, GVirConfigDomainInterfaceFilterrefClass))

typedef struct _GVirConfigDomainInterfaceFilterref GVirConfigDomainInterfaceFilterref;
typedef struct _GVirConfigDomainInterfaceFilterrefPrivate GVirConfigDomainInterfaceFilterrefPrivate;
typedef struct _GVirConfigDomainInterfaceFilterrefClass GVirConfigDomainInterfaceFilterrefClass;

struct _GVirConfigDomainInterfaceFilterref
{
    GVirConfigObject parent;

    GVirConfigDomainInterfaceFilterrefPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainInterfaceFilterrefClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_interface_filterref_get_type(void);

GVirConfigDomainInterfaceFilterref *gvir_config_domain_interface_filterref_new(void);

GVirConfigDomainInterfaceFilterref *
gvir_config_domain_interface_filterref_new_from_xml(const gchar *xml, GError **error);

void gvir_config_domain_interface_filterref_set_name(GVirConfigDomainInterfaceFilterref *filterref,
                                                       const char *filter);
const char *gvir_config_domain_interface_filterref_get_name(GVirConfigDomainInterfaceFilterref *filterref);

void gvir_config_domain_interface_filterref_add_parameter(GVirConfigDomainInterfaceFilterref *filterref,
                                                          GVirConfigDomainInterfaceFilterrefParameter *parameter);
GList *gvir_config_domain_interface_filterref_get_parameters(GVirConfigDomainInterfaceFilterref *filterref);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_INTERFACE_FILTERREF_H__ */
