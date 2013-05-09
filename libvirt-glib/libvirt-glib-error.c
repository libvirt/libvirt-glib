/*
 * libvirt-glib-error.c: libvirt glib integration
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

#include <config.h>

#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

#include "libvirt-glib/libvirt-glib.h"

/**
 * SECTION:libvirt-glib-error
 * @short_description: Convert libvirt error reports to GLib error reports
 * @title: Error reporting
 * @stability: Stable
 * @include: libvirt-glib/libvirt-glib.h
 *
 * The libvirt API uses the <code>virError</code> structure for reporting
 * errors back to the application programmer. The libvirt API errors are
 * provided in thread-local variables, while the GLib standard practice is
 * to return errors via out parameters. This library provides a simple way
 * to fill in <code>GError **</code> output parameters with the contents
 * of the most recent libvirt error object in the current thread.
 *
 * The <code>gvir_error_new</code>, <code>gvir_error_new_literal</code> and
 * <code>gvir_error_new_valist</code> methods all return a newly created
 * <code>GError *</code> object instance, differing only in the way the
 * message needs to be provided. For most usage though, it is preferrable
 * to use the <code>gvir_set_error</code>, <code>gvir_set_error_literal</code>
 * and <code>gvir_set_error_valist</code> methods. These all accept a
 * <code>GError **</code> argument and take care to only fill it if it
 * points to a non-NULL location.
 *
 * <example>
 * <title>Reporting GLib errors with libvirt APIs</title>
 * <programlisting><![CDATA[
 * gboolean myapp_start_guest(const gchar *xml, GError **error)
 * {
 *     if (virDomainCreate(conn, xml, 0) &lt; 0) {
 *         gvir_set_error_literal(error, "Unable to start virtual machine");
 *        return FALSE;
 *     }
 *
 *     return TRUE;
 * }
 * ]]></programlisting>
 * </example>
 *
 */

/**
 * gvir_error_new:
 * @domain: error domain
 * @code: error code
 * @format: printf()-style format for error message
 * @...: parameters for message format
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
 * gvir_error_new_literal:
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
        return g_error_new(domain,
                           code,
                           "%s",
                           message);

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
                           verr->message);
}

/**
 * gvir_error_new_valist:
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


/**
 * gvir_set_error:
 * @error: pointer to error location
 * @domain: error domain
 * @code: error code
 * @format: printf()-style format for error message
 * @...: parameters for message format
 *
 * If @error is NULL this does nothing. Otherwise it
 * creates a new #GError with the given @domain and @code,
 * and a message formatted with @format, and stores it
 * in @error.
 */
void gvir_set_error(GError **error,
                    GQuark domain,
                    gint code,
                    const gchar *format,
                    ...)
{
    va_list args;
    gchar *message;

    if (!error)
        return;

    va_start(args, format);
    message = g_strdup_vprintf(format, args);
    va_end(args);

    *error = gvir_error_new_literal(domain, code, message);

    g_free(message);
}


/**
 * gvir_set_error_literal:
 * @error: pointer to error location
 * @domain: error domain
 * @code: error code
 * @message: error message
 *
 * If @error is NULL this does nothing. Otherwise it
 * creates a new #GError and stores it in @error; unlike
 * gvir_set_error(), @message is not a printf()-style
 * format string. Use this function if @message contains
 * text you don't have control over, that could include
 * printf() escape sequences.
 */
void gvir_set_error_literal(GError **error,
                            GQuark domain,
                            gint code,
                            const gchar *message)
{
    if (!error)
        return;

    *error = gvir_error_new_literal(domain, code, message);
}


/**
 * gvir_set_error_valist:
 * @error: pointer to error location
 * @domain: error domain
 * @code: error code
 * @format: printf()-style format for error message
 * @args: #va_list of parameters for the message format
 *
 * If @error is NULL this does nothing. Otherwise it
 * creates a new #GError with the given @domain and @code,
 * and a message formatted with @format, and stores it
 * in @error.
 */
void gvir_set_error_valist(GError **error,
                           GQuark domain,
                           gint code,
                           const gchar *format,
                           va_list args)
{
    gchar *message;
    if (!error)
        return;

    message = g_strdup_vprintf(format, args);
    *error = gvir_error_new_literal(domain, code, message);

    g_free(message);
}

static void
gvir_log_valist(GLogLevelFlags level, const gchar *format, va_list args) G_GNUC_PRINTF(2, 0);

static void
gvir_log_valist(GLogLevelFlags level, const gchar *format, va_list args)
{
    gchar *message;
    virErrorPtr verr = virGetLastError();

    message = g_strdup_vprintf(format, args);

    if (verr)
        g_log(G_LOG_DOMAIN, level, "%s: %s", message, verr->message);
    else
        g_log(G_LOG_DOMAIN, level, "%s", message);

    g_free(message);
}

void gvir_warning(const gchar *format, ...)
{
    va_list args;

    va_start(args, format);
    gvir_log_valist(G_LOG_LEVEL_WARNING, format, args);
    va_end(args);
}

void gvir_critical(const gchar *format, ...)
{
    va_list args;

    va_start(args, format);
    gvir_log_valist(G_LOG_LEVEL_CRITICAL, format, args);
    va_end(args);
}
