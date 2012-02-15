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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <libvirt/virterror.h>

#include "libvirt-glib-main.h"

static void
gvir_error_func(gpointer opaque G_GNUC_UNUSED,
                virErrorPtr err)
{
    g_debug("Error: %s", err->message);
}


void gvir_init(int *argc,
                char ***argv)
{
    GError *err = NULL;
    if (!gvir_init_check(argc, argv, &err)) {
        g_error("Could not initialize libvirt-glib: %s\n",
                err->message);
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


gboolean gvir_init_check(int *argc G_GNUC_UNUSED,
                         char ***argv G_GNUC_UNUSED,
                         GError **err G_GNUC_UNUSED)
{
    virSetErrorFunc(NULL, gvir_error_func);

    /* Threading is always enabled from 2.31.0 onwards */
#if !GLIB_CHECK_VERSION(2, 31, 0)
    if (!g_thread_supported())
        g_thread_init(NULL);
#endif

    virInitialize();

    /* GLib >= 2.31.0 debug is off by default, so we need to
     * enable it. Older versions are on by default, so we need
     * to disable it.
     */
#if GLIB_CHECK_VERSION(2, 31, 0)
    if (getenv("LIBVIRT_GLIB_DEBUG"))
        g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
                          gvir_log_handler, (void*)0x1);
#else
    if (!getenv("LIBVIRT_GLIB_DEBUG"))
        g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
                          gvir_log_handler, NULL);
#endif

    return TRUE;
}
