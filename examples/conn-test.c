/*
 * conn-test.c: test libvirt gobject integration
 *
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

#include <libvirt-gobject/libvirt-gobject.h>

static void
do_connection_open(GObject *source,
                   GAsyncResult *res,
                   gpointer opaque G_GNUC_UNUSED)
{
    GVirConnection *conn = GVIR_CONNECTION(source);
    GError *err = NULL;
    gchar *hv_name = NULL;
    gulong hv_version = 0;
    guint major, minor, micro;

    if (!gvir_connection_open_finish(conn, res, &err)) {
        g_error("%s", err->message);
    }

    g_print("Connected to libvirt\n");

    if (!(hv_name = gvir_connection_get_hypervisor_name(conn, &err))) {
        g_error("%s", err->message);
    }

    g_print("Hypervisor name: %s\n", hv_name);

    if (!(hv_version = gvir_connection_get_version(conn, &err))) {
        g_error("%s", err->message);
    }

    major = hv_version / 1000000;
    hv_version %= 1000000;
    minor = hv_version / 1000;
    micro = hv_version % 1000;
    g_print("Hypervisor version: %u.%u.%u\n", major, minor, micro);

    g_free(hv_name);
    g_object_unref(conn);
}

static void quit(gpointer data,
                 GObject *where_the_object_was G_GNUC_UNUSED)
{
    GMainLoop *loop = data;

    g_main_loop_quit(loop);
}

int main(int argc, char **argv)
{
    GVirConnection *conn;
    GMainLoop *loop;

    gvir_init_object(&argc, &argv);

    if (argc != 2) {
        g_error("syntax: %s URI", argv[0]);
        return 1;
    }

    loop = g_main_loop_new(g_main_context_default(),
                           TRUE);

    conn = gvir_connection_new(argv[1]);
    g_object_weak_ref(G_OBJECT(conn), quit, loop);

    gvir_connection_open_async(conn, NULL, do_connection_open, loop);

    g_main_loop_run(loop);
    g_main_loop_unref(loop);

    return 0;
}
