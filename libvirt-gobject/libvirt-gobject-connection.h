/*
 * libvirt-gobject-connection.h: libvirt gobject integration
 *
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

#if !defined(__LIBVIRT_GOBJECT_H__) && !defined(LIBVIRT_GOBJECT_BUILD)
#error "Only <libvirt-gobject/libvirt-gobject.h> can be included directly."
#endif

#ifndef __LIBVIRT_GOBJECT_CONNECTION_H__
#define __LIBVIRT_GOBJECT_CONNECTION_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <libvirt-gobject/libvirt-gobject-domain.h>

G_BEGIN_DECLS

#define GVIR_TYPE_CONNECTION            (gvir_connection_get_type ())
#define GVIR_CONNECTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONNECTION, GVirConnection))
#define GVIR_CONNECTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONNECTION, GVirConnectionClass))
#define GVIR_IS_CONNECTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONNECTION))
#define GVIR_IS_CONNECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONNECTION))
#define GVIR_CONNECTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONNECTION, GVirConnectionClass))


typedef struct _GVirConnection GVirConnection;
typedef struct _GVirConnectionPrivate GVirConnectionPrivate;
typedef struct _GVirConnectionClass GVirConnectionClass;

struct _GVirConnection
{
    GObject parent;

    GVirConnectionPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConnectionClass
{
    GObjectClass parent_class;

    gpointer padding[20];
};

GType gvir_connection_get_type(void);

GVirConnection *gvir_connection_new(const char *uri);
gboolean gvir_connection_open(GVirConnection *conn,
                              GError **err);
void gvir_connection_open_async(GVirConnection *conn,
                                GCancellable *cancellable,
                                GAsyncReadyCallback callback,
                                gpointer opaque);
gboolean gvir_connection_open_finish(GVirConnection *conn,
                                     GAsyncResult *result,
                                     GError **err);
gboolean gvir_connection_is_open(GVirConnection *conn);
void gvir_connection_close(GVirConnection *conn);

gboolean gvir_connection_fetch_domains(GVirConnection *conn,
                                       GError **err);

const gchar *gvir_connection_get_uri(GVirConnection *conn);
GList *gvir_connection_get_domains(GVirConnection *conn);

GVirDomain *gvir_connection_get_domain(GVirConnection *conn,
                                       const gchar *uuid);
GVirDomain *gvir_connection_find_domain_by_id(GVirConnection *conn,
                                              gint id);
GVirDomain *gvir_connection_find_domain_by_name(GVirConnection *conn,
                                                const gchar *name);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_CONNECTION_H__ */
