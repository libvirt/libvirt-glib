/*
 * libvirt-gconfig-capabilities-host-secmodel.h: libvirt security model capabilities
 *
 * Copyright (C) 2014 SUSE LINUX Products GmbH, Nuernberg, Germany.
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
 * Authors: Cédric Bosdonnat <cbosdonnat@suse.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_CAPABILITIES_HOST_SECMODEL            (gvir_config_capabilities_host_secmodel_get_type ())
#define GVIR_CONFIG_CAPABILITIES_HOST_SECMODEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_HOST_SECMODEL, GVirConfigCapabilitiesHostSecModel))
#define GVIR_CONFIG_CAPABILITIES_HOST_SECMODEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_HOST_SECMODEL, GVirConfigCapabilitiesHostSecModelClass))
#define GVIR_CONFIG_IS_CAPABILITIES_HOST_SECMODEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_HOST_SECMODEL))
#define GVIR_CONFIG_IS_CAPABILITIES_HOST_SECMODEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_CAPABILITIES_HOST_SECMODEL))
#define GVIR_CONFIG_CAPABILITIES_HOST_SECMODEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_CAPABILITIES_HOST_SECMODEL, GVirConfigCapabilitiesHostSecModelClass))

typedef struct _GVirConfigCapabilitiesHostSecModel GVirConfigCapabilitiesHostSecModel;
typedef struct _GVirConfigCapabilitiesHostSecModelPrivate GVirConfigCapabilitiesHostSecModelPrivate;
typedef struct _GVirConfigCapabilitiesHostSecModelClass GVirConfigCapabilitiesHostSecModelClass;

struct _GVirConfigCapabilitiesHostSecModel
{
    GVirConfigObject parent;

    GVirConfigCapabilitiesHostSecModelPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigCapabilitiesHostSecModelClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_config_capabilities_host_secmodel_get_type(void);

const gchar *
gvir_config_capabilities_host_secmodel_get_model(GVirConfigCapabilitiesHostSecModel *secmodel);

const gchar *
gvir_config_capabilities_host_secmodel_get_doi(GVirConfigCapabilitiesHostSecModel *secmodel);

G_END_DECLS
