/*
 * libvirt-gconfig-capabilities.c: libvirt capabilities configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_CAPABILITIES_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES, GVirConfigCapabilitiesPrivate))

struct _GVirConfigCapabilitiesPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigCapabilities, gvir_config_capabilities, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_capabilities_class_init(GVirConfigCapabilitiesClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigCapabilitiesPrivate));
}


static void gvir_config_capabilities_init(GVirConfigCapabilities *caps)
{
    g_debug("Init GVirConfigCapabilities=%p", caps);

    caps->priv = GVIR_CONFIG_CAPABILITIES_GET_PRIVATE(caps);
}


GVirConfigCapabilities *gvir_config_capabilities_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_CAPABILITIES,
                                    "capabilities",
                                    DATADIR "/libvirt/schemas/capability.rng");
    return GVIR_CONFIG_CAPABILITIES(object);
}

GVirConfigCapabilities *gvir_config_capabilities_new_from_xml(const gchar *xml,
                                                              GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_CAPABILITIES,
                                             "capabilities",
                                             DATADIR "/libvirt/schemas/capability.rng",
                                             xml, error);
    return GVIR_CONFIG_CAPABILITIES(object);
}

/**
 * gvir_config_capabilities_get_host:
 *
 * Gets the host capabilities.
 *
 * Returns: (transfer full): a new #GVirConfigCapabilitiesHost.
 */
GVirConfigCapabilitiesHost *
gvir_config_capabilities_get_host(GVirConfigCapabilities *caps)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES(caps), NULL);

    object = gvir_config_object_get_child_with_type
                                (GVIR_CONFIG_OBJECT(caps),
                                 "host",
                                 GVIR_CONFIG_TYPE_CAPABILITIES_HOST);

    return GVIR_CONFIG_CAPABILITIES_HOST(object);
}
