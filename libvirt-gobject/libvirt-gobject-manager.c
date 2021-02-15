/*
 * libvirt-gobject-manager.c: libvirt glib integration
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

#include <libvirt/libvirt.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"
#include "libvirt-gobject/libvirt-gobject-compat.h"

#define GVIR_MANAGER_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_MANAGER, GVirManagerPrivate))

struct _GVirManagerPrivate
{
    GMutex *lock;
    GList *connections;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirManager, gvir_manager, G_TYPE_OBJECT);


enum {
    PROP_0,
};


#if 0
#define GVIR_MANAGER_ERROR gvir_manager_error_quark()

static GQuark
gvir_manager_error_quark(void)
{
    return g_quark_from_static_string("gvir-manager");
}
#endif


static void gvir_manager_finalize(GObject *object)
{
    GVirManager *man = GVIR_MANAGER(object);
    GVirManagerPrivate *priv = man->priv;

    GList *tmp = priv->connections;
    while (tmp) {
        GVirConnection *conn = tmp->data;
        g_object_unref(conn);
        tmp = tmp->next;
    }
    g_list_free(priv->connections);

    g_mutex_clear(priv->lock);
    g_free(priv->lock);

    G_OBJECT_CLASS(gvir_manager_parent_class)->finalize(object);
}


static void gvir_manager_class_init(GVirManagerClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_manager_finalize;

    g_signal_new("connection-added",
                 G_OBJECT_CLASS_TYPE(object_class),
                 G_SIGNAL_RUN_FIRST,
                 G_STRUCT_OFFSET(GVirManagerClass, connection_added),
                 NULL, NULL,
                 g_cclosure_marshal_VOID__OBJECT,
                 G_TYPE_NONE,
                 1,
                 GVIR_TYPE_CONNECTION);
    g_signal_new("connection-removed",
                 G_OBJECT_CLASS_TYPE(object_class),
                 G_SIGNAL_RUN_FIRST,
                 G_STRUCT_OFFSET(GVirManagerClass, connection_removed),
                 NULL, NULL,
                 g_cclosure_marshal_VOID__OBJECT,
                 G_TYPE_NONE,
                 1,
                 GVIR_TYPE_CONNECTION);
}


static void gvir_manager_init(GVirManager *manager)
{
    GVirManagerPrivate *priv;

    priv = manager->priv = GVIR_MANAGER_GET_PRIVATE(manager);

    priv->lock = g_new0(GMutex, 1);
    g_mutex_init(priv->lock);
}


GVirManager *gvir_manager_new(void)
{
    return GVIR_MANAGER(g_object_new(GVIR_TYPE_MANAGER,
                                     NULL));
}



void gvir_manager_add_connection(GVirManager *man,
                                 GVirConnection *conn)
{
    GVirManagerPrivate *priv;

    g_return_if_fail(GVIR_IS_MANAGER(man));
    g_return_if_fail(GVIR_IS_CONNECTION(conn));

    priv = man->priv;
    g_mutex_lock(priv->lock);
    g_object_ref(conn);
    priv->connections = g_list_append(priv->connections, conn);

    /* Hold extra reference while emitting signal */
    g_object_ref(conn);
    g_mutex_unlock(priv->lock);
    g_signal_emit_by_name(man, "connection-added", conn);
    g_object_unref(conn);
}

void gvir_manager_remove_connection(GVirManager *man,
                                    GVirConnection *conn)
{
    GVirManagerPrivate *priv;

    g_return_if_fail(GVIR_IS_MANAGER(man));
    g_return_if_fail(GVIR_IS_CONNECTION(conn));

    priv = man->priv;
    g_mutex_lock(priv->lock);
    priv->connections = g_list_remove(priv->connections, conn);
    g_mutex_unlock(priv->lock);

    g_signal_emit_by_name(man, "connection-removed", conn);
    g_object_unref(conn);
}


/**
 * gvir_manager_get_connections:
 *
 * Returns: (transfer full)(element-type GVirConnection): the connections.
 * The returned list should be freed with g_list_free(), after its elements
 * have been unreffed with g_object_unref().
 */
GList *gvir_manager_get_connections(GVirManager *man)
{
    GList *tmp;

    g_return_val_if_fail(GVIR_IS_MANAGER(man), NULL);

    tmp = man->priv->connections;
    while (tmp) {
        GVirConnection *conn = tmp->data;
        g_object_ref(conn);
        tmp = tmp->next;
    }

    return g_list_copy(man->priv->connections);
}


/**
 * gvir_manager_find_connection_by_uri:
 *
 * Returns: (transfer full)(allow-none): the connection,or NULL. The
 * returned object should be unreffed with g_object_unref() when no longer
 * needed.
 */
GVirConnection *gvir_manager_find_connection_by_uri(GVirManager *man,
                                                    const gchar *uri)
{
    GList *tmp;

    g_return_val_if_fail(GVIR_IS_MANAGER(man), NULL);
    g_return_val_if_fail(uri != NULL, NULL);

    tmp = man->priv->connections;
    while (tmp) {
        GVirConnection *conn = tmp->data;
        if (g_strcmp0(gvir_connection_get_uri(conn), uri) == 0)
            return conn;
        tmp = tmp->next;
    }
    return NULL;
}
