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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#include <libvirt-gobject/libvirt-gobject-stream.h>
#include <libvirt/libvirt.h>
#include <libvirt-gobject/libvirt-gobject-domain-snapshot.h>

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
    void (*pmsuspended)(GVirDomain *dom);

    gpointer padding[19];
};


typedef enum {
    GVIR_DOMAIN_STATE_NONE    = 0,     /* no state */
    GVIR_DOMAIN_STATE_RUNNING = 1,     /* the domain is running */
    GVIR_DOMAIN_STATE_BLOCKED = 2,     /* the domain is blocked on resource */
    GVIR_DOMAIN_STATE_PAUSED  = 3,     /* the domain is paused by user */
    GVIR_DOMAIN_STATE_SHUTDOWN= 4,     /* the domain is being shut down */
    GVIR_DOMAIN_STATE_SHUTOFF = 5,     /* the domain is shut off */
    GVIR_DOMAIN_STATE_CRASHED = 6,     /* the domain is crashed */
    GVIR_DOMAIN_STATE_PMSUSPENDED = 7  /* the domain is suspended by guest
                                          power management */
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
    GVIR_DOMAIN_DELETE_REMOVE_NVRAM       = VIR_DOMAIN_UNDEFINE_NVRAM,
    GVIR_DOMAIN_DELETE_KEEP_NVRAM         = VIR_DOMAIN_UNDEFINE_KEEP_NVRAM,
} GVirDomainDeleteFlags;

/**
 * GVirDomainXMLFlags:
 * @GVIR_DOMAIN_XML_NONE: No flags
 * @GVIR_DOMAIN_XML_SECURE: Dump security sensitive information too
 * @GVIR_DOMAIN_XML_INACTIVE: Dump inactive domain information
 * @GVIR_DOMAIN_XML_UPDATE_CPU: Update guest CPU requirements according to host CPU
 */
typedef enum {
    GVIR_DOMAIN_XML_NONE            = 0,
    GVIR_DOMAIN_XML_SECURE          = VIR_DOMAIN_XML_SECURE,
    GVIR_DOMAIN_XML_INACTIVE        = VIR_DOMAIN_XML_INACTIVE,
    GVIR_DOMAIN_XML_UPDATE_CPU      = VIR_DOMAIN_XML_UPDATE_CPU,
} GVirDomainXMLFlags;

/**
 * GVirDomainShutdownFlags:
 * @GVIR_DOMAIN_SHUTDOWN_NONE: No flags, hypervisor choice
 * @GVIR_DOMAIN_SHUTDOWN_ACPI_POWER_BTN: Send ACPI event
 * @GVIR_DOMAIN_SHUTDOWN_GUEST_AGENT: Use guest agent
 *
 */
typedef enum {
    GVIR_DOMAIN_SHUTDOWN_NONE           = 0,
    GVIR_DOMAIN_SHUTDOWN_ACPI_POWER_BTN = VIR_DOMAIN_SHUTDOWN_ACPI_POWER_BTN,
    GVIR_DOMAIN_SHUTDOWN_GUEST_AGENT    = VIR_DOMAIN_SHUTDOWN_GUEST_AGENT,
} GVirDomainShutdownFlags;

/**
 * GVirDomainSnapshotCreateFlags:
 * @GVIR_DOMAIN_SNAPSHOT_NONE: No flags
 * @GVIR_DOMAIN_SNAPSHOT_REDEFINE: Restore or alter metadata
 * @GVIR_DOMAIN_SNAPSHOT_CURRENT: With redefine, make snapshot current
 * @GVIR_DOMAIN_SNAPSHOT_NO_METADATA: Make snapshot without remembering it
 * @GVIR_DOMAIN_SNAPSHOT_HALT: Stop running guest after snapshot
 * @GVIR_DOMAIN_SNAPSHOT_DISK_ONLY: Disk snapshot, not system checkpoint
 * @GVIR_DOMAIN_SNAPSHOT_REUSE_EXT: Reuse any existing external files
 * @GVIR_DOMAIN_SNAPSHOT_QUIESCE: Use guest agent to quiesce all mounter
 *                                file systems within the domain
 * @GVIR_DOMAIN_SNAPSHOT_ATOMIC: Atomically avoid partial changes
 */
typedef enum {
    GVIR_DOMAIN_SNAPSHOT_NONE         = 0,
    GVIR_DOMAIN_SNAPSHOT_REDEFINE     = VIR_DOMAIN_SNAPSHOT_CREATE_REDEFINE,
    GVIR_DOMAIN_SNAPSHOT_CURRENT      = VIR_DOMAIN_SNAPSHOT_CREATE_CURRENT,
    GVIR_DOMAIN_SNAPSHOT_NO_METADATA  = VIR_DOMAIN_SNAPSHOT_CREATE_NO_METADATA,
    GVIR_DOMAIN_SNAPSHOT_HALT         = VIR_DOMAIN_SNAPSHOT_CREATE_HALT,
    GVIR_DOMAIN_SNAPSHOT_DISK_ONLY    = VIR_DOMAIN_SNAPSHOT_CREATE_DISK_ONLY,
    GVIR_DOMAIN_SNAPSHOT_REUSE_EXT    = VIR_DOMAIN_SNAPSHOT_CREATE_REUSE_EXT,
    GVIR_DOMAIN_SNAPSHOT_QUIESCE      = VIR_DOMAIN_SNAPSHOT_CREATE_QUIESCE,
    GVIR_DOMAIN_SNAPSHOT_ATOMIC       = VIR_DOMAIN_SNAPSHOT_CREATE_ATOMIC,
} GVirDomainSnapshotCreateFlags;

