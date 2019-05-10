/*
 * libvirt-gconfig-domain-capabilities.c: libvirt domain capabilities configuration
 *
 * Copyright (C) 2019 Red Hat, Inc.
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
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_CAPABILITIES_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES, GVirConfigDomainCapabilitiesPrivate))

struct _GVirConfigDomainCapabilitiesPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainCapabilities, gvir_config_domain_capabilities, GVIR_CONFIG_TYPE_OBJECT);

static void
gvir_config_domain_capabilities_class_init(GVirConfigDomainCapabilitiesClass *klass G_GNUC_UNUSED)
{
}


static void
gvir_config_domain_capabilities_init(GVirConfigDomainCapabilities *domain_caps)
{
    domain_caps->priv = GVIR_CONFIG_DOMAIN_CAPABILITIES_GET_PRIVATE(domain_caps);
}


GVirConfigDomainCapabilities *
gvir_config_domain_capabilities_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES,
                                    "domainCapabilities",
                                    DATADIR "/libvirt/schemas/domaincaps.rng");
    return GVIR_CONFIG_DOMAIN_CAPABILITIES(object);
}

GVirConfigDomainCapabilities *
gvir_config_domain_capabilities_new_from_xml(const gchar *xml,
                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES,
                                             "domainCapabilities",
                                             DATADIR "/libvirt/schemas/domaincaps.rng",
                                             xml, error);
    return GVIR_CONFIG_DOMAIN_CAPABILITIES(object);
}

/**
 * gvir_config_domain_capabilities_get_os:
 * @domain_caps: a #GVirConfigDomainCapabilities
 *
 * Gets the @os associated with the @domain_caps.
 *
 * Returns: (transfer full): a new #GVirConfigDomainCapabilitiesOs.
 */
GVirConfigDomainCapabilitiesOs *
gvir_config_domain_capabilities_get_os(GVirConfigDomainCapabilities *domain_caps)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_CAPABILITIES(domain_caps), NULL);

    object = gvir_config_object_get_child_with_type
                                (GVIR_CONFIG_OBJECT(domain_caps),
                                "os",
                                GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES_OS);

    return GVIR_CONFIG_DOMAIN_CAPABILITIES_OS(object);
}
