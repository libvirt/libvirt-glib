/*
 * libvirt-gobject-domain-config.c: libvirt gobject integration
 *
 * Copyright (C) 2010 Red Hat
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_H__
#define __LIBVIRT_GCONFIG_DOMAIN_H__

G_BEGIN_DECLS

#define GVIR_TYPE_CONFIG_DOMAIN            (gvir_config_domain_get_type ())
#define GVIR_CONFIG_DOMAIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONFIG_DOMAIN, GVirConfigDomain))
#define GVIR_CONFIG_DOMAIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONFIG_DOMAIN, GVirConfigDomainClass))
#define GVIR_IS_CONFIG_DOMAIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONFIG_DOMAIN))
#define GVIR_IS_CONFIG_DOMAIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONFIG_DOMAIN))
#define GVIR_CONFIG_DOMAIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONFIG_DOMAIN, GVirConfigDomainClass))

typedef struct _GVirConfigDomain GVirConfigDomain;
typedef struct _GVirConfigDomainPrivate GVirConfigDomainPrivate;
typedef struct _GVirConfigDomainClass GVirConfigDomainClass;

struct _GVirConfigDomain
{
    GVirConfigObject parent;

    GVirConfigDomainPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_domain_get_type(void);

GVirConfigDomain *gvir_config_domain_new_from_xml(const gchar *xml, GError **error);
GVirConfigDomain *gvir_config_domain_new(void);

char *gvir_config_domain_get_name(GVirConfigDomain *domain);
void gvir_config_domain_set_name(GVirConfigDomain *domain, const char *name);
guint64 gvir_config_domain_get_memory(GVirConfigDomain *domain);
void gvir_config_domain_set_memory(GVirConfigDomain *domain, guint64 memory);
GStrv gvir_config_domain_get_features(GVirConfigDomain *domain);
void gvir_config_domain_set_features(GVirConfigDomain *domain,
                                     const GStrv features);


G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_H__ */
