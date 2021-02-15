/*
 * libvirt-gconfig-domain-chardev-source-private.h: libvirt domain chardev configuration
 *
 * Copyright (C) 2013 Red Hat, Inc.
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#pragma once

#include <libvirt-gconfig/libvirt-gconfig-xml-doc.h>

G_BEGIN_DECLS

GVirConfigDomainChardevSource *
gvir_config_domain_chardev_source_new_from_tree(GVirConfigXmlDoc *doc,
                                                xmlNodePtr tree);

GVirConfigDomainChardevSource *
gvir_config_domain_chardev_source_pty_new_from_tree(GVirConfigXmlDoc *doc,
                                                    xmlNodePtr tree);

G_END_DECLS
