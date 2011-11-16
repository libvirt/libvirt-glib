/*
 * libvirt-glib-main.c: libvirt glib integration
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

#include <stdlib.h>
#include <libvirt/virterror.h>

#include "libvirt-glib-main.h"

gboolean debugFlag = FALSE;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

static void
gvir_error_func(gpointer opaque G_GNUC_UNUSED,
                virErrorPtr err)
{
    DEBUG("Error: %s", err->message);
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


gboolean gvir_init_check(int *argc G_GNUC_UNUSED,
                         char ***argv G_GNUC_UNUSED,
                         GError **err G_GNUC_UNUSED)
{
    char *debugEnv = getenv("LIBVIRT_GLIB_DEBUG");
    if (debugEnv && *debugEnv && *debugEnv != '0')
        debugFlag = 1;

    virSetErrorFunc(NULL, gvir_error_func);
    if (!g_thread_supported())
        g_thread_init(NULL);

    virInitialize();

    return TRUE;
}

