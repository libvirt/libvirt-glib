/*
 * libvirt-gconfig-helpers.h: various GVirConfig helpers
 *
 * Copyright (C) 2010, 2011 Red Hat
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
 * Authors: Daniel P. Berrange <berrange@redhat.com>
 *          Christophe Fergeau <cfergeau@gmail.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_HELPERS_H__
#define __LIBVIRT_GCONFIG_HELPERS_H__

G_BEGIN_DECLS

GQuark gvir_config_object_error_quark(void);
#define GVIR_CONFIG_OBJECT_ERROR gvir_config_object_error_quark()

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_HELPERS_H__ */
