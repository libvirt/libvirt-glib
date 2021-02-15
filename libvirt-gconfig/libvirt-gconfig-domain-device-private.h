/*
 * libvirt-gconfig-domain-device-private.h: libvirt domain device configuration
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
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#pragma once

#include <libvirt-gconfig/libvirt-gconfig-xml-doc.h>

G_BEGIN_DECLS

GVirConfigDomainDevice *
gvir_config_domain_device_new_from_tree(GVirConfigXmlDoc *doc,
                                        xmlNodePtr tree);
GVirConfigDomainDevice *
gvir_config_domain_controller_new_from_tree(GVirConfigXmlDoc *doc,
                                            xmlNodePtr tree);
GVirConfigDomainDevice *
gvir_config_domain_disk_new_from_tree(GVirConfigXmlDoc *doc,
                                      xmlNodePtr tree);
GVirConfigDomainDevice *
gvir_config_domain_filesys_new_from_tree(GVirConfigXmlDoc *doc,
                                         xmlNodePtr tree);
GVirConfigDomainDevice *
gvir_config_domain_graphics_new_from_tree(GVirConfigXmlDoc *doc,
                                          xmlNodePtr tree);
GVirConfigDomainDevice *
gvir_config_domain_hostdev_new_from_tree(GVirConfigXmlDoc *doc,
                                         xmlNodePtr tree);
GVirConfigDomainDevice *
gvir_config_domain_interface_new_from_tree(GVirConfigXmlDoc *doc,
                                           xmlNodePtr tree);
GVirConfigDomainDevice *
gvir_config_domain_smartcard_new_from_tree(GVirConfigXmlDoc *doc,
                                           xmlNodePtr tree);

G_END_DECLS
