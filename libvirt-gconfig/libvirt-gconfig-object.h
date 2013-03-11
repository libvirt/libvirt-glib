/*
 * libvirt-gconfig-object.h: base object for XML configuration
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

#ifndef __LIBVIRT_GCONFIG_OBJECT_H__
#define __LIBVIRT_GCONFIG_OBJECT_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_OBJECT            (gvir_config_object_get_type ())
#define GVIR_CONFIG_OBJECT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_OBJECT, GVirConfigObject))
#define GVIR_CONFIG_OBJECT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_OBJECT, GVirConfigObjectClass))
#define GVIR_CONFIG_IS_OBJECT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_OBJECT))
#define GVIR_CONFIG_IS_OBJECT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_OBJECT))
#define GVIR_CONFIG_OBJECT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_OBJECT, GVirConfigObjectClass))

typedef struct _GVirConfigObject GVirConfigObject;
typedef struct _GVirConfigObjectPrivate GVirConfigObjectPrivate;
typedef struct _GVirConfigObjectClass GVirConfigObjectClass;

struct _GVirConfigObject
{
    GObject parent;

    GVirConfigObjectPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigObjectClass
{
    GObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_object_get_type(void);

GVirConfigObject *gvir_config_object_new(GType type,
                                         const char *root_name,
                                         const char *schema);
GVirConfigObject *gvir_config_object_new_from_xml(GType type,
                                                  const char *root_name,
                                                  const char *schema,
                                                  const gchar *xml,
                                                  GError **error);
void gvir_config_object_validate(GVirConfigObject *config,
                                 GError **err);

gchar *gvir_config_object_to_xml(GVirConfigObject *config);

const gchar *gvir_config_object_get_schema(GVirConfigObject *config);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_OBJECT_H__ */
