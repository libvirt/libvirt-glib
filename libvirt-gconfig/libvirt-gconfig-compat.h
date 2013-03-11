/*
 * libvirt-gconfig-compat.h: libvirt configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#ifndef __LIBVIRT_GCONFIG_COMPAT_H__
#define __LIBVIRT_GCONFIG_COMPAT_H__

#include <glib-object.h>

#if GLIB_CHECK_VERSION(2, 35, 0)
#define g_type_init()
#endif

#endif /* __LIBVIRT_GCONFIG_COMPAT_H__ */
