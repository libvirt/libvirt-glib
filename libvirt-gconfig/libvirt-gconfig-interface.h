/*
 * libvirt-gconfig-interface.c: libvirt interface configuration
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

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_INTERFACE            (gvir_config_interface_get_type ())
#define GVIR_CONFIG_INTERFACE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_INTERFACE, GVirConfigInterface))
#define GVIR_CONFIG_INTERFACE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_INTERFACE, GVirConfigInterfaceClass))
#define GVIR_CONFIG_IS_INTERFACE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_INTERFACE))
#define GVIR_CONFIG_IS_INTERFACE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_INTERFACE))
#define GVIR_CONFIG_INTERFACE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_INTERFACE, GVirConfigInterfaceClass))

typedef struct _GVirConfigInterface GVirConfigInterface;
typedef struct _GVirConfigInterfacePrivate GVirConfigInterfacePrivate;
typedef struct _GVirConfigInterfaceClass GVirConfigInterfaceClass;

struct _GVirConfigInterface
{
    GVirConfigObject parent;

    GVirConfigInterfacePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigInterfaceClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_interface_get_type(void);

GVirConfigInterface *gvir_config_interface_new(void);
GVirConfigInterface *gvir_config_interface_new_from_xml(const gchar *xml,
                                                        GError **error);

G_END_DECLS
