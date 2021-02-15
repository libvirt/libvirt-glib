/*
 * libvirt-glib-error.h: libvirt glib integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010-2011 Red Hat, Inc.
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

#include <glib.h>

G_BEGIN_DECLS

GError *gvir_error_new(GQuark domain,
                       gint code,
                       const gchar *format,
                       ...) G_GNUC_PRINTF(3, 4);

GError *gvir_error_new_literal(GQuark domain,
                               gint code,
                               const gchar *message);

GError *gvir_error_new_valist(GQuark domain,
                              gint code,
                              const gchar *format,
                              va_list args) G_GNUC_PRINTF(3, 0);

void gvir_set_error(GError **error,
                    GQuark domain,
                    gint code,
                    const gchar *format,
                    ...) G_GNUC_PRINTF(4, 5);

void gvir_set_error_literal(GError **error,
                            GQuark domain,
                            gint code,
                            const gchar *message);

void gvir_set_error_valist(GError **error,
                           GQuark domain,
                           gint code,
                           const gchar *format,
                           va_list args) G_GNUC_PRINTF(4, 0);

void gvir_critical(const gchar *format, ...) G_GNUC_PRINTF(1, 2);
void gvir_warning(const gchar *format, ...) G_GNUC_PRINTF(1, 2);

G_END_DECLS
