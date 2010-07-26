/*
 * conn-test.c: test libvirt gobject integration
 *
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

#include <libvirt-gobject/libvirt-gobject.h>

static void
do_connection_open(GObject *source,
                   GAsyncResult *res,
                   gpointer opaque)
{
    VirGConnection *conn = VIR_G_CONNECTION(source);
    GError *err = NULL;
    GMainLoop *loop = opaque;

    if (!vir_g_connection_open_finish(conn, res, &err)) {
        g_error("%s", err->message);
    }
    g_print("Connected to libvirt\n");
    g_main_loop_quit(loop);
}

int main(int argc, char **argv)
{
    VirGConnection *conn;
    GMainLoop *loop;

    vir_g_object_init(&argc, &argv);

    if (argc != 2)
        g_error("syntax: %s URI", argv[0]);

    conn = vir_g_connection_new(argv[1]);

    loop = g_main_loop_new(g_main_context_default(),
                           TRUE);

    vir_g_connection_open(conn, NULL, do_connection_open, loop);
    vir_g_connection_open(conn, NULL, do_connection_open, loop);

    g_main_loop_run(loop);

    return 0;
}

