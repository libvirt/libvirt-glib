/*
 * libvirt-gconfig-capabilities-cpu-arch.c: libvirt guest architecture capabilities
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010-2012 Red Hat, Inc.
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
 * Authors: Zeeshan Ali <zeenix@redhat.com>
 *          Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_CAPABILITIES_GUEST_ARCH_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_ARCH, GVirConfigCapabilitiesGuestArchPrivate))

struct _GVirConfigCapabilitiesGuestArchPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigCapabilitiesGuestArch, gvir_config_capabilities_guest_arch, GVIR_CONFIG_TYPE_OBJECT);

static void gvir_config_capabilities_guest_arch_class_init(GVirConfigCapabilitiesGuestArchClass *klass G_GNUC_UNUSED)
{
}

static void gvir_config_capabilities_guest_arch_init(GVirConfigCapabilitiesGuestArch *arch)
{
    arch->priv = GVIR_CONFIG_CAPABILITIES_GUEST_ARCH_GET_PRIVATE(arch);
}

const gchar *
gvir_config_capabilities_guest_arch_get_name(GVirConfigCapabilitiesGuestArch *arch)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_GUEST_ARCH(arch), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(arch),
                                            NULL,
                                            "name");
}

struct GetDomainData {
    GVirConfigXmlDoc *doc;
    const gchar *schema;
    GList *domains;
};

static gboolean add_domain(xmlNodePtr node, gpointer opaque)
{
    struct GetDomainData* data = (struct GetDomainData*)opaque;
    GVirConfigObject *object;

    if (g_strcmp0((const gchar *)node->name, "domain") != 0)
        return TRUE;

    object = gvir_config_object_new_from_tree
                                (GVIR_CONFIG_TYPE_CAPABILITIES_GUEST_DOMAIN,
                                 data->doc,
                                 data->schema,
                                 node);
    if (object != NULL)
        data->domains = g_list_append(data->domains, object);
    else
        g_debug("Failed to parse %s node", node->name);

    return TRUE;
}

/**
 * gvir_config_capabilities_guest_arch_get_domains:
 *
 * Gets the possible domains for this architecture.
 *
 * Returns: (element-type LibvirtGConfig.CapabilitiesGuestDomain) (transfer full):
 * a newly allocated #GList of #GVirConfigCapabilitiesGuestDomain.
 */
GList *
gvir_config_capabilities_guest_arch_get_domains(GVirConfigCapabilitiesGuestArch *arch)
{
    struct GetDomainData data;

    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_GUEST_ARCH(arch), NULL);

    g_object_get(G_OBJECT(arch), "doc", &data.doc, NULL);
    g_return_val_if_fail(data.doc != NULL, NULL);
    data.schema = gvir_config_object_get_schema(GVIR_CONFIG_OBJECT(arch));
    data.domains = NULL;

    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(arch),
                                     NULL,
                                     add_domain,
                                     &data);
    g_clear_object(&data.doc);

    return data.domains;
}

const gchar *
gvir_config_capabilities_guest_arch_get_emulator(GVirConfigCapabilitiesGuestArch *arch)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_CAPABILITIES_GUEST_ARCH(arch), NULL);

    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(arch),
                                               "emulator");
}
