/*
 * libvirt-gconfig-domain-chardev.c: libvirt domain chardev configuration
 *
 * Copyright (C) 2011 Red Hat, Inc.
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"
#include "libvirt-gconfig/libvirt-gconfig-domain-chardev-source-private.h"

#define GVIR_CONFIG_DOMAIN_CHARDEV_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV, GVirConfigDomainChardevPrivate))

struct _GVirConfigDomainChardevPrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GVirConfigDomainChardev, gvir_config_domain_chardev, GVIR_CONFIG_TYPE_DOMAIN_DEVICE)

static void gvir_config_domain_chardev_class_init(GVirConfigDomainChardevClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_chardev_init(GVirConfigDomainChardev *chardev)
{
    chardev->priv = GVIR_CONFIG_DOMAIN_CHARDEV_GET_PRIVATE(chardev);
}

static void prepend_prop(xmlNodePtr node, xmlAttrPtr prop)
{
    if (node->properties == NULL) {
        node->properties = prop;
    } else {
        prop->next = node->properties;
        node->properties->prev = prop;
        node->properties = prop;
    }
}

void gvir_config_domain_chardev_set_source(GVirConfigDomainChardev *chardev,
                                           GVirConfigDomainChardevSource *source)
{
    xmlNodePtr chardev_node;
    xmlNodePtr source_node;
    xmlNodePtr child;
    xmlAttrPtr attr;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CHARDEV(chardev));
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE(source));

    chardev_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(chardev));
    source_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(source));

    g_return_if_fail((chardev_node != NULL) && (source_node != NULL));

    for (child = source_node->children; child != NULL; child = child->next) {
        xmlUnlinkNode(child);
        xmlAddChild(chardev_node, child);
    }

    for (attr = source_node->properties; attr != NULL; attr = attr->next) {
        xmlAttrPtr new_attr;
        new_attr = xmlCopyProp(chardev_node, attr);
        prepend_prop(chardev_node, new_attr);
    }
}


/**
 * gvir_config_domain_chardev_get_source:
 * @chardev: a #GVirConfigDomainChardev
 *
 * Gets the source for the chardev
 *
 * Returns: (transfer full): the chardev source
 */
GVirConfigDomainChardevSource *
gvir_config_domain_chardev_get_source(GVirConfigDomainChardev *chardev)
{
    GVirConfigXmlDoc *doc;
    xmlNodePtr tree;

    doc = gvir_config_object_get_xml_doc(GVIR_CONFIG_OBJECT(chardev));
    tree = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(chardev));

    return gvir_config_domain_chardev_source_new_from_tree(doc,
                                                           tree);
}
