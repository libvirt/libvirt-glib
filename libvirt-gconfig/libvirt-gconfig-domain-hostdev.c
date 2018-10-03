/*
 * libvirt-gconfig-domain-hostdev.c: libvirt domain hostdev configuration
 *
 * Copyright (C) 2016 Red Hat, Inc.
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
 * Authors: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 *          Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_HOSTDEV_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV, GVirConfigDomainHostdevPrivate))

struct _GVirConfigDomainHostdevPrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GVirConfigDomainHostdev, gvir_config_domain_hostdev, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_hostdev_class_init(GVirConfigDomainHostdevClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_hostdev_init(GVirConfigDomainHostdev *hostdev)
{
    hostdev->priv = GVIR_CONFIG_DOMAIN_HOSTDEV_GET_PRIVATE(hostdev);
}

G_GNUC_INTERNAL GVirConfigDomainDevice *
gvir_config_domain_hostdev_new_from_tree(GVirConfigXmlDoc *doc,
                                         xmlNodePtr tree)
{
    const char *type;
    GType gtype;

    type = gvir_config_xml_get_attribute_content(tree, "type");
    if (type == NULL)
        return NULL;

    if (g_str_equal(type, "usb")) {
        goto unimplemented;
    } else if (g_str_equal(type, "pci")) {
        gtype = GVIR_CONFIG_TYPE_DOMAIN_HOSTDEV_PCI;
    } else if (g_str_equal(type, "scsi")) {
        goto unimplemented;
    } else {
        g_debug("Unknown domain hostdev node: %s", type);
        return NULL;
    }

    return GVIR_CONFIG_DOMAIN_DEVICE(gvir_config_object_new_from_tree(gtype, doc, NULL, tree));

unimplemented:
    g_debug("Parsing of '%s' domain hostdev nodes is unimplemented", type);
    return NULL;
}

/**
 * gvir_config_domain_hostdev_set_boot_order:
 * @hostdev: the host device
 * @order: the boot order
 *
 * If a positive integer is passed as @order, @hostdev is marked bootable and
 * boot order set to @order, otherwise @hostdev is marked to be unbootable.
 */
void gvir_config_domain_hostdev_set_boot_order(GVirConfigDomainHostdev *hostdev,
                                               gint order)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_HOSTDEV(hostdev));

    if (order >= 0) {
        char *order_str = g_strdup_printf("%u", order);

        gvir_config_object_replace_child_with_attribute(GVIR_CONFIG_OBJECT(hostdev),
                                                        "boot",
                                                        "order",
                                                        order_str);
        g_free(order_str);
    } else {
        gvir_config_object_delete_child(GVIR_CONFIG_OBJECT(hostdev),
                                        "boot",
                                        NULL);
    }
}

/**
 * gvir_config_domain_hostdev_get_boot_order:
 * @hostdev: the host device
 *
 * Returns: The boot order if @hostdev is bootable, otherwise a negative integer.
 */
gint gvir_config_domain_hostdev_get_boot_order(GVirConfigDomainHostdev *hostdev)
{
    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(hostdev),
                                                   "boot", "order", -1);
}

/**
 * gvir_config_domain_hostdev_set_readonly:
 * @hostdev: the host device
 * @readonly: the new readonly status
 *
 * Set the readonly status of @hostdev to @readonly.
 */
void gvir_config_domain_hostdev_set_readonly(GVirConfigDomainHostdev *hostdev,
                                             gboolean readonly)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_HOSTDEV(hostdev));

    if (readonly) {
        GVirConfigObject *node;

        node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(hostdev),
                                                "readonly");
        g_object_unref(node);
    } else {
        gvir_config_object_delete_child(GVIR_CONFIG_OBJECT(hostdev),
                                        "readonly", NULL);
    }
}

/**
 * gvir_config_domain_hostdev_get_readonly:
 * @hostdev: the host device
 *
 * Returns: %TRUE if @hostdev is readonly, %FALSE otherwise.
 */
gboolean gvir_config_domain_hostdev_get_readonly(GVirConfigDomainHostdev *hostdev)
{
    return gvir_config_object_has_child(GVIR_CONFIG_OBJECT(hostdev),
                                        "readonly");
}

/**
 * gvir_config_domain_hostdev_set_shareable:
 * @hostdev: the host device
 * @shareable: the new shareable status
 *
 * Set whether or not @hostdev is shared between domains.
 */
void gvir_config_domain_hostdev_set_shareable(GVirConfigDomainHostdev *hostdev,
                                              gboolean shareable)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_HOSTDEV(hostdev));

    if (shareable) {
        GVirConfigObject *node;

        node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(hostdev),
                                                "shareable");
        g_object_unref(node);
    } else {
        gvir_config_object_delete_child(GVIR_CONFIG_OBJECT(hostdev),
                                        "shareable", NULL);
    }
}

/**
 * gvir_config_domain_hostdev_get_shareable:
 * @hostdev: the host device
 *
 * Returns: %TRUE if @hostdev is shared between domains, %FALSE otherwise.
 */
gboolean gvir_config_domain_hostdev_get_shareable(GVirConfigDomainHostdev *hostdev)
{
    return gvir_config_object_has_child(GVIR_CONFIG_OBJECT(hostdev),
                                        "shareable");
}
