/*
 * libvirt-glib.h: libvirt glib integration
 *
 * Copyright (C) 2008 Daniel P. Berrange
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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <libvirt/libvirt.h>

static gboolean debugFlag = FALSE;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

struct virHandleGLib
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

static int nextwatch = 1;
static unsigned int nhandles = 0;
static struct virHandleGLib **handles = NULL;

static gboolean
virEventDispatchHandleGLib(GIOChannel *source,
                           GIOCondition condition,
                           gpointer opaque)
{
    struct virHandleGLib *data = opaque;
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


int virEventAddHandleGLib(int fd,
                          int events,
                          virEventHandleCallback cb,
                          void *opaque,
                          virFreeCallback ff)
{
    struct virHandleGLib *data;
    GIOCondition cond = 0;

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
                                  virEventDispatchHandleGLib,
                                  data);

    handles[nhandles++] = data;

    return data->watch;
}

static struct virHandleGLib *
virEventFindHandle(int watch)
{
    unsigned int i;
    for (i = 0 ; i < nhandles ; i++)
        if (handles[i]->watch == watch)
            return handles[i];

    return NULL;
}

void virEventUpdateHandleGLib(int watch,
                              int events)
{
    struct virHandleGLib *data = virEventFindHandle(watch);

    if (!data) {
        DEBUG("Update for missing handle watch %d", watch);
        return;
    }

    if (events) {
        GIOCondition cond = 0;
        if (events == data->events)
            return;

        if (data->source)
            g_source_remove(data->source);

        cond |= G_IO_HUP;
        if (events & VIR_EVENT_HANDLE_READABLE)
            cond |= G_IO_IN;
        if (events & VIR_EVENT_HANDLE_WRITABLE)
            cond |= G_IO_OUT;
        data->source = g_io_add_watch(data->channel,
                                      cond,
                                      virEventDispatchHandleGLib,
                                      data);
        data->events = events;
    } else {
        if (!data->source)
            return;

        g_source_remove(data->source);
        data->source = 0;
        data->events = 0;
    }
}

int virEventRemoveHandleGLib(int watch)
{
    struct virHandleGLib *data = virEventFindHandle(watch);

    if (!data) {
        DEBUG("Remove of missing watch %d", watch);
        return -1;
    }

    DEBUG("Remove handle %d %d\n", watch, data->fd);

    g_source_remove(data->source);
    data->source = 0;
    data->events = 0;
    if (data->ff)
        (data->ff)(data->opaque);
    free(data);

    return 0;
}

struct virTimeoutGLib
{
    int timer;
    int interval;
    guint source;
    virEventTimeoutCallback cb;
    void *opaque;
    virFreeCallback ff;
};


static int nexttimer = 1;
static unsigned int ntimeouts = 0;
static struct virTimeoutGLib **timeouts = NULL;

static gboolean
virEventDispatchTimeoutGLib(void *opaque)
{
    struct virTimeoutGLib *data = opaque;
    DEBUG("Dispatch timeout %p %p %p %p\n", data, data->cb, data->timer, data->opaque);
    (data->cb)(data->timer, data->opaque);

    return TRUE;
}

static int
virEventAddTimeoutGLib(int interval,
                       virEventTimeoutCallback cb,
                       void *opaque,
                       virFreeCallback ff)
{
    struct virTimeoutGLib *data;

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
                                     virEventDispatchTimeoutGLib,
                                     data);

    timeouts[ntimeouts++] = data;

    DEBUG("Add timeout %p %d %p %p %d\n", data, interval, cb, opaque, data->timer);

    return data->timer;
}


static struct virTimeoutGLib *
virEventFindTimer(int timer)
{
    unsigned int i;
    for (i = 0 ; i < ntimeouts ; i++)
        if (timeouts[i]->timer == timer)
            return timeouts[i];

    return NULL;
}


void virEventUpdateTimeoutGLib(int timer,
                               int interval)
{
    struct virTimeoutGLib *data = virEventFindTimer(timer);

    if (!data) {
        DEBUG("Update of missing timer %d", timer);
        return;
    }

    DEBUG("Update timeout %p %d %d\n", data, timer, interval);

    if (interval >= 0) {
        if (data->source)
            return;

        data->interval = interval;
        data->source = g_timeout_add(data->interval,
                                     virEventDispatchTimeoutGLib,
                                     data);
    } else {
        if (!data->source)
            return;

        g_source_remove(data->source);
        data->source = 0;
    }
}

int virEventRemoveTimeoutGLib(int timer)
{
    struct virTimeoutGLib *data = virEventFindTimer(timer);

    if (!data) {
        DEBUG("Remove of missing timer %d", timer);
        return -1;
    }

    DEBUG("Remove timeout %p %d\n", data, timer);

    if (!data->source)
        return -1;

    g_source_remove(data->source);
    data->source = 0;

    if (data->ff)
        (data->ff)(data->opaque);

    free(data);

    return 0;
}


void virEventRegisterGLib(void) {
    char *debugEnv = getenv("LIBVIRT_GLIB_DEBUG");
    if (debugEnv && *debugEnv && *debugEnv != '0')
        debugFlag = 1;

    virEventRegisterImpl(virEventAddHandleGLib,
                         virEventUpdateHandleGLib,
                         virEventRemoveHandleGLib,
                         virEventAddTimeoutGLib,
                         virEventUpdateTimeoutGLib,
                         virEventRemoveTimeoutGLib);
}

