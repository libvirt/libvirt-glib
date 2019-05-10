/*
 * libvirt-gconfig-domain-capabilities-os.c: libvirt OS domain capabilities
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

#define GVIR_CONFIG_DOMAIN_CAPABILITIES_OS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CAPABILITIES_OS, GVirConfigDomainCapabilitiesOsPrivate))

struct _GVirConfigDomainCapabilitiesOsPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainCapabilitiesOs, gvir_config_domain_capabilities_os, GVIR_CONFIG_TYPE_OBJECT);

static GList *
_gvir_config_domain_capabilities_os_get_firmwares(GVirConfigDomainCapabilitiesOs *os);

static void gvir_config_domain_capabilities_os_class_init(GVirConfigDomainCapabilitiesOsClass *klass)
{
    klass->get_firmwares = _gvir_config_domain_capabilities_os_get_firmwares;
}

static void gvir_config_domain_capabilities_os_init(GVirConfigDomainCapabilitiesOs *os)
{
    os->priv = GVIR_CONFIG_DOMAIN_CAPABILITIES_OS_GET_PRIVATE(os);
}

static gboolean add_firmwares(xmlNodePtr node, gpointer opaque)
{
    GList **firmwares = opaque;
    GVirConfigDomainOsFirmware firmware;

    if (node == NULL || node->children == NULL)
        return TRUE;

    firmware = gvir_config_genum_get_value
                        (GVIR_CONFIG_TYPE_DOMAIN_OS_FIRMWARE,
                         (const gchar *)node->children->content,
                         GVIR_CONFIG_DOMAIN_OS_FIRMWARE_BIOS);
    *firmwares = g_list_append(*firmwares, GINT_TO_POINTER(firmware));

    return TRUE;
}

static gboolean search_firmwares(xmlNodePtr node, gpointer opaque)
{
    const gchar *content;

    if (!g_str_equal(node->name, "enum"))
        return TRUE;

    content = gvir_config_xml_get_attribute_content(node, "name");
    if (content == NULL)
        return TRUE;

    if (!g_str_equal(content, "firmware"))
        return TRUE;

    gvir_config_xml_foreach_child(node,
                                  add_firmwares,
                                  opaque);

    return TRUE;
}

static GList *
_gvir_config_domain_capabilities_os_get_firmwares(GVirConfigDomainCapabilitiesOs *os)
{
    GList *firmwares = NULL;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_CAPABILITIES_OS(os), NULL);

    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(os),
                                     NULL,
                                     search_firmwares,
                                     &firmwares);

    return firmwares;
}

/**
 * gvir_config_domain_capabilities_os_get_firmwares:
 *
 * Gets the firmwares supported by @os. The returned list should be freed with
 * g_list_free().
 *
 * Returns: (element-type LibvirtGConfig.DomainOsFirmware) (transfer container):
 * a newly allocated #GList of #GVirConfigDomainOsFirmware.
 */
GList *
gvir_config_domain_capabilities_os_get_firmwares(GVirConfigDomainCapabilitiesOs *os)
{
    return GVIR_CONFIG_DOMAIN_CAPABILITIES_OS_GET_CLASS(os)->get_firmwares(os);
}