/**
 * GVirDomainUpdateDeviceFlags:
 * @GVIR_DOMAIN_UPDATE_DEVICE_CURRENT: Update current domain state
 * @GVIR_DOMAIN_UPDATE_DEVICE_LIVE: Update state for only active domains
 * @GVIR_DOMAIN_UPDATE_DEVICE_CONFIG: Update state for persistent state only
 */
typedef enum {
    GVIR_DOMAIN_UPDATE_DEVICE_CURRENT   = VIR_DOMAIN_AFFECT_CURRENT,
    GVIR_DOMAIN_UPDATE_DEVICE_LIVE      = VIR_DOMAIN_AFFECT_LIVE,
    GVIR_DOMAIN_UPDATE_DEVICE_CONFIG    = VIR_DOMAIN_AFFECT_CONFIG,
} GVirDomainUpdateDeviceFlags;

/**
 * GVirDomainRebootFlags:
 * @GVIR_DOMAIN_REBOOT_NONE: No flags, hypervisor choice
 * @GVIR_DOMAIN_REBOOT_ACPI_POWER_BTN: Send ACPI event
 * @GVIR_DOMAIN_REBOOT_GUEST_AGENT: Use guest agent
 *
 */
typedef enum {
    GVIR_DOMAIN_REBOOT_NONE           = 0,
    GVIR_DOMAIN_REBOOT_ACPI_POWER_BTN = VIR_DOMAIN_REBOOT_ACPI_POWER_BTN,
    GVIR_DOMAIN_REBOOT_GUEST_AGENT    = VIR_DOMAIN_REBOOT_GUEST_AGENT,
} GVirDomainRebootFlags;

/**
 * GVirDomainSnapshotListFlags:
 * @GVIR_DOMAIN_SNAPSHOT_LIST_ALL: List all snapshots
 * @GVIR_DOMAIN_SNAPSHOT_LIST_DESCENDANTS: List all descendants, not just
 *                                         children, when listing a snapshot.
 *                                         For historical reasons, groups do not use contiguous bits.
 * @GVIR_DOMAIN_SNAPSHOT_LIST_ROOTS: Filter by snapshots with no parents, when listing a domain
 * @GVIR_DOMAIN_SNAPSHOT_LIST_METADATA: Filter by snapshots which have metadata
 * @GVIR_DOMAIN_SNAPSHOT_LIST_LEAVES: Filter by snapshots with no children
 * @GVIR_DOMAIN_SNAPSHOT_LIST_NO_LEAVES: Filter by snapshots that have children
 * @GVIR_DOMAIN_SNAPSHOT_LIST_NO_METADATA: Filter by snapshots with no metadata
 * @GVIR_DOMAIN_SNAPSHOT_LIST_INACTIVE: Filter by snapshots taken while guest was shut off
 * @GVIR_DOMAIN_SNAPSHOT_LIST_ACTIVE: Filter by snapshots taken while guest was active, and with memory state
 * @GVIR_DOMAIN_SNAPSHOT_LIST_DISK_ONLY: Filter by snapshots taken while guest was active, but without memory state
 * @GVIR_DOMAIN_SNAPSHOT_LIST_INTERNAL: Filter by snapshots stored internal to disk images
 * @GVIR_DOMAIN_SNAPSHOT_LIST_EXTERNAL: Filter by snapshots that use files external to disk images
 */
typedef enum {
    GVIR_DOMAIN_SNAPSHOT_LIST_ALL         = 0,
    GVIR_DOMAIN_SNAPSHOT_LIST_DESCENDANTS = VIR_DOMAIN_SNAPSHOT_LIST_DESCENDANTS,
    GVIR_DOMAIN_SNAPSHOT_LIST_ROOTS       = VIR_DOMAIN_SNAPSHOT_LIST_ROOTS,
    GVIR_DOMAIN_SNAPSHOT_LIST_METADATA    = VIR_DOMAIN_SNAPSHOT_LIST_METADATA,
    GVIR_DOMAIN_SNAPSHOT_LIST_LEAVES      = VIR_DOMAIN_SNAPSHOT_LIST_LEAVES,
    GVIR_DOMAIN_SNAPSHOT_LIST_NO_LEAVES   = VIR_DOMAIN_SNAPSHOT_LIST_NO_LEAVES,
    GVIR_DOMAIN_SNAPSHOT_LIST_NO_METADATA = VIR_DOMAIN_SNAPSHOT_LIST_NO_METADATA,
    GVIR_DOMAIN_SNAPSHOT_LIST_INACTIVE    = VIR_DOMAIN_SNAPSHOT_LIST_INACTIVE,
    GVIR_DOMAIN_SNAPSHOT_LIST_ACTIVE      = VIR_DOMAIN_SNAPSHOT_LIST_ACTIVE,
    GVIR_DOMAIN_SNAPSHOT_LIST_DISK_ONLY   = VIR_DOMAIN_SNAPSHOT_LIST_DISK_ONLY,
    GVIR_DOMAIN_SNAPSHOT_LIST_INTERNAL    = VIR_DOMAIN_SNAPSHOT_LIST_INTERNAL,
    GVIR_DOMAIN_SNAPSHOT_LIST_EXTERNAL    = VIR_DOMAIN_SNAPSHOT_LIST_EXTERNAL
} GVirDomainSnapshotListFlags;

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
void gvir_domain_start_async(GVirDomain *dom,
                             guint flags,
                             GCancellable *cancellable,
                             GAsyncReadyCallback callback,
                             gpointer user_data);
