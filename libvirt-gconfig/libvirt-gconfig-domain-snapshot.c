/*
 * libvirt-gconfig-domain-snapshot.c: libvirt domain snapshot configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_SNAPSHOT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT, GVirConfigDomainSnapshotPrivate))

struct _GVirConfigDomainSnapshotPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainSnapshot, gvir_config_domain_snapshot, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_snapshot_class_init(GVirConfigDomainSnapshotClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_snapshot_init(GVirConfigDomainSnapshot *snapshot)
{
    snapshot->priv = GVIR_CONFIG_DOMAIN_SNAPSHOT_GET_PRIVATE(snapshot);
}


GVirConfigDomainSnapshot *gvir_config_domain_snapshot_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT,
                                    "domainsnapshot",
                                    DATADIR "/libvirt/schemas/domainsnapshot.rng");
    return GVIR_CONFIG_DOMAIN_SNAPSHOT(object);
}

GVirConfigDomainSnapshot *gvir_config_domain_snapshot_new_from_xml(const gchar *xml,
                                                                   GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT,
                                    "domainsnapshot",
                                    DATADIR "/libvirt/schemas/domainsnapshot.rng",
                                    xml, error);
    return GVIR_CONFIG_DOMAIN_SNAPSHOT(object);
}


const char *gvir_config_domain_snapshot_get_name(GVirConfigDomainSnapshot *snapshot)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot), NULL);

    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(snapshot),
                                               "name");
}


void gvir_config_domain_snapshot_set_name(GVirConfigDomainSnapshot *snapshot,
                                          const char *name)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(snapshot),
                                        "name", name);
}


const char *gvir_config_domain_snapshot_get_description(GVirConfigDomainSnapshot *snapshot)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot), NULL);

    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(snapshot),
                                               "description");
}


void gvir_config_domain_snapshot_set_description(GVirConfigDomainSnapshot *snapshot,
                                                 const char *description)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(snapshot),
                                        "description", description);
}


GVirConfigDomainSnapshotMemoryState gvir_config_domain_snapshot_get_memory_state(GVirConfigDomainSnapshot *snapshot)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot),
                         GVIR_CONFIG_DOMAIN_SNAPSHOT_MEMORY_STATE_NONE);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(snapshot),
                                                  "memory", "snapshot",
                                                  GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT_MEMORY_STATE,
                                                  GVIR_CONFIG_DOMAIN_SNAPSHOT_MEMORY_STATE_NONE);
}


void gvir_config_domain_snapshot_set_memory_state(GVirConfigDomainSnapshot *snapshot,
                                                  GVirConfigDomainSnapshotMemoryState state)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot));

    gvir_config_object_add_child_with_attribute_enum(GVIR_CONFIG_OBJECT(snapshot),
                                                     "memory", "snapshot",
                                                     GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT_MEMORY_STATE,
                                                     state);
}


const char *gvir_config_domain_snapshot_get_memory_file(GVirConfigDomainSnapshot *snapshot)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(snapshot),
                                            "memory", "file");
}


void gvir_config_domain_snapshot_set_memory_file(GVirConfigDomainSnapshot *snapshot,
                                                 const char *filename)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot));

    gvir_config_object_add_child_with_attribute(GVIR_CONFIG_OBJECT(snapshot),
                                                "memory", "file", filename);
}


time_t gvir_config_domain_snapshot_get_creation_time(GVirConfigDomainSnapshot *snapshot)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot), 0);

    return gvir_config_object_get_node_content_uint64(GVIR_CONFIG_OBJECT(snapshot),
                                                      "creationTime");
}


GVirConfigDomainSnapshotDomainState gvir_config_domain_snapshot_get_state(GVirConfigDomainSnapshot *snapshot)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot),
                         GVIR_CONFIG_DOMAIN_SNAPSHOT_DOMAIN_STATE_NOSTATE);

    return gvir_config_object_get_node_content_genum(GVIR_CONFIG_OBJECT(snapshot),
                                                     "state",
                                                     GVIR_CONFIG_TYPE_DOMAIN_SNAPSHOT_DOMAIN_STATE,
                                                     GVIR_CONFIG_DOMAIN_SNAPSHOT_DOMAIN_STATE_NOSTATE);
}


const char *gvir_config_domain_snapshot_get_parent(GVirConfigDomainSnapshot *snapshot)
{
    GVirConfigObject *parent;
    const char *parent_name;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot), NULL);

    parent = gvir_config_object_get_child(GVIR_CONFIG_OBJECT(snapshot),
                                          "parent");
    if (parent == NULL)
        return NULL;

    parent_name = gvir_config_object_get_node_content(parent, "name");
    g_object_unref(G_OBJECT(parent));

    return parent_name;
}


/**
 * gvir_config_domain_snapshot_get_domain:
 * @snapshot: a #GVirConfigDomainSnapshot
 *
 * Gets the configuration of the domain @snapshot is a snapshot of.
 *
 * Returns: (transfer full): A #GVirConfigDomain. The returned object
 * should be unreffed with g_object_unref() when no longer needed.
 */
