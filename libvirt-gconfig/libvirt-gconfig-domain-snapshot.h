/*
 * libvirt-gconfig-domain-snapshot.h: libvirt domain snapshot configuration
 *
 * Copyright (C) 2010-2013 Red Hat, Inc.
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
 *         Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_SNAPSHOT_H__
#define __LIBVIRT_GCONFIG_DOMAIN_SNAPSHOT_H__

#include <libvirt-gconfig/libvirt-gconfig-domain.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-snapshot-disk.h>

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT            (gvir_config_domain_snapshot_get_type ())
#define GVIR_CONFIG_DOMAIN_SNAPSHOT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT, GVirConfigDomainSnapshot))
#define GVIR_CONFIG_DOMAIN_SNAPSHOT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT, GVirConfigDomainSnapshotClass))
#define GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT))
#define GVIR_CONFIG_IS_DOMAIN_SNAPSHOT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT))
#define GVIR_CONFIG_DOMAIN_SNAPSHOT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT, GVirConfigDomainSnapshotClass))

typedef struct _GVirConfigDomainSnapshot GVirConfigDomainSnapshot;
typedef struct _GVirConfigDomainSnapshotPrivate GVirConfigDomainSnapshotPrivate;
typedef struct _GVirConfigDomainSnapshotClass GVirConfigDomainSnapshotClass;

struct _GVirConfigDomainSnapshot
{
    GVirConfigObject parent;

    GVirConfigDomainSnapshotPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainSnapshotClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_SNAPSHOT_DOMAIN_STATE_NOSTATE = 0,     /* no state */
    GVIR_CONFIG_DOMAIN_SNAPSHOT_DOMAIN_STATE_RUNNING = 1,     /* the domain is running */
    GVIR_CONFIG_DOMAIN_SNAPSHOT_DOMAIN_STATE_BLOCKED = 2,     /* the domain is blocked on resource */
    GVIR_CONFIG_DOMAIN_SNAPSHOT_DOMAIN_STATE_PAUSED  = 3,     /* the domain is paused by user */
    GVIR_CONFIG_DOMAIN_SNAPSHOT_DOMAIN_STATE_SHUTDOWN= 4,     /* the domain is being shut down */
    GVIR_CONFIG_DOMAIN_SNAPSHOT_DOMAIN_STATE_SHUTOFF = 5,     /* the domain is shut off */
    GVIR_CONFIG_DOMAIN_SNAPSHOT_DOMAIN_STATE_CRASHED = 6,     /* the domain is crashed */
    GVIR_CONFIG_DOMAIN_SNAPSHOT_DOMAIN_STATE_PMSUSPENDED = 7, /* the domain is suspended by guest
                                                                 power management */
    GVIR_CONFIG_DOMAIN_SNAPSHOT_DOMAIN_STATE_DISK_SNAPSHOT = 100
} GVirConfigDomainSnapshotDomainState;


typedef enum {
    GVIR_CONFIG_DOMAIN_SNAPSHOT_MEMORY_STATE_NONE,
    GVIR_CONFIG_DOMAIN_SNAPSHOT_MEMORY_STATE_INTERNAL,
    GVIR_CONFIG_DOMAIN_SNAPSHOT_MEMORY_STATE_EXTERNAL,
} GVirConfigDomainSnapshotMemoryState;


GType gvir_config_domain_snapshot_get_type(void);

GVirConfigDomainSnapshot *gvir_config_domain_snapshot_new(void);
GVirConfigDomainSnapshot *gvir_config_domain_snapshot_new_from_xml(const gchar *xml,
                                                                   GError **error);

const char *gvir_config_domain_snapshot_get_name(GVirConfigDomainSnapshot *snapshot);
void gvir_config_domain_snapshot_set_name(GVirConfigDomainSnapshot *snapshot,
                                          const char *name);
const char *gvir_config_domain_snapshot_get_description(GVirConfigDomainSnapshot *snapshot);
void gvir_config_domain_snapshot_set_description(GVirConfigDomainSnapshot *snapshot,
                                                 const char *description);
GVirConfigDomainSnapshotMemoryState gvir_config_domain_snapshot_get_memory_state(GVirConfigDomainSnapshot *snapshot);
void gvir_config_domain_snapshot_set_memory_state(GVirConfigDomainSnapshot *snapshot,
                                                  GVirConfigDomainSnapshotMemoryState state);
const char *gvir_config_domain_snapshot_get_memory_file(GVirConfigDomainSnapshot *snapshot);
void gvir_config_domain_snapshot_set_memory_file(GVirConfigDomainSnapshot *snapshot,
                                                 const char *filename);
time_t gvir_config_domain_snapshot_get_creation_time(GVirConfigDomainSnapshot *snapshot);
GVirConfigDomainSnapshotDomainState gvir_config_domain_snapshot_get_state(GVirConfigDomainSnapshot *snapshot);
const char *gvir_config_domain_snapshot_get_parent(GVirConfigDomainSnapshot *snapshot);
GVirConfigDomain *gvir_config_domain_snapshot_get_domain(GVirConfigDomainSnapshot *snapshot);
void gvir_config_domain_snapshot_set_disks(GVirConfigDomainSnapshot *snapshot,
                                           GList *disks);
void gvir_config_domain_snapshot_add_disk(GVirConfigDomainSnapshot *snapshot,
                                          GVirConfigDomainSnapshotDisk *disk);
GList *gvir_config_domain_snapshot_get_disks(GVirConfigDomainSnapshot *snapshot);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_SNAPSHOT_H__ */
