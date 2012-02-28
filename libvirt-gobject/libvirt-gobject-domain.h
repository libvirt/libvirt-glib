/*
 * libvirt-gobject-domain.c: libvirt gobject integration
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#ifndef __LIBVIRT_GOBJECT_DOMAIN_H__
#define __LIBVIRT_GOBJECT_DOMAIN_H__

G_BEGIN_DECLS

#include <libvirt-gobject/libvirt-gobject-stream.h>
#include <libvirt/libvirt.h>

#define GVIR_TYPE_DOMAIN            (gvir_domain_get_type ())
#define GVIR_DOMAIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_DOMAIN, GVirDomain))
#define GVIR_DOMAIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_DOMAIN, GVirDomainClass))
#define GVIR_IS_DOMAIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_DOMAIN))
#define GVIR_IS_DOMAIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_DOMAIN))
#define GVIR_DOMAIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_DOMAIN, GVirDomainClass))

#define GVIR_TYPE_DOMAIN_INFO       (gvir_domain_info_get_type())
#define GVIR_TYPE_DOMAIN_HANDLE     (gvir_domain_handle_get_type())

typedef struct _GVirDomain GVirDomain;
typedef struct _GVirDomainPrivate GVirDomainPrivate;
typedef struct _GVirDomainClass GVirDomainClass;

struct _GVirDomain
{
    GObject parent;

    GVirDomainPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirDomainClass
{
    GObjectClass parent_class;

    /* signals */
    void (*started)(GVirDomain *dom);
    void (*stopped)(GVirDomain *dom);
    void (*resumed)(GVirDomain *dom);
    void (*updated)(GVirDomain *dom);
    void (*suspended)(GVirDomain *dom);

    gpointer padding[20];
};


typedef enum {
    GVIR_DOMAIN_STATE_NONE    = 0, /* no state */
    GVIR_DOMAIN_STATE_RUNNING = 1, /* the domain is running */
    GVIR_DOMAIN_STATE_BLOCKED = 2, /* the domain is blocked on resource */
    GVIR_DOMAIN_STATE_PAUSED  = 3, /* the domain is paused by user */
    GVIR_DOMAIN_STATE_SHUTDOWN= 4, /* the domain is being shut down */
    GVIR_DOMAIN_STATE_SHUTOFF = 5, /* the domain is shut off */
    GVIR_DOMAIN_STATE_CRASHED = 6  /* the domain is crashed */
} GVirDomainState;


typedef enum {
    GVIR_DOMAIN_START_NONE         = 0,
    GVIR_DOMAIN_START_PAUSED       = (1 << 0),
    GVIR_DOMAIN_START_AUTODESTROY  = (1 << 1),
    GVIR_DOMAIN_START_BYPASS_CACHE = (1 << 2),
    GVIR_DOMAIN_START_FORCE_BOOT   = (1 << 3),
} GVirDomainStartFlags;

/**
 * GVirDomainDeleteFlags:
 * @GVIR_DOMAIN_DELETE_NONE: No flags
 * @GVIR_DOMAIN_DELETE_SAVED_STATE: Also remove associated saved state (if present).
 * @GVIR_DOMAIN_DELETE_SNAPSHOTS_METADATA: If last use of domain, then also
 *                                         remove any snapshot metadata.
 */
typedef enum {
    GVIR_DOMAIN_DELETE_NONE               = 0,
    GVIR_DOMAIN_DELETE_SAVED_STATE        = VIR_DOMAIN_UNDEFINE_MANAGED_SAVE,
    GVIR_DOMAIN_DELETE_SNAPSHOTS_METADATA = VIR_DOMAIN_UNDEFINE_SNAPSHOTS_METADATA,
} GVirDomainDeleteFlags;

typedef struct _GVirDomainInfo GVirDomainInfo;
struct _GVirDomainInfo
{
    GVirDomainState state; /* the running state */
    guint64 maxMem;        /* the maximum memory in KBytes allowed */
    guint64 memory;        /* the memory in KBytes used by the domain */
    guint16 nrVirtCpu;     /* the number of virtual CPUs for the domain */
    guint64 cpuTime;       /* the CPU time used in nanoseconds */
};

GType gvir_domain_get_type(void);
GType gvir_domain_info_get_type(void);
GType gvir_domain_handle_get_type(void);

const gchar *gvir_domain_get_name(GVirDomain *dom);
const gchar *gvir_domain_get_uuid(GVirDomain *dom);
gint gvir_domain_get_id(GVirDomain *dom,
                        GError **err);

gboolean gvir_domain_start(GVirDomain *dom,
                           guint flags,
                           GError **err);
gboolean gvir_domain_resume(GVirDomain *dom,
                            GError **err);
gboolean gvir_domain_stop(GVirDomain *dom,
                          guint flags,
                          GError **err);
gboolean gvir_domain_delete(GVirDomain *dom,
                            guint flags,
                            GError **err);
gboolean gvir_domain_shutdown(GVirDomain *dom,
                              guint flags,
                              GError **err);
gboolean gvir_domain_reboot(GVirDomain *dom,
                            guint flags,
                            GError **err);

GVirDomainInfo *gvir_domain_get_info(GVirDomain *dom,
                                     GError **err);

GVirConfigDomain *gvir_domain_get_config(GVirDomain *dom,
                                         guint flags,
                                         GError **err);
gboolean gvir_domain_set_config(GVirDomain *domain,
                                GVirConfigDomain *conf,
                                GError **err);

gchar *gvir_domain_screenshot(GVirDomain *dom,
                              GVirStream *stream,
                              guint monitor_id,
                              guint flags,
                              GError **err);

gboolean gvir_domain_open_console(GVirDomain *dom,
                                  GVirStream *stream,
                                  const gchar *devname,
                                  guint flags,
                                  GError **err);

gboolean gvir_domain_open_graphics(GVirDomain *dom,
                                   guint idx,
                                   int fd,
                                   unsigned int flags,
                                   GError **err);

gboolean gvir_domain_suspend (GVirDomain *dom,
                              GError **err);
gboolean gvir_domain_save (GVirDomain *dom,
                           unsigned int flags,
                           GError **err);
void gvir_domain_save_async (GVirDomain *dom,
                             unsigned int flags,
                             GCancellable *cancellable,
                             GAsyncReadyCallback callback,
                             gpointer user_data);
gboolean gvir_domain_save_finish (GVirDomain *dom,
                                  GAsyncResult *result,
                                  GError **err);
gboolean gvir_domain_get_persistent(GVirDomain *dom);
gboolean gvir_domain_get_saved(GVirDomain *dom);

GList *gvir_domain_get_devices(GVirDomain *domain,
                               GError **err);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_DOMAIN_H__ */
