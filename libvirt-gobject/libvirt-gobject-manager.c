/*
 * libvirt-gobject-manager.c: libvirt glib integration
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

#include <libvirt/libvirt.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_MANAGER_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_MANAGER, GVirManagerPrivate))

struct _GVirManagerPrivate
{
    GList *connections;
};

G_DEFINE_TYPE(GVirManager, gvir_manager, G_TYPE_OBJECT);


enum {
    PROP_0,
};


#define GVIR_MANAGER_ERROR gvir_manager_error_quark()

#if 0
static GQuark
gvir_manager_error_quark(void)
{
    return g_quark_from_static_string("vir-g-manager");
}
#endif


static void gvir_manager_finalize(GObject *object)
{
    GVirManager *man = GVIR_MANAGER(object);
    GVirManagerPrivate *priv = man->priv;

    DEBUG("Finalize GVirManager=%p", man);

    GList *tmp = priv->connections;
    while (tmp) {
        GVirConnection *conn = tmp->data;
        g_object_unref(conn);
        tmp = tmp->next;
    }
    g_list_free(priv->connections);

    G_OBJECT_CLASS(gvir_manager_parent_class)->finalize(object);
}


static void gvir_manager_class_init(GVirManagerClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_manager_finalize;

    g_type_class_add_private(klass, sizeof(GVirManagerPrivate));
}


static void gvir_manager_init(GVirManager *conn)
{
    GVirManagerPrivate *priv;

    DEBUG("Init GVirManager=%p", conn);

    priv = conn->priv = GVIR_MANAGER_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}


GVirManager *gvir_manager_new(void)
{
    return GVIR_MANAGER(g_object_new(GVIR_TYPE_MANAGER,
                                     NULL));
}



void gvir_manager_add_connection(GVirManager *man,
                                 GVirConnection *conn)
{
    GVirManagerPrivate *priv = man->priv;

    g_object_ref(conn);
    priv->connections = g_list_append(priv->connections, conn);
}

void gvir_manager_remove_connection(GVirManager *man,
                                    GVirConnection *conn)
{
    GVirManagerPrivate *priv = man->priv;

    priv->connections = g_list_remove(priv->connections, conn);
    g_object_unref(conn);
}


/**
 * gvir_manager_get_connections:
 * Returns: (transfer full)(element-type GVirConnection): the connections
 */
GList *gvir_manager_get_connections(GVirManager *man)
{
    GVirManagerPrivate *priv = man->priv;

    GList *tmp = priv->connections;
    while (tmp) {
        GVirConnection *conn = tmp->data;
        g_object_ref(conn);
        tmp = tmp->next;
    }

    return g_list_copy(priv->connections);
}


/**
 * gvir_manager_find_connection_by_uri:
 * Returns: (transfer full)(allow-none): the connection,or NULL
 */
GVirConnection *gvir_manager_find_connection_by_uri(GVirManager *man,
                                                    const gchar *uri)
{
    GVirManagerPrivate *priv = man->priv;

    GList *tmp = priv->connections;
    while (tmp) {
        GVirConnection *conn = tmp->data;
        if (g_strcmp0(gvir_connection_get_uri(conn), uri) == 0)
            return conn;
        tmp = tmp->next;
    }
    return NULL;
}

