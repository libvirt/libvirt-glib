/*
 * libvirt-glib-event.c: libvirt glib integration
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libvirt/libvirt.h>

#include "libvirt-glib/libvirt-glib.h"

#ifdef G_OS_WIN32
#include <io.h>
#endif

/**
 * SECTION:libvirt-glib-event
 * @short_description: Integrate libvirt with the GMain event framework
 * @title: Event loop
 * @stability: Stable
 * @include: libvirt-glib/libvirt-glib.h
 *
 * The libvirt API has the ability to provide applications with asynchronous
 * notifications of interesting events. To enable this functionality though,
 * applications must provide libvirt with an event loop implementation. The
 * libvirt-glib API provides such an implementation, which naturally integrates
 * with the GMain event loop framework.
 *
 * To enable use of the GMain event loop glue, the <code>gvir_event_register()</code>
 * should be invoked. Once this is done, it is mandatory to have the default
 * GMain event loop run by a thread in the application, usually the primary
 * thread, eg by using <code>gtk_main()</code> or <code>g_application_run()</code>
 *
 * <example>
 * <title>Registering for events with a GTK application</title>
 * <programlisting><![CDATA[
 * int main(int argc, char **argv) {
 *   ...setup...
 *   gvir_event_register();
 *   ...more setup...
 *   gtk_main();
 *   return 0;
 * }
 * ]]></programlisting>
 * </example>
 *
 * <example>
 * <title>Registering for events using Appplication</title>
 * <programlisting><![CDATA[
 * int main(int argc, char **argv) {
 *   ...setup...
 *   GApplication *app = ...create some impl of GApplication...
 *   gvir_event_register();
 *   ...more setup...
 *   g_application_run(app);
 *   return 0;
 * }
 * ]]></programlisting>
 * </example>
 */


#if GLIB_CHECK_VERSION(2, 31, 0)
#define g_mutex_new() g_new0(GMutex, 1)
#endif

struct gvir_event_handle
{
    int watch;
    int fd;
    int events;
    int removed;
    GIOChannel *channel;
    guint source;
    virEventHandleCallback cb;
    void *opaque;
    virFreeCallback ff;
};

struct gvir_event_timeout
{
    int timer;
    int interval;
    int removed;
    guint source;
    virEventTimeoutCallback cb;
    void *opaque;
    virFreeCallback ff;
};

static GMutex *eventlock = NULL;

static int nextwatch = 1;
static GPtrArray *handles;

static int nexttimer = 1;
static GPtrArray *timeouts;

static gboolean
gvir_event_handle_dispatch(GIOChannel *source G_GNUC_UNUSED,
                           GIOCondition condition,
                           gpointer opaque)
{
    struct gvir_event_handle *data = opaque;
    int events = 0;

    if (condition & G_IO_IN)
        events |= VIR_EVENT_HANDLE_READABLE;
    if (condition & G_IO_OUT)
        events |= VIR_EVENT_HANDLE_WRITABLE;
    if (condition & G_IO_HUP)
        events |= VIR_EVENT_HANDLE_HANGUP;
    if (condition & G_IO_ERR)
        events |= VIR_EVENT_HANDLE_ERROR;

    g_debug("Dispatch handler %p %d %d %d %p\n", data, data->watch, data->fd, events, data->opaque);

    (data->cb)(data->watch, data->fd, events, data->opaque);

    return TRUE;
}


static int
gvir_event_handle_add(int fd,
                      int events,
                      virEventHandleCallback cb,
                      void *opaque,
                      virFreeCallback ff)
{
    struct gvir_event_handle *data;
    GIOCondition cond = 0;
    int ret;

    g_mutex_lock(eventlock);

    data = g_new0(struct gvir_event_handle, 1);

    if (events & VIR_EVENT_HANDLE_READABLE)
        cond |= G_IO_IN;
    if (events & VIR_EVENT_HANDLE_WRITABLE)
        cond |= G_IO_OUT;

    data->watch = nextwatch++;
    data->fd = fd;
    data->events = events;
    data->cb = cb;
    data->opaque = opaque;
#ifdef G_OS_WIN32
    data->channel = g_io_channel_win32_new_socket(_get_osfhandle(fd));
#else
    data->channel = g_io_channel_unix_new(fd);
#endif
    data->ff = ff;

    g_debug("Add handle %p %d %d %d %p\n", data, data->watch, data->fd, events, data->opaque);

    if (events != 0) {
        data->source = g_io_add_watch(data->channel,
                                      cond,
                                      gvir_event_handle_dispatch,
                                      data);
    }

    g_ptr_array_add(handles, data);

    ret = data->watch;

    g_mutex_unlock(eventlock);

    return ret;
}

