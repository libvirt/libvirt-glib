/*
 * libvirt-gconfig-domain-seclabel.h: libvirt domain security label configuration
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
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_SECLABEL            (gvir_config_domain_seclabel_get_type ())
#define GVIR_CONFIG_DOMAIN_SECLABEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_SECLABEL, GVirConfigDomainSeclabel))
#define GVIR_CONFIG_DOMAIN_SECLABEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_SECLABEL, GVirConfigDomainSeclabelClass))
#define GVIR_CONFIG_IS_DOMAIN_SECLABEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_SECLABEL))
#define GVIR_CONFIG_IS_DOMAIN_SECLABEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_SECLABEL))
#define GVIR_CONFIG_DOMAIN_SECLABEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_SECLABEL, GVirConfigDomainSeclabelClass))

typedef struct _GVirConfigDomainSeclabel GVirConfigDomainSeclabel;
typedef struct _GVirConfigDomainSeclabelPrivate GVirConfigDomainSeclabelPrivate;
typedef struct _GVirConfigDomainSeclabelClass GVirConfigDomainSeclabelClass;

struct _GVirConfigDomainSeclabel
{
    GVirConfigObject parent;

    GVirConfigDomainSeclabelPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainSeclabelClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_SECLABEL_DYNAMIC,
    GVIR_CONFIG_DOMAIN_SECLABEL_STATIC
} GVirConfigDomainSeclabelType;

GType gvir_config_domain_seclabel_get_type(void);

GVirConfigDomainSeclabel *gvir_config_domain_seclabel_new(void);
GVirConfigDomainSeclabel *gvir_config_domain_seclabel_new_from_xml(const gchar *xml, GError **error);

void gvir_config_domain_seclabel_set_type(GVirConfigDomainSeclabel *seclabel, GVirConfigDomainSeclabelType type);
void gvir_config_domain_seclabel_set_model(GVirConfigDomainSeclabel *seclabel, const gchar *model);
void gvir_config_domain_seclabel_set_baselabel(GVirConfigDomainSeclabel *seclabel, const gchar *label);
void gvir_config_domain_seclabel_set_label(GVirConfigDomainSeclabel *seclabel, const gchar *label);

G_END_DECLS
