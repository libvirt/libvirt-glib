/*
 * libvirt-gconfig-domain-smartcard-passthrough.c: libvirt domain smartcard passthrough configuration
 *
 * Copyright (C) 2012 Red Hat, Inc.
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
 * Author: Alexander Larsson <alexl@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_SMARTCARD_PASSTHROUGH_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_PASSTHROUGH, GVirConfigDomainSmartcardPassthroughPrivate))

struct _GVirConfigDomainSmartcardPassthroughPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainSmartcardPassthrough, gvir_config_domain_smartcard_passthrough, GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD);


static void gvir_config_domain_smartcard_passthrough_class_init(GVirConfigDomainSmartcardPassthroughClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_smartcard_passthrough_init(GVirConfigDomainSmartcardPassthrough *smartcard)
{
    smartcard->priv = GVIR_CONFIG_DOMAIN_SMARTCARD_PASSTHROUGH_GET_PRIVATE(smartcard);
}


GVirConfigDomainSmartcardPassthrough *gvir_config_domain_smartcard_passthrough_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_PASSTHROUGH,
                                    "smartcard", NULL);
    gvir_config_object_set_attribute(object, "mode", "passthrough", NULL);
    return GVIR_CONFIG_DOMAIN_SMARTCARD_PASSTHROUGH(object);
}

GVirConfigDomainSmartcardPassthrough *gvir_config_domain_smartcard_passthrough_new_from_xml(const gchar *xml,
                                                                              GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_PASSTHROUGH,
                                             "smartcard", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "mode"), "passthrough") != 0) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
    return GVIR_CONFIG_DOMAIN_SMARTCARD_PASSTHROUGH(object);
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

void
gvir_config_domain_smartcard_passthrough_set_source(GVirConfigDomainSmartcardPassthrough *smartcard,
                                                    GVirConfigDomainChardevSource *source)
{
    xmlNodePtr smartcard_node;
    xmlNodePtr source_node;
    xmlNodePtr child;
    xmlAttrPtr attr;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SMARTCARD_PASSTHROUGH(smartcard));
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CHARDEV_SOURCE(source));

    smartcard_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(smartcard));
    source_node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(source));

    g_return_if_fail((smartcard_node != NULL) && (source_node != NULL));

    for (child = source_node->children; child != NULL; child = child->next) {
        xmlUnlinkNode(child);
        xmlAddChild(smartcard_node, child);
    }

    for (attr = source_node->properties; attr != NULL; attr = attr->next) {
        xmlAttrPtr new_attr;
        new_attr = xmlCopyProp(smartcard_node, attr);
        prepend_prop(smartcard_node, new_attr);
    }
}
