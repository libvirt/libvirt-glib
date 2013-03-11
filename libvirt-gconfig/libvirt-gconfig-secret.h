/*
 * libvirt-gconfig-secret.h: libvirt secret configuration
 *
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_SECRET_H__
#define __LIBVIRT_GCONFIG_SECRET_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_SECRET            (gvir_config_secret_get_type ())
#define GVIR_CONFIG_SECRET(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_SECRET, GVirConfigSecret))
#define GVIR_CONFIG_SECRET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_SECRET, GVirConfigSecretClass))
#define GVIR_CONFIG_IS_SECRET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_SECRET))
#define GVIR_CONFIG_IS_SECRET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_SECRET))
#define GVIR_CONFIG_SECRET_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_SECRET, GVirConfigSecretClass))

typedef struct _GVirConfigSecret GVirConfigSecret;
typedef struct _GVirConfigSecretPrivate GVirConfigSecretPrivate;
typedef struct _GVirConfigSecretClass GVirConfigSecretClass;

struct _GVirConfigSecret
{
    GVirConfigObject parent;

    GVirConfigSecretPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigSecretClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_secret_get_type(void);

GVirConfigSecret *gvir_config_secret_new(void);
GVirConfigSecret *gvir_config_secret_new_from_xml(const gchar *xml,
                                                  GError **error);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_SECRET_H__ */
