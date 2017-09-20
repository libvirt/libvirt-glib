/*
 * test-events.c: unit tests for libvirt/glib mainloop integration
 *
 * Copyright (C) 2014 Red Hat, Inc.
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include <libvirt-glib/libvirt-glib.h>

#include <libvirt/libvirt.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

static GMainLoop *main_loop;
static int watch_fd;
static int watch_id;
static int timeout_id;

static void watch_destroyed(void *opaque)
{
    gint *watch = opaque;

    g_assert_cmpint(*watch, >=, 0);
    g_assert_cmpint(*watch, ==, watch_id);
    g_message("destroyed watch %d", *watch);
    *watch = -1;
}


static gboolean check_destroyed(gpointer user_data)
{
    gint *id = user_data;

    g_assert_cmpint(*id, ==, -1);

    return G_SOURCE_REMOVE;
}


static void watch_cb(int watch, int fd, int events, void *opaque G_GNUC_UNUSED)
{
    g_assert_cmpint(watch_id, !=, -1);
    g_assert_cmpint(watch, ==, watch_id);
    g_assert_cmpint(fd, ==, watch_fd);

    g_message("got event(s) %x on fd %d (watch %d)", events, fd, watch);
}

static gboolean idle_quit_cb(gpointer user_data G_GNUC_UNUSED)
{
    g_main_loop_quit(main_loop);

    return G_SOURCE_REMOVE;
}

static gboolean test_watch(gpointer user_data G_GNUC_UNUSED)
{
    int removal_status;

    watch_id = virEventAddHandle(watch_fd,
                                 VIR_EVENT_HANDLE_READABLE | VIR_EVENT_HANDLE_WRITABLE,
                                 watch_cb, &watch_id, watch_destroyed);
    virEventUpdateHandle(watch_id, 0);
    removal_status = virEventRemoveHandle(watch_id);
    g_assert_cmpint(removal_status, ==, 0);
    virEventUpdateHandle(watch_id, VIR_EVENT_HANDLE_READABLE);
    removal_status = virEventRemoveHandle(watch_id);
    g_assert_cmpint(removal_status, ==, -1);
    g_idle_add_full(G_PRIORITY_LOW, check_destroyed, &watch_id, NULL);
    g_idle_add_full(G_PRIORITY_LOW,
                    idle_quit_cb,
                    main_loop,
                    NULL);

    return G_SOURCE_REMOVE;
}


static void test_remove_disabled_watch(void)
{
    main_loop = g_main_loop_new(NULL, FALSE);
    watch_fd = open("/dev/null", O_RDONLY);
    g_idle_add(test_watch, NULL);
    g_main_loop_run(main_loop);
    g_main_loop_unref(main_loop);
    close(watch_fd);
    watch_fd = -1;
    g_assert_cmpint(watch_id, ==, -1);
}


static void timeout_destroyed(void *opaque)
{
    gint *timeout = opaque;

    g_assert_cmpint(*timeout, !=, -1);
    g_assert_cmpint(*timeout, ==, timeout_id);
    g_message("destroyed timeout %d", *timeout);
    *timeout = -1;
}


static void timeout_cb(int timer, void *opaque G_GNUC_UNUSED)
{
    g_assert_cmpint(timeout_id, !=, -1);
    g_assert_cmpint(timer, ==, timeout_id);

    g_message("timer %d fired", timer);
}


static gboolean test_timeout(gpointer user_data G_GNUC_UNUSED)
{
    int removal_status;

    timeout_id = virEventAddTimeout(10000, timeout_cb, &timeout_id, timeout_destroyed);
    virEventUpdateTimeout(timeout_id, -1);
    removal_status = virEventRemoveTimeout(timeout_id);
    g_assert_cmpint(removal_status, ==, 0);
    virEventUpdateTimeout(timeout_id, 20000);
    removal_status = virEventRemoveTimeout(timeout_id);
    g_assert_cmpint(removal_status, ==, -1);

    g_idle_add_full(G_PRIORITY_LOW, check_destroyed, &timeout_id, NULL);
    g_idle_add_full(G_PRIORITY_LOW,
                    (GSourceFunc)g_main_loop_quit,
                    main_loop,
                    NULL);

    return G_SOURCE_REMOVE;
}


static void test_remove_disabled_timeout(void)
{
    main_loop = g_main_loop_new(NULL, FALSE);
    g_idle_add(test_timeout, NULL);
    g_main_loop_run(main_loop);
    g_main_loop_unref(main_loop);
    g_assert_cmpint(timeout_id, ==, -1);
}


int main(int argc, char **argv)
{

    gvir_init(&argc, &argv);
    g_test_init(&argc, &argv, NULL);
    gvir_event_register();

    g_test_add_func("/libvirt-glib/remove-disabled-watch",
                    test_remove_disabled_watch);
    g_test_add_func("/libvirt-glib/remove-disabled-timeout",
                    test_remove_disabled_timeout);

    return g_test_run();
}
