/*
 * libvirt-gconfig-helpers-private.h: libvirt configuration helpers
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_HELPERS_PRIVATE_H__
#define __LIBVIRT_GCONFIG_HELPERS_PRIVATE_H__

G_BEGIN_DECLS

GError *gvir_config_error_new(GQuark domain, gint code,
                              const gchar *format, ...)
    G_GNUC_PRINTF(3, 4);
void gvir_config_set_error(GError **err,
                           GQuark domain, gint code,
                           const gchar *format, ...)
    G_GNUC_PRINTF(4, 5);
void gvir_config_set_error_literal(GError **err,
                                   GQuark domain, gint code,
                                   const gchar *message);
void gvir_config_set_error_valist(GError **err,
                                  GQuark domain, gint code,
                                  const gchar *format,
                                  va_list args)
    G_GNUC_PRINTF(4, 0);
xmlNodePtr gvir_config_xml_parse(const char *xml,
                                 const char *root_node,
                                 GError **err);
typedef gboolean (*GVirConfigXmlNodeIterator)(xmlNodePtr node, gpointer opaque);
void gvir_config_xml_foreach_child(xmlNodePtr node,
                                   GVirConfigXmlNodeIterator iter_func,
                                   gpointer opaque);
xmlNode * gvir_config_xml_get_element (xmlNode *node, ...)
    G_GNUC_NULL_TERMINATED;
const char * gvir_config_xml_get_child_element_content (xmlNode *node,
                                                        const char *child_name);
const char *gvir_config_xml_get_attribute_content(xmlNodePtr node,
                                                  const char *attr_name);
char *gvir_config_xml_node_to_string(xmlNodePtr node);
const char *gvir_config_genum_get_nick (GType enum_type, gint value);
int gvir_config_genum_get_value (GType enum_type, const char *nick, gint default_value);
G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_HELPERS_PRIVATE_H__ */
