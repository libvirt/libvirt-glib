/*
 * libvirt-gconfig-capabilities.c: libvirt capabilities configuration
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

#ifndef __LIBVIRT_GCONFIG_CAPABILITIES_H__
#define __LIBVIRT_GCONFIG_CAPABILITIES_H__

#include "libvirt-gconfig/libvirt-gconfig-capabilities-host.h"

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_CAPABILITIES            (gvir_config_capabilities_get_type ())
#define GVIR_CONFIG_CAPABILITIES(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_CAPABILITIES, GVirConfigCapabilities))
#define GVIR_CONFIG_CAPABILITIES_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_CAPABILITIES, GVirConfigCapabilitiesClass))
#define GVIR_CONFIG_IS_CAPABILITIES(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_CAPABILITIES))
#define GVIR_CONFIG_IS_CAPABILITIES_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_CAPABILITIES))
#define GVIR_CONFIG_CAPABILITIES_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_CAPABILITIES, GVirConfigCapabilitiesClass))

typedef struct _GVirConfigCapabilities GVirConfigCapabilities;
typedef struct _GVirConfigCapabilitiesPrivate GVirConfigCapabilitiesPrivate;
typedef struct _GVirConfigCapabilitiesClass GVirConfigCapabilitiesClass;

struct _GVirConfigCapabilities
{
    GVirConfigObject parent;

    GVirConfigCapabilitiesPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigCapabilitiesClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_capabilities_get_type(void);

GVirConfigCapabilities *gvir_config_capabilities_new(void);
GVirConfigCapabilities *gvir_config_capabilities_new_from_xml(const gchar *xml,
                                                              GError **error);
GVirConfigCapabilitiesHost *
gvir_config_capabilities_get_host(GVirConfigCapabilities *caps);
GList *gvir_config_capabilities_get_guests(GVirConfigCapabilities *caps);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_CAPABILITIES_H__ */
