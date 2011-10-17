/*
 * libvirt-gconfig-config-object-private.h: base object for XML configuration
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
#ifndef __LIBVIRT_GCONFIG_OBJECT_PRIVATE_H__
#define __LIBVIRT_GCONFIG_OBJECT_PRIVATE_H__

G_BEGIN_DECLS

xmlNodePtr gvir_config_object_get_xml_node(GVirConfigObject *config);
char *gvir_config_object_get_node_content(GVirConfigObject *object,
                                          const char *node_name);
guint64 gvir_config_object_get_node_content_uint64(GVirConfigObject *object,
                                                   const char *node_name);
void gvir_config_object_set_node_content(GVirConfigObject *object,
                                         const char *node_name,
                                         const char *value);
void gvir_config_object_set_node_content_uint64(GVirConfigObject *object,
                                                const char *node_name,
                                                guint64 value);
xmlNodePtr gvir_config_object_replace_child(GVirConfigObject *object,
                                            const char *child_name);
void gvir_config_object_set_child(GVirConfigObject *object,
                                  xmlNodePtr child);


G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_OBJECT_H__ */
