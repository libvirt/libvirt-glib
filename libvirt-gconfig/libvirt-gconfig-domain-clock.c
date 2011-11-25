/*
 * libvirt-gobject-config-domain-clock.c: libvirt glib integration
 *
 * Copyright (C) 2011 Red Hat
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include <string.h>

#include <libxml/tree.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"

#define GVIR_CONFIG_DOMAIN_CLOCK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_CLOCK, GVirConfigDomainClockPrivate))

struct _GVirConfigDomainClockPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainClock, gvir_config_domain_clock, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_domain_clock_class_init(GVirConfigDomainClockClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainClockPrivate));
}


static void gvir_config_domain_clock_init(GVirConfigDomainClock *klock)
{
    g_debug("Init GVirConfigDomainClock=%p", klock);

    klock->priv = GVIR_CONFIG_DOMAIN_CLOCK_GET_PRIVATE(klock);
}


GVirConfigDomainClock *gvir_config_domain_clock_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_DOMAIN_CLOCK,
                                    "clock", NULL);
    return GVIR_CONFIG_DOMAIN_CLOCK(object);
}

GVirConfigDomainClock *gvir_config_domain_clock_new_from_xml(const gchar *xml,
                                                GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_DOMAIN_CLOCK,
                                             "clock", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_CLOCK(object);
}

void gvir_config_domain_clock_set_offset(GVirConfigDomainClock *klock,
                                         GVirConfigDomainClockOffset offset)
{
    xmlNodePtr node;
    const char *offset_str;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_CLOCK(klock));

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(klock));
    g_return_if_fail(node != NULL);
    offset_str = gvir_config_genum_get_nick(GVIR_TYPE_CONFIG_DOMAIN_CLOCK_OFFSET,
                                             offset);
    g_return_if_fail(offset_str != NULL);
    xmlNewProp(node, (xmlChar*)"offset", (xmlChar*)offset_str);
}

void gvir_config_domain_clock_set_timezone(GVirConfigDomainClock *klock,
                                           const char *tz)
{
    xmlNodePtr node;
    xmlChar *encoded_tz;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_CLOCK(klock));
    g_return_if_fail(tz != NULL);

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(klock));
    if (node == NULL)
        return;

    xmlNewProp(node, (xmlChar*)"offset", (xmlChar*)"timezone");
    encoded_tz = xmlEncodeEntitiesReentrant(node->doc, (xmlChar*)tz);
    xmlNewProp(node, (xmlChar*)"timezone", encoded_tz);
    xmlFree(encoded_tz);
}

void gvir_config_domain_clock_set_variable_offset(GVirConfigDomainClock *klock,
                                                  gint seconds)
{
    xmlNodePtr node;
    char *offset_str;

    g_return_if_fail(GVIR_IS_CONFIG_DOMAIN_CLOCK(klock));

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(klock));
    if (node == NULL)
        return;

    xmlNewProp(node, (xmlChar*)"offset", (xmlChar*)"variable");
    offset_str = g_strdup_printf("%d", seconds);
    xmlNewProp(node, (xmlChar*)"adjustment", (xmlChar*)offset_str);
    g_free(offset_str);
}