GVirConfigDomain *gvir_config_domain_snapshot_get_domain(GVirConfigDomainSnapshot *snapshot)
{
    GVirConfigObject *domain;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot), NULL);

    domain = gvir_config_object_get_child_with_type(GVIR_CONFIG_OBJECT(snapshot),
                                                    "domain",
                                                    GVIR_CONFIG_TYPE_DOMAIN);

    return GVIR_CONFIG_DOMAIN(domain);
}


/**
 * gvir_config_domain_snapshot_set_disks:
 * @snapshot: a #GVirConfigDomainSnapshot
 * @disks: (in) (element-type LibvirtGConfig.DomainSnapshotDisk):
 */
void gvir_config_domain_snapshot_set_disks(GVirConfigDomainSnapshot *snapshot,
                                           GList *disks)
{
    GVirConfigObject *disks_node;
    GList *it;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot));

    if (disks == NULL) {
        gvir_config_object_delete_children(GVIR_CONFIG_OBJECT(snapshot),
                                           "disks",
                                           NULL);
        return;
    }

    disks_node = gvir_config_object_new(GVIR_CONFIG_TYPE_OBJECT,
                                        "disks", NULL);

    for (it = disks; it != NULL; it = it->next) {
        if (!GVIR_CONFIG_IS_DOMAIN_SNAPSHOT_DISK(it->data)) {
            g_warn_if_reached();
            continue;
        }
        gvir_config_object_attach_add(disks_node,
                                      GVIR_CONFIG_OBJECT(it->data));
    }

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(snapshot),
                                      "disks",
                                      disks_node);
    g_object_unref(G_OBJECT(disks_node));
}


void gvir_config_domain_snapshot_add_disk(GVirConfigDomainSnapshot *snapshot,
                                          GVirConfigDomainSnapshotDisk *disk)
{
    GVirConfigObject *disks_node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot));
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT_DISK(disk));

    disks_node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(snapshot),
                                              "disks");

    gvir_config_object_attach_add(disks_node, GVIR_CONFIG_OBJECT(disk));
    g_object_unref(G_OBJECT(disks_node));
}


struct GetDiskData {
    GVirConfigXmlDoc *doc;
    GList *disks;
};

static gboolean get_disk(xmlNodePtr node, gpointer opaque)
{
    struct GetDiskData* data = (struct GetDiskData*)opaque;
    GVirConfigDomainSnapshotDisk *disk;

    disk = gvir_config_domain_snapshot_disk_new_from_tree(data->doc, node);
    if (disk != NULL)
        data->disks = g_list_prepend(data->disks, disk);
    else
        g_debug("Failed to parse %s node", node->name);

    return TRUE;
}


/**
 * gvir_config_domain_snapshot_get_disks:
 * @snapshot: a #GVirConfigDomainSnapshot
 *
 * Gets the list of disks attached to @snapshot.  The returned list should
 * be freed with g_list_free(), after its elements have been unreffed with
 * g_object_unref().
 *
 * Returns: (element-type LibvirtGConfig.DomainSnapshotDisk) (transfer full):
 * a newly allocated #GList of #GVirConfigDomainSnapshotDisk.
 */
GList *gvir_config_domain_snapshot_get_disks(GVirConfigDomainSnapshot *snapshot)
{
    struct GetDiskData data;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_SNAPSHOT(snapshot), NULL);

    g_object_get(G_OBJECT(snapshot), "doc", &data.doc, NULL);
    data.disks = NULL;
    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(snapshot), "disks",
                                     get_disk, &data);
    if (data.doc != NULL) {
        g_object_unref(G_OBJECT(data.doc));
    }

    return g_list_reverse(data.disks);
}
