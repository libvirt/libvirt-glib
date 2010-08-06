/*
 * libvirt-glib-error.h: libvirt glib integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010 Red Hat
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#ifndef __LIBVIRT_GLIB_ERROR_H__
#define __LIBVIRT_GLIB_ERROR_H__

#include <glib.h>

G_BEGIN_DECLS

GError *gvir_error_new(GQuark domain,
                       gint code,
                       const gchar *format,
                       ...);

GError *gvir_error_new_literal(GQuark domain,
                               gint code,
                               const gchar *message);

GError *gvir_error_new_valist(GQuark domain,
                              gint code,
                              const gchar *format,
                              va_list args);

G_END_DECLS

#endif /* __LIBVIRT_GLIB_ERROR_H__ */
