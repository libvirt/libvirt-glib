/*
 * libvirt-gobject-domain_snapshot.c: libvirt gobject integration
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

#ifndef __LIBVIRT_GOBJECT_DOMAIN_SNAPSHOT_H__
#define __LIBVIRT_GOBJECT_DOMAIN_SNAPSHOT_H__

G_BEGIN_DECLS

#define GVIR_TYPE_DOMAIN_SNAPSHOT            (gvir_domain_snapshot_get_type ())
#define GVIR_DOMAIN_SNAPSHOT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_DOMAIN_SNAPSHOT, GVirDomainSnapshot))
#define GVIR_DOMAIN_SNAPSHOT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_DOMAIN_SNAPSHOT, GVirDomainSnapshotClass))
#define GVIR_IS_DOMAIN_SNAPSHOT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_DOMAIN_SNAPSHOT))
#define GVIR_IS_DOMAIN_SNAPSHOT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_DOMAIN_SNAPSHOT))
#define GVIR_DOMAIN_SNAPSHOT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_DOMAIN_SNAPSHOT, GVirDomainSnapshotClass))

#define GVIR_TYPE_DOMAIN_SNAPSHOT_HANDLE     (gvir_domain_snapshot_handle_get_type())

typedef struct _GVirDomainSnapshot GVirDomainSnapshot;
typedef struct _GVirDomainSnapshotPrivate GVirDomainSnapshotPrivate;
typedef struct _GVirDomainSnapshotClass GVirDomainSnapshotClass;

struct _GVirDomainSnapshot
{
    GObject parent;

    GVirDomainSnapshotPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirDomainSnapshotClass
{
    GObjectClass parent_class;

    gpointer padding[20];
};

/**
 * GVirDomainSnapshotDeleteFlags:
 * @GVIR_DOMAIN_SNAPSHOT_DELETE_CHILDREN: Also delete children
 * @GVIR_DOMAIN_SNAPSHOT_DELETE_METADATA_ONLY: Delete just metadata
 * @GVIR_DOMAIN_SNAPSHOT_DELETE_CHILDREN_ONLY: Delete just children
 */
typedef enum {
  GVIR_DOMAIN_SNAPSHOT_DELETE_CHILDREN = 1,
  GVIR_DOMAIN_SNAPSHOT_DELETE_METADATA_ONLY = 2,
  GVIR_DOMAIN_SNAPSHOT_DELETE_CHILDREN_ONLY = 4
} GVirDomainSnapshotDeleteFlags;


/**
 * GVirDomainSnapshotRevertFlags:
 * @GVIR_DOMAIN_SNAPSHOT_REVERT_RUNNING: Run after revert
 * @GVIR_DOMAIN_SNAPSHOT_REVERT_PAUSED: Pause after revert
 * @GVIR_DOMAIN_SNAPSHOT_REVERT_FORCE: Allow risky reverts
 */
typedef enum {
  GVIR_DOMAIN_SNAPSHOT_REVERT_RUNNING = 1,
  GVIR_DOMAIN_SNAPSHOT_REVERT_PAUSED = 2,
  GVIR_DOMAIN_SNAPSHOT_REVERT_FORCE = 4
} GVirDomainSnapshotRevertFlags;


GType gvir_domain_snapshot_get_type(void);
GType gvir_domain_snapshot_handle_get_type(void);

const gchar *gvir_domain_snapshot_get_name(GVirDomainSnapshot *snapshot);

GVirConfigDomainSnapshot *gvir_domain_snapshot_get_config
                                (GVirDomainSnapshot *snapshot,
                                 guint flags,
                                 GError **err);

gboolean gvir_domain_snapshot_delete (GVirDomainSnapshot *snapshot,
                                      guint flags,
                                      GError **error);

void gvir_domain_snapshot_delete_async(GVirDomainSnapshot *snapshot,
                                       guint flags,
                                       GCancellable *cancellable,
                                       GAsyncReadyCallback callback,
                                       gpointer user_data);

gboolean gvir_domain_snapshot_delete_finish(GVirDomainSnapshot *snapshot,
                                            GAsyncResult *res,
                                            GError **error);

gboolean gvir_domain_snapshot_get_is_current(GVirDomainSnapshot *snapshot,
                                             guint flags,
                                             gboolean *is_current,
                                             GError **error);

gboolean gvir_domain_snapshot_revert_to(GVirDomainSnapshot *snapshot,
                                        guint flags,
                                        GError **error);

void gvir_domain_snapshot_revert_to_async(GVirDomainSnapshot *snapshot,
                                          guint flags,
                                          GCancellable *cancellable,
                                          GAsyncReadyCallback callback,
                                          gpointer user_data);

gboolean gvir_domain_snapshot_revert_to_finish(GVirDomainSnapshot *snapshot,
                                               GAsyncResult *result,
                                               GError **error);


gboolean gvir_domain_snapshot_set_config(GVirDomainSnapshot *snapshot,
                                         GVirConfigDomainSnapshot *conf,
                                         GError **error);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_DOMAIN_SNAPSHOT_H__ */
