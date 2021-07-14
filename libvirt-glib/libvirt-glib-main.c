/*
 * libvirt-glib-main.c: libvirt glib integration
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

#include <stdlib.h>
#include <stdio.h>
#include <libvirt/virterror.h>
#include <glib/gi18n-lib.h>

#include "libvirt-glib-main.h"

/**
 * SECTION:libvirt-glib-main
 * @short_description: Initialize the library
 * @title: Library initialization
 * @stability: Stable
 * @include: libvirt-glib/libvirt-glib.h
 *
 * The Libvirt GLib library provides glue to integrate core libvirt
 * infrastructure with the GLib library. This enables consistent
 * error reporting procedures and a common event loop implementation
 * for applications.
 *
 * Before using any functions in the Libvirt GLib library, it must be initialized
 * by calling <code>gvir_init</code> or <code>gvir_init_check</code>.
 *
 * <example>
 * <title>Initializing the Libvirt GLib library</title>
 * <programlisting><![CDATA[
 * int main(int argc, char **argv) {
 *   ...setup...
 *   gvir_init(&argc, &argv);
 *   ...more setup...
 *   gtk_main();
 *   return 0;
 * }
 * ]]></programlisting>
 * </example>
 *
 */

static void
gvir_error_func(gpointer opaque G_GNUC_UNUSED,
                virErrorPtr err)
{
    g_debug("Error: %s", err->message);
}



/**
 * gvir_init:
 * @argc: (inout): Address of the argc parameter of your main() function (or 0
 *     if argv is NULL). This will be changed if any arguments were handled.
 * @argv: (array length=argc) (inout) (allow-none) (transfer none): Address of the
 *     <parameter>argv</parameter> parameter of main(), or %NULL. Any options
 *     understood by GTK+ are stripped before return.
 *
 * Call this function before using any other Libvirt GLib functions in your applications.
 * It will initialize everything needed to operate the toolkit and parses some standard
 * command line options.
 *
 * Although you are expected to pass the @argc, @argv parameters from main() to this
 * function, it is possible to pass NULL if @argv is not available or commandline
 * handling is not required.
 *
 * @argc and @argv are adjusted accordingly so your own code will never see those
 * standard arguments.
 *
 * This method will also turn on debug logging of the library if the
 * <literal>LIBVIRT_GLIB_DEBUG</literal> environment variable is set.
 *
 * This function will terminate your program if it was unable to initialize
 * for some reason. If you want the program to fall back to an alternate
 * mode of operation call <code>gvir_init_check</code> instead.
 */
void gvir_init(int *argc,
                char ***argv)
{
    GError *err = NULL;
    if (!gvir_init_check(argc, argv, &err)) {
        g_error("Could not initialize libvirt-glib: %s\n",
                err ? err->message : "<unknown error>");
    }
}


static void gvir_log_handler(const gchar *log_domain G_GNUC_UNUSED,
                             GLogLevelFlags log_level G_GNUC_UNUSED,
                             const gchar *message,
                             gpointer user_data)
{
    if (user_data)
        fprintf(stderr, "%s\n", message);
}


/**
 * gvir_init_check:
 * @argc: (inout): Address of the argc parameter of your main() function (or 0
 *     if argv is NULL). This will be changed if any arguments were handled.
 * @argv: (array length=argc) (inout) (allow-none) (transfer none): Address of the
 *     <parameter>argv</parameter> parameter of main(), or %NULL. Any options
 *     understood by GTK+ are stripped before return.
 * @err: filled with the error information if initialized failed.
 *
 * This function does the same work as gvir_init() with only a single
 * change: It does not terminate the program if the Libvirt GLib library
 * can't be initialized. Instead it returns %FALSE on failure.
 *
 * This way the application can fall back to some other mode of
 * operation.
 *
 * Return value: %TRUE if the library was successfully initialized,
 *     %FALSE otherwise
 */
gboolean gvir_init_check(int *argc G_GNUC_UNUSED,
                         char ***argv G_GNUC_UNUSED,
                         GError **err G_GNUC_UNUSED)
{
    virSetErrorFunc(NULL, gvir_error_func);

    virInitialize();

    if (!bindtextdomain(PACKAGE, LOCALEDIR))
        return FALSE;

    if (getenv("LIBVIRT_GLIB_DEBUG"))
        g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
                          gvir_log_handler, (void*)0x1);

    return TRUE;
}
