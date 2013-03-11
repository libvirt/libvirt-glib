/*
 * libvirt-gconfig-domain-memballoon.h: libvirt domain memballoon configuration
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

#ifndef __LIBVIRT_GCONFIG_DOMAIN_MEMBALLOON_H__
#define __LIBVIRT_GCONFIG_DOMAIN_MEMBALLOON_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_MEMBALLOON            (gvir_config_domain_memballoon_get_type ())
#define GVIR_CONFIG_DOMAIN_MEMBALLOON(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_MEMBALLOON, GVirConfigDomainMemballoon))
#define GVIR_CONFIG_DOMAIN_MEMBALLOON_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_MEMBALLOON, GVirConfigDomainMemballoonClass))
#define GVIR_CONFIG_IS_DOMAIN_MEMBALLOON(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_MEMBALLOON))
#define GVIR_CONFIG_IS_DOMAIN_MEMBALLOON_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_MEMBALLOON))
#define GVIR_CONFIG_DOMAIN_MEMBALLOON_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_MEMBALLOON, GVirConfigDomainMemballoonClass))

typedef struct _GVirConfigDomainMemballoon GVirConfigDomainMemballoon;
typedef struct _GVirConfigDomainMemballoonPrivate GVirConfigDomainMemballoonPrivate;
typedef struct _GVirConfigDomainMemballoonClass GVirConfigDomainMemballoonClass;

struct _GVirConfigDomainMemballoon
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainMemballoonPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainMemballoonClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_MEMBALLOON_MODEL_NONE,
    GVIR_CONFIG_DOMAIN_MEMBALLOON_MODEL_VIRTIO,
} GVirConfigDomainMemballoonModel;

GType gvir_config_domain_memballoon_get_type(void);

GVirConfigDomainMemballoon *gvir_config_domain_memballoon_new(void);
GVirConfigDomainMemballoon *gvir_config_domain_memballoon_new_from_xml(const gchar *xml,
                                                                       GError **error);
void gvir_config_domain_memballoon_set_model(GVirConfigDomainMemballoon *memballoon,
                                             GVirConfigDomainMemballoonModel model);


G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_MEMBALLOON_H__ */