static struct gvir_event_handle *
gvir_event_handle_find(int watch)
{
    guint i;

    for (i = 0 ; i < handles->len ; i++) {
        struct gvir_event_handle *h = g_ptr_array_index(handles, i);

        if (h == NULL) {
            g_warn_if_reached ();
            continue;
        }

        if ((h->watch == watch) && !h->removed) {
            return h;
        }
    }

    return NULL;
}

static void
gvir_event_handle_update(int watch,
                         int events)
{
    struct gvir_event_handle *data;

    g_mutex_lock(eventlock);

    data = gvir_event_handle_find(watch);
    if (!data) {
        g_debug("Update for missing handle watch %d", watch);
        goto cleanup;
    }

    g_debug("Update handle %p %d %d %d\n", data, watch, data->fd, events);

    if (events) {
        GIOCondition cond = 0;
        if (events == data->events)
            goto cleanup;

        if (data->source)
            g_source_remove(data->source);

        cond |= G_IO_HUP;
        if (events & VIR_EVENT_HANDLE_READABLE)
            cond |= G_IO_IN;
        if (events & VIR_EVENT_HANDLE_WRITABLE)
            cond |= G_IO_OUT;
        data->source = g_io_add_watch(data->channel,
                                      cond,
                                      gvir_event_handle_dispatch,
                                      data);
        data->events = events;
    } else {
        if (!data->source)
            goto cleanup;

        g_source_remove(data->source);
        data->source = 0;
        data->events = 0;
    }

cleanup:
    g_mutex_unlock(eventlock);
}

static gboolean
_event_handle_remove(gpointer data)
{
    struct gvir_event_handle *h = data;

    if (h->ff)
        (h->ff)(h->opaque);

    g_mutex_lock(eventlock);
    g_ptr_array_remove_fast(handles, h);
    g_mutex_unlock(eventlock);

    return FALSE;
}

static int
gvir_event_handle_remove(int watch)
{
    struct gvir_event_handle *data;
    int ret = -1;

    g_mutex_lock(eventlock);

    data = gvir_event_handle_find(watch);
    if (!data) {
        g_debug("Remove of missing watch %d", watch);
        goto cleanup;
    }

    g_debug("Remove handle %p %d %d\n", data, watch, data->fd);

    if (data->source != 0) {
        g_source_remove(data->source);
        data->source = 0;
        data->events = 0;
    }

    g_warn_if_fail(data->channel != NULL);
    g_io_channel_unref(data->channel);
    data->channel = NULL;

    /* since the actual watch deletion is done asynchronously, a handle_update call may
     * reschedule the watch before it's fully deleted, that's why we need to mark it as
     * 'removed' to prevent reuse
     */
    data->removed = TRUE;
    g_idle_add(_event_handle_remove, data);

    ret = 0;

cleanup:
    g_mutex_unlock(eventlock);
    return ret;
}


static gboolean
gvir_event_timeout_dispatch(void *opaque)
{
    struct gvir_event_timeout *data = opaque;
    g_debug("Dispatch timeout %p %p %d %p\n", data, data->cb, data->timer, data->opaque);
    (data->cb)(data->timer, data->opaque);

    return TRUE;
}

static int
gvir_event_timeout_add(int interval,
                       virEventTimeoutCallback cb,
                       void *opaque,
                       virFreeCallback ff)
{
    struct gvir_event_timeout *data;
    int ret;

    g_mutex_lock(eventlock);

    data = g_new0(struct gvir_event_timeout, 1);
    data->timer = nexttimer++;
    data->interval = interval;
    data->cb = cb;
    data->opaque = opaque;
    data->ff = ff;
    if (interval >= 0)
        data->source = g_timeout_add(interval,
                                     gvir_event_timeout_dispatch,
                                     data);

    g_ptr_array_add(timeouts, data);

    g_debug("Add timeout %p %d %p %p %d\n", data, interval, cb, opaque, data->timer);

    ret = data->timer;

    g_mutex_unlock(eventlock);

    return ret;
}


