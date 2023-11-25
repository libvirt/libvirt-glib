/*
 * libvirt-gconfig-helpers.c: libvirt configuration helpers
 *
 * Copyright (C) 2010, 2011 Red Hat, Inc.
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
 * Authors: Daniel P. Berrange <berrange@redhat.com>
 *          Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include <string.h>

#include <libxml/parser.h>
#include <libxml/xmlerror.h>
#include <glib/gi18n-lib.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-helpers-private.h"

GQuark
gvir_config_object_error_quark(void)
{
    return g_quark_from_static_string("gvir-config-object");
}

static GError *gvir_config_error_new_literal(GQuark domain,
                                             gint code,
                                             const gchar *message)
{
    const xmlError *xerr = xmlGetLastError();

    if (!xerr)
        return NULL;

    if (message)
        return g_error_new(domain,
                           code,
                           "%s: %s",
                           message,
                           xerr->message);
    else
        return g_error_new(domain,
                           code,
                           "%s",
                           xerr->message);
}


GError *gvir_config_error_new(GQuark domain,
                              gint code,
                              const gchar *format,
                              ...)
{
    GError *err;
    va_list args;
    gchar *message;

    va_start(args, format);
    message = g_strdup_vprintf(format, args);
    va_end(args);

    err = gvir_config_error_new_literal(domain, code, message);

    g_free(message);

    return err;
}


void gvir_config_set_error(GError **err,
                           GQuark domain, gint code,
                           const gchar *format, ...)
{
    va_list args;
    gchar *message;

    if (!err)
        return;

    va_start(args, format);
    message = g_strdup_vprintf(format, args);
    va_end(args);

    *err = gvir_config_error_new_literal(domain, code, message);

    g_free(message);
}


void gvir_config_set_error_literal(GError **err,
                                   GQuark domain, gint code,
                                   const gchar *message)
{
    if (!err)
        return;

    *err = gvir_config_error_new_literal(domain, code, message);
}


void gvir_config_set_error_valist(GError **err,
                                  GQuark domain, gint code,
                                  const gchar *format,
                                  va_list args)
{
    gchar *message;

    if (!err)
        return;

    message = g_strdup_vprintf(format, args);

    *err = gvir_config_error_new_literal(domain, code, message);

    g_free(message);
}

xmlNodePtr
gvir_config_xml_parse(const char *xml, const char *root_node, GError **err)
{
    xmlDocPtr doc;

    if (!xml) {
        *err = g_error_new(GVIR_CONFIG_OBJECT_ERROR,
                           0,
                           "%s",
                           _("No XML document to parse"));
        return NULL;
    }

    doc = xmlParseMemory(xml, strlen(xml));
    if (!doc) {
        gvir_config_set_error_literal(err, GVIR_CONFIG_OBJECT_ERROR,
                                      0,
                                      _("Unable to parse configuration"));
        return NULL;
    }
    if ((!doc->children) ||
         ((root_node != NULL) && g_strcmp0((char *)doc->children->name, root_node) != 0)) {
        g_set_error(err,
                    GVIR_CONFIG_OBJECT_ERROR,
                    0,
                    _("XML data has no '%s' node"),
                    root_node);
        xmlFreeDoc(doc);
        return NULL;
    }

    return doc->children;
}

void gvir_config_xml_foreach_child(xmlNodePtr node,
                                   GVirConfigXmlNodeIterator iter_func,
                                   gpointer opaque)
{
    xmlNodePtr it;

    g_return_if_fail(iter_func != NULL);

    it = node->children;
    while (it != NULL) {
        gboolean cont;
        xmlNodePtr next = it->next;

        if (!xmlIsBlankNode(it)) {
            cont = iter_func(it, opaque);
            if (!cont)
                break;
        }

        it = next;
    }
}

/*
 * gvir_config_xml_get_element is
 *
 * Copyright (C) 2006, 2007 OpenedHand Ltd.
 *
 * Author: Jorn Baayen <jorn@openedhand.com>
 */
xmlNode *
gvir_config_xml_get_element (xmlNode *node, ...)
{
        va_list var_args;

        va_start (var_args, node);

        while (TRUE) {
                const char *arg;

                arg = va_arg (var_args, const char *);
                if (!arg)
                        break;

                for (node = node->children; node; node = node->next)
                        if (!g_strcmp0 (arg, (char *) node->name))
                                break;

                if (!node)
                        break;
        }

        va_end (var_args);

        return node;
}

G_GNUC_INTERNAL const char *
gvir_config_xml_get_child_element_content (xmlNode *node,
                                           const char *child_name)
{
    xmlNode *child_node;

    child_node = gvir_config_xml_get_element(node, child_name, NULL);
    if (!child_node || !(child_node->children))
        return NULL;

    return (const char *)child_node->children->content;
}

G_GNUC_INTERNAL const char *
gvir_config_xml_get_attribute_content(xmlNodePtr node, const char *attr_name)
{
    xmlAttr *attr;

    for (attr = node->properties; attr; attr = attr->next)
        if (g_strcmp0 (attr_name, (char *)attr->name) == 0)
            return (const char *)attr->children->content;

    return NULL;
}

const char *gvir_config_genum_get_nick (GType enum_type, gint value)
{
    GEnumClass *enum_class;
    GEnumValue *enum_value;

    g_return_val_if_fail (G_TYPE_IS_ENUM (enum_type), NULL);

    enum_class = g_type_class_ref(enum_type);
    enum_value = g_enum_get_value(enum_class, value);
    g_type_class_unref(enum_class);

    if (enum_value != NULL)
        return enum_value->value_nick;

    g_return_val_if_reached(NULL);
}

G_GNUC_INTERNAL int
gvir_config_genum_get_value (GType enum_type, const char *nick,
                             gint default_value)
{
    GEnumClass *enum_class;
    GEnumValue *enum_value;

    g_return_val_if_fail(G_TYPE_IS_ENUM(enum_type), default_value);
    g_return_val_if_fail(nick != NULL, default_value);

    enum_class = g_type_class_ref(enum_type);
    enum_value = g_enum_get_value_by_nick(enum_class, nick);
    g_type_class_unref(enum_class);

    if (enum_value != NULL)
        return enum_value->value;

    g_return_val_if_reached(default_value);
}

G_GNUC_INTERNAL char *
gvir_config_xml_node_to_string(xmlNodePtr node)
{
    xmlBufferPtr xmlbuf;
    char *xml;

    if (node == NULL)
        return NULL;

    xmlbuf = xmlBufferCreate();
    if (xmlNodeDump(xmlbuf, node->doc, node, 0, 1) < 0)
        xml = NULL;
    else
        xml = g_strndup((gchar *)xmlBufferContent(xmlbuf), xmlBufferLength(xmlbuf));

    xmlBufferFree(xmlbuf);

    return xml;
}
