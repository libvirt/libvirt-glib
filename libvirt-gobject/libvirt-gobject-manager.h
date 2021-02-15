/*
 * libvirt-gobject-manager.h: libvirt gobject integration
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

#define GVIR_TYPE_MANAGER            (gvir_manager_get_type ())
#define GVIR_MANAGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_MANAGER, GVirManager))
#define GVIR_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_MANAGER, GVirManagerClass))
#define GVIR_IS_MANAGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_MANAGER))
#define GVIR_IS_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_MANAGER))
#define GVIR_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_MANAGER, GVirManagerClass))


typedef struct _GVirManager GVirManager;
typedef struct _GVirManagerPrivate GVirManagerPrivate;
typedef struct _GVirManagerClass GVirManagerClass;

struct _GVirManager
{
    GObject parent;

    GVirManagerPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirManagerClass
{
    GObjectClass parent_class;

    /* Signals */
    void (*connection_added)(GVirManager *man, GVirConnection *conn);
    void (*connection_removed)(GVirManager *man, GVirConnection *conn);

    gpointer padding[20];
};

GType gvir_manager_get_type(void);

GVirManager *gvir_manager_new(void);

void gvir_manager_add_connection(GVirManager *man,
                                 GVirConnection *conn);

void gvir_manager_remove_connection(GVirManager *man,
                                    GVirConnection *conn);

GList *gvir_manager_get_connections(GVirManager *man);

GVirConnection *gvir_manager_find_connection_by_uri(GVirManager *man,
                                                    const gchar *uri);

G_END_DECLS
