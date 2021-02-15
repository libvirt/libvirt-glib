/*
 * libvirt-gconfig-object-private.h: base object for XML configuration
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
#pragma once

G_BEGIN_DECLS

#include <libvirt-gconfig/libvirt-gconfig-xml-doc.h>

GVirConfigObject *gvir_config_object_new_from_tree(GType type,
                                                   GVirConfigXmlDoc *doc,
                                                   const char *schema,
                                                   xmlNodePtr tree);

GVirConfigXmlDoc *gvir_config_object_get_xml_doc(GVirConfigObject *config);
xmlNodePtr gvir_config_object_get_xml_node(GVirConfigObject *config);
const char *gvir_config_object_get_node_content(GVirConfigObject *object,
                                                const char *node_name);
guint64 gvir_config_object_get_node_content_uint64(GVirConfigObject *object,
                                                   const char *node_name);
gint gvir_config_object_get_node_content_genum(GVirConfigObject *object,
                                               const char *node_name,
                                               GType enum_type,
                                               gint default_value);
const char *gvir_config_object_get_attribute(GVirConfigObject *object,
                                             const char *node_name,
                                             const char *attr_name);
guint64 gvir_config_object_get_attribute_uint64(GVirConfigObject *object,
                                                const char *node_name,
                                                const char *attr_name,
                                                guint64 default_value);
gint gvir_config_object_get_attribute_genum(GVirConfigObject *object,
                                            const char *node_name,
                                            const char *attr_name,
                                            GType enum_type,
                                            gint default_value);
gboolean gvir_config_object_get_attribute_boolean(GVirConfigObject *object,
                                                  const char *node_name,
                                                  const char *attr_name,
                                                  gboolean default_value);
void gvir_config_object_set_node_content(GVirConfigObject *object,
                                         const char *node_name,
                                         const char *value);
void gvir_config_object_set_node_content_uint64(GVirConfigObject *object,
                                                const char *node_name,
                                                guint64 value);
GVirConfigObject *gvir_config_object_add_child(GVirConfigObject *object,
                                               const char *child_name);
void gvir_config_object_add_child_with_attribute(GVirConfigObject *object,
                                                 const char *child_name,
                                                 const char *attr_name,
                                                 const char *attr_value);
void gvir_config_object_add_child_with_attribute_enum(GVirConfigObject *object,
                                                      const char *child_name,
                                                      const char *attr_name,
                                                      GType attr_type,
                                                      unsigned int attr_value);
GVirConfigObject *gvir_config_object_replace_child(GVirConfigObject *object,
                                                   const char *child_name);
void gvir_config_object_replace_child_with_attribute(GVirConfigObject *object,
                                                     const char *child_name,
                                                     const char *attr_name,
                                                     const char *attr_value);
void gvir_config_object_replace_child_with_attribute_enum(GVirConfigObject *object,
                                                         const char *child_name,
                                                         const char *attr_name,
                                                         GType attr_type,
                                                         unsigned int attr_value);
void gvir_config_object_delete_child(GVirConfigObject *object,
                                     const char *child_name,
                                     const char *ns_href);
void gvir_config_object_delete_children(GVirConfigObject *object,
                                        const char *child_name,
                                        const char *ns_href);
void gvir_config_object_set_child(GVirConfigObject *object,
                                  xmlNodePtr child);

void gvir_config_object_attach_add(GVirConfigObject *parent,
                                   GVirConfigObject *child);
void gvir_config_object_attach_replace(GVirConfigObject *parent,
                                       const char *child_name,
                                       GVirConfigObject *child);
void gvir_config_object_set_attribute(GVirConfigObject *object,
                                      ...) G_GNUC_NULL_TERMINATED;
void gvir_config_object_set_attribute_with_type(GVirConfigObject *object,
                                                ...) G_GNUC_NULL_TERMINATED;
void gvir_config_object_remove_attribute(GVirConfigObject *object,
                                         const char *attr_name);
void gvir_config_object_foreach_child(GVirConfigObject *object,
                                      const char *parent_name,
                                      GVirConfigXmlNodeIterator iter_func,
                                      gpointer opaque);
gboolean gvir_config_object_set_namespace(GVirConfigObject *object,
                                          const char *ns,
                                          const char *ns_uri,
                                          gboolean ns_children);
GVirConfigObject *gvir_config_object_get_child(GVirConfigObject *object,
                                               const gchar *child_name);
GVirConfigObject *gvir_config_object_get_child_with_type(GVirConfigObject *object,
                                                         const gchar *child_name,
                                                         GType child_type);
gboolean gvir_config_object_has_child(GVirConfigObject *object,
                                      const gchar *child_name);

G_END_DECLS