static struct gvir_event_timeout *
gvir_event_timeout_find(int timer)
{
    guint i;

    g_return_val_if_fail(timeouts != NULL, NULL);

    for (i = 0 ; i < timeouts->len ; i++) {
        struct gvir_event_timeout *t = g_ptr_array_index(timeouts, i);

        if (t == NULL) {
            g_warn_if_reached ();
            continue;
        }

        if ((t->timer == timer) && !t->removed) {
            return t;
        }
    }

    return NULL;
}


static void
gvir_event_timeout_update(int timer,
                          int interval)
{
    struct gvir_event_timeout *data;

    g_mutex_lock(eventlock);

    data = gvir_event_timeout_find(timer);
    if (!data) {
        g_debug("Update of missing timer %d", timer);
        goto cleanup;
    }

    g_debug("Update timeout %p %d %d\n", data, timer, interval);

    if (interval >= 0) {
        if (data->source)
            g_source_remove(data->source);

        data->interval = interval;
        data->source = g_timeout_add(data->interval,
                                     gvir_event_timeout_dispatch,
                                     data);
    } else {
        if (!data->source)
            goto cleanup;

        g_source_remove(data->source);
        data->source = 0;
    }

cleanup:
    g_mutex_unlock(eventlock);
}

static gboolean
_event_timeout_remove(gpointer data)
{
    struct gvir_event_timeout *t = data;

    if (t->ff)
        (t->ff)(t->opaque);

    g_mutex_lock(eventlock);
    g_ptr_array_remove_fast(timeouts, t);
    g_mutex_unlock(eventlock);

    return FALSE;
}

static int
gvir_event_timeout_remove(int timer)
{
    struct gvir_event_timeout *data;
    int ret = -1;

    g_mutex_lock(eventlock);

    data = gvir_event_timeout_find(timer);
    if (!data) {
        g_debug("Remove of missing timer %d", timer);
        goto cleanup;
    }

    g_debug("Remove timeout %p %d\n", data, timer);

    if (data->source != 0) {
        g_source_remove(data->source);
        data->source = 0;
    }

    /* since the actual timeout deletion is done asynchronously, a timeout_update call may
     * reschedule the timeout before it's fully deleted, that's why we need to mark it as
     * 'removed' to prevent reuse
     */
    data->removed = TRUE;
    g_idle_add(_event_timeout_remove, data);

    ret = 0;

cleanup:
    g_mutex_unlock(eventlock);
    return ret;
}


static gpointer event_register_once(gpointer data G_GNUC_UNUSED)
{
    eventlock = g_mutex_new();
    timeouts = g_ptr_array_new_with_free_func(g_free);
    handles = g_ptr_array_new_with_free_func(g_free);
    virEventRegisterImpl(gvir_event_handle_add,
                         gvir_event_handle_update,
                         gvir_event_handle_remove,
                         gvir_event_timeout_add,
                         gvir_event_timeout_update,
                         gvir_event_timeout_remove);
    return NULL;
}


/**
 * gvir_event_register:
 *
 * Registers a libvirt event loop implementation that is backed
 * by the default <code>GMain</code> context. If invoked more
 * than once this method will be a no-op. Applications should,
 * however, take care not to register any another non-GLib
 * event loop with libvirt.
 *
 * After invoking this method, it is mandatory to run the
 * default GMain event loop. Typically this can be satisfied
 * by invoking <code>gtk_main</code> or <code>g_application_run</code>
 * in the application's main thread. Failure to run the event
 * loop will mean no libvirt events get dispatched, and the
 * libvirt keepalive timer will kill off libvirt connections
 * frequently.
 */
void gvir_event_register(void)
{
    static GOnce once = G_ONCE_INIT;

    g_once(&once, event_register_once, NULL);
}
