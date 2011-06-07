/*
 * libvirt-gobject-config_domain_snapshot.c: libvirt glib integration
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

#include <string.h>

#include "libvirt-gconfig/libvirt-gconfig.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_CONFIG_DOMAIN_SNAPSHOT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_SNAPSHOT, GVirConfigDomainSnapshotPrivate))

struct _GVirConfigDomainSnapshotPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainSnapshot, gvir_config_domain_snapshot, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_domain_snapshot_class_init(GVirConfigDomainSnapshotClass *klass)
{

    g_type_class_add_private(klass, sizeof(GVirConfigDomainSnapshotPrivate));
}


static void gvir_config_domain_snapshot_init(GVirConfigDomainSnapshot *conn)
{
    GVirConfigDomainSnapshotPrivate *priv;

    DEBUG("Init GVirConfigDomainSnapshot=%p", conn);

    priv = conn->priv = GVIR_CONFIG_DOMAIN_SNAPSHOT_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}


GVirConfigDomainSnapshot *gvir_config_domain_snapshot_new(const gchar *xml)
{
    return GVIR_CONFIG_DOMAIN_SNAPSHOT(g_object_new(GVIR_TYPE_CONFIG_DOMAIN_SNAPSHOT,
                                                    "doc", xml,
                                                    "schema", DATADIR "/libvirt/schemas/domainsnapshot.rng",
                                                    NULL));
}
