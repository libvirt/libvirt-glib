/*
 * libvirt-glib-event.c: libvirt glib integration
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libvirt/libvirt.h>

#include "libvirt-glib/libvirt-glib.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

struct gvir_event_handle
{
    int watch;
    int fd;
    int events;
    int enabled;
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
    guint source;
    virEventTimeoutCallback cb;
    void *opaque;
    virFreeCallback ff;
};

GMutex *eventlock = NULL;

static int nextwatch = 1;
static unsigned int nhandles = 0;
static struct gvir_event_handle **handles = NULL;

static int nexttimer = 1;
static unsigned int ntimeouts = 0;
static struct gvir_event_timeout **timeouts = NULL;

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

    DEBUG("Dispatch handler %d %d %p\n", data->fd, events, data->opaque);

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

    handles = g_realloc(handles, sizeof(*handles)*(nhandles+1));
    data = g_malloc(sizeof(*data));
    memset(data, 0, sizeof(*data));

    if (events & VIR_EVENT_HANDLE_READABLE)
        cond |= G_IO_IN;
    if (events & VIR_EVENT_HANDLE_WRITABLE)
        cond |= G_IO_OUT;

    data->watch = nextwatch++;
    data->fd = fd;
    data->events = events;
    data->cb = cb;
    data->opaque = opaque;
    data->channel = g_io_channel_unix_new(fd);
    data->ff = ff;

    DEBUG("Add handle %d %d %p\n", data->fd, events, data->opaque);

    data->source = g_io_add_watch(data->channel,
                                  cond,
                                  gvir_event_handle_dispatch,
                                  data);

    handles[nhandles++] = data;

    ret = data->watch;

    g_mutex_unlock(eventlock);

    return ret;
}

static struct gvir_event_handle *
gvir_event_handle_find(int watch)
{
    unsigned int i;
    for (i = 0 ; i < nhandles ; i++)
        if (handles[i]->watch == watch)
            return handles[i];

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
        DEBUG("Update for missing handle watch %d", watch);
        goto cleanup;
    }

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

static int
gvir_event_handle_remove(int watch)
{
    struct gvir_event_handle *data;
    int ret = -1;

    g_mutex_lock(eventlock);

    data = gvir_event_handle_find(watch);
    if (!data) {
        DEBUG("Remove of missing watch %d", watch);
        goto cleanup;
    }

    DEBUG("Remove handle %d %d\n", watch, data->fd);

    if (!data->source)
        goto cleanup;

    g_source_remove(data->source);
    data->source = 0;
    data->events = 0;
    if (data->ff)
        (data->ff)(data->opaque);
    free(data);

    ret = 0;

cleanup:
    g_mutex_unlock(eventlock);
    return ret;
}


static gboolean
gvir_event_timeout_dispatch(void *opaque)
{
    struct gvir_event_timeout *data = opaque;
    DEBUG("Dispatch timeout %p %p %d %p\n", data, data->cb, data->timer, data->opaque);
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

    timeouts = g_realloc(timeouts, sizeof(*timeouts)*(ntimeouts+1));
    data = g_malloc(sizeof(*data));
    memset(data, 0, sizeof(*data));

    data->timer = nexttimer++;
    data->interval = interval;
    data->cb = cb;
    data->opaque = opaque;
    data->ff = ff;
    if (interval >= 0)
        data->source = g_timeout_add(interval,
                                     gvir_event_timeout_dispatch,
                                     data);

    timeouts[ntimeouts++] = data;

    DEBUG("Add timeout %p %d %p %p %d\n", data, interval, cb, opaque, data->timer);

    ret = data->timer;

    g_mutex_unlock(eventlock);

    return ret;
}


static struct gvir_event_timeout *
gvir_event_timeout_find(int timer)
{
    unsigned int i;
    for (i = 0 ; i < ntimeouts ; i++)
        if (timeouts[i]->timer == timer)
            return timeouts[i];

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
        DEBUG("Update of missing timer %d", timer);
        goto cleanup;
    }

    DEBUG("Update timeout %p %d %d\n", data, timer, interval);

    if (interval >= 0) {
        if (data->source)
            goto cleanup;

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

static int
gvir_event_timeout_remove(int timer)
{
    struct gvir_event_timeout *data;
    int ret = -1;

    g_mutex_lock(eventlock);

    data = gvir_event_timeout_find(timer);
    if (!data) {
        DEBUG("Remove of missing timer %d", timer);
        goto cleanup;
    }

    DEBUG("Remove timeout %p %d\n", data, timer);

    if (!data->source)
        goto cleanup;

    g_source_remove(data->source);
    data->source = 0;

    if (data->ff)
        (data->ff)(data->opaque);

    free(data);

    ret = 0;

cleanup:
    g_mutex_unlock(eventlock);
    return ret;
}


void gvir_event_register(void) {
    eventlock = g_mutex_new();
    virEventRegisterImpl(gvir_event_handle_add,
                         gvir_event_handle_update,
                         gvir_event_handle_remove,
                         gvir_event_timeout_add,
                         gvir_event_timeout_update,
                         gvir_event_timeout_remove);
}

