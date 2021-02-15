/*
 * libvirt-gconfig-main.c: libvirt gconfig integration
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

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-compat.h"

/**
 * gvir_config_init:
 * @argc: (inout): pointer to application's argc
 * @argv: (inout) (array length=argc) (allow-none) (transfer none): pointer to application's argv
 */
void gvir_config_init(int *argc,
                      char ***argv)
{
    GError *err = NULL;
    if (!gvir_config_init_check(argc, argv, &err)) {
        g_error("Could not initialize libvirt-gconfig: %s\n",
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


/**
 * gvir_config_init_check:
 * @argc: (inout): pointer to application's argc
 * @argv: (inout) (array length=argc) (allow-none) (transfer none): pointer to application's argv
 * @err: pointer to a #GError to which a message will be posted on error
 */
gboolean gvir_config_init_check(int *argc G_GNUC_UNUSED,
                                char ***argv G_GNUC_UNUSED,
                                GError **err G_GNUC_UNUSED)
{
    if (getenv("LIBVIRT_GCONFIG_DEBUG"))
        g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
                          gvir_log_handler, (void*)0x1);

    return TRUE;
}
