/*
 * libvirt-gconfig-domain-chardev-source.c: libvirt domain chardev source base class
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

#define GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CHARDEV_SOURCE, GVirConfigDomainChardevSourcePrivate))

struct _GVirConfigDomainChardevSourcePrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GVirConfigDomainChardevSource, gvir_config_domain_chardev_source, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_chardev_source_class_init(GVirConfigDomainChardevSourceClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_chardev_source_init(GVirConfigDomainChardevSource *source)
{
    source->priv = GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE_GET_PRIVATE(source);
}

G_GNUC_INTERNAL GVirConfigDomainChardevSource *
gvir_config_domain_chardev_source_new_from_tree(GVirConfigXmlDoc *doc,
                                                xmlNodePtr tree)
{
    const gchar *type;

    g_return_val_if_fail(GVIR_CONFIG_IS_XML_DOC(doc), NULL);
    g_return_val_if_fail(tree != NULL, NULL);
    g_return_val_if_fail(tree->name != NULL, NULL);

    type = gvir_config_xml_get_attribute_content(tree, "type");

    if (g_str_equal(type, "pty"))
        return gvir_config_domain_chardev_source_pty_new_from_tree(doc, tree);

    g_debug("Unknown chardev source type: %s", type);
    return NULL;
}
