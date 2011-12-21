/*
 * libvirt-gconfig-domain-snapshot.h: libvirt domain snapshot configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_SNAPSHOT_H__
#define __LIBVIRT_GCONFIG_DOMAIN_SNAPSHOT_H__

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


GType gvir_config_domain_snapshot_get_type(void);

GVirConfigDomainSnapshot *gvir_config_domain_snapshot_new(void);
GVirConfigDomainSnapshot *gvir_config_domain_snapshot_new_from_xml(const gchar *xml,
                                                                   GError **error);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_SNAPSHOT_H__ */
