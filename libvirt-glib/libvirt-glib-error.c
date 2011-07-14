/*
 * libvirt-glib-error.c: libvirt glib integration
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

#include <config.h>

#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

#include "libvirt-glib/libvirt-glib.h"

/**
 * gvir_error_new: (skip)
 * @domain: error domain
 * @code: error code
 * @format: printf()-style format for error message
 * @Varargs: parameters for message format
 *
 * Creates a new #GError with the given @domain and @code,
 * and a message formatted with @format.
 *
 * Returns: a new #GError
 */
GError *gvir_error_new(GQuark domain,
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

    err = gvir_error_new_literal(domain, code, message);

    g_free(message);

    return err;
}

/**
 * gvir_error_new_literal: (skip)
 * @domain: error domain
 * @code: error code
 * @message: error message
 *
 * Creates a new #GError; unlike gvir_error_new(), @message is
 * not a printf()-style format string. Use this function if
 * @message contains text you don't have control over,
 * that could include printf() escape sequences.
 *
 * Returns: a new #GError
 */
GError *gvir_error_new_literal(GQuark domain,
                               gint code,
                               const gchar *message)
{
    virErrorPtr verr = virGetLastError();

    if (!verr)
        return NULL;

    if (message)
        return g_error_new(domain,
                           code,
                           "%s: %s",
                           message,
                           verr->message);
    else
        return g_error_new(domain,
                           code,
                           "%s",
                           message);
}

/**
 * gvir_error_new_valist: (skip)
 * @domain: error domain
 * @code: error code
 * @format: printf()-style format for error message
 * @args: #va_list of parameters for the message format
 *
 * Creates a new #GError with the given @domain and @code,
 * and a message formatted with @format.
 *
 * Returns: a new #GError
 */
GError *gvir_error_new_valist(GQuark domain,
                              gint code,
                              const gchar *format,
                              va_list args)
{
    GError *err;
    gchar *message;

    message = g_strdup_vprintf(format, args);

    err = gvir_error_new_literal(domain, code, message);

    g_free(message);

    return err;
}