gboolean gvir_domain_start_finish(GVirDomain *dom,
                                  GAsyncResult *result,
                                  GError **err);

gboolean gvir_domain_resume(GVirDomain *dom,
                            GError **err);
void gvir_domain_resume_async(GVirDomain *dom,
                              GCancellable *cancellable,
                              GAsyncReadyCallback callback,
                              gpointer user_data);
gboolean gvir_domain_resume_finish(GVirDomain *dom,
                                   GAsyncResult *result,
                                   GError **err);
gboolean gvir_domain_wakeup(GVirDomain *dom,
                            guint flags,
                            GError **err);
void gvir_domain_wakeup_async(GVirDomain *dom,
                              guint flags,
                              GCancellable *cancellable,
                              GAsyncReadyCallback callback,
                              gpointer user_data);
gboolean gvir_domain_wakeup_finish(GVirDomain *dom,
                                   GAsyncResult *result,
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

gboolean gvir_domain_save_to_file(GVirDomain *dom,
                                  gchar *filename,
                                  GVirConfigDomain *custom_conf,
                                  guint flags,
                                  GError **err);

void gvir_domain_save_to_file_async(GVirDomain *dom,
                                    gchar *filename,
                                    GVirConfigDomain *custom_conf,
                                    guint flags,
                                    GCancellable *cancellable,
                                    GAsyncReadyCallback callback,
                                    gpointer user_data);

gboolean gvir_domain_save_to_file_finish(GVirDomain *dom,
                                         GAsyncResult *result,
                                         GError **err);

GVirDomainInfo *gvir_domain_get_info(GVirDomain *dom,
                                     GError **err);
void gvir_domain_get_info_async(GVirDomain *dom,
                                GCancellable *cancellable,
                                GAsyncReadyCallback callback,
                                gpointer user_data);
GVirDomainInfo *gvir_domain_get_info_finish(GVirDomain *dom,
                                            GAsyncResult *result,
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
int gvir_domain_open_graphics_fd(GVirDomain *dom,
                                 guint idx,
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

gboolean gvir_domain_update_device(GVirDomain *dom,
                                   GVirConfigDomainDevice *device,
                                   guint flags,
                                   GError **err);

GVirDomainSnapshot *
gvir_domain_create_snapshot(GVirDomain *dom,
                            GVirConfigDomainSnapshot *custom_conf,
                            guint flags,
                            GError **err);

void gvir_domain_create_snapshot_async(GVirDomain *dom,
                                       GVirConfigDomainSnapshot *custom_conf,
                                       guint flags,
                                       GCancellable *cancellable,
                                       GAsyncReadyCallback callback,
                                       gpointer user_data);

GVirDomainSnapshot *gvir_domain_create_snapshot_finish(GVirDomain  *domain,
                                                       GAsyncResult *result,
                                                       GError **error);


gboolean gvir_domain_fetch_snapshots(GVirDomain *dom,
                                     guint list_flags,
                                     GCancellable *cancellable,
                                     GError **error);

GList *gvir_domain_get_snapshots(GVirDomain *dom);

void gvir_domain_fetch_snapshots_async(GVirDomain *dom,
                                       guint list_flags,
                                       GCancellable *cancellable,
                                       GAsyncReadyCallback callback,
                                       gpointer user_data);

gboolean gvir_domain_fetch_snapshots_finish(GVirDomain *dom,
                                            GAsyncResult *res,
                                            GError **error);

gboolean gvir_domain_get_has_current_snapshot(GVirDomain *dom,
                                              guint flags,
                                              gboolean *has_current_snapshot,
                                              GError **error);

gboolean gvir_domain_set_time(GVirDomain *dom,
                              GDateTime *date_time,
                              guint flags,
                              GError **err);
void gvir_domain_set_time_async(GVirDomain *dom,
                                GDateTime *date_time,
                                guint flags,
                                GCancellable *cancellable,
                                GAsyncReadyCallback callback,
                                gpointer user_data);
gboolean gvir_domain_set_time_finish(GVirDomain *dom,
                                     GAsyncResult *result,
                                     GError **err);

G_END_DECLS
