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

G_BEGIN_DECLS

#define VIR_G_TYPE_CONNECTION            (vir_g_connection_get_type ())
#define VIR_G_CONNECTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), VIR_G_TYPE_CONNECTION, VirGConnection))
#define VIR_G_CONNECTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), VIR_G_TYPE_CONNECTION, VirGConnectionClass))
#define VIR_G_IS_CONNECTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIR_G_TYPE_CONNECTION))
#define VIR_G_IS_CONNECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), VIR_G_TYPE_CONNECTION))
#define VIR_G_CONNECTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), VIR_G_TYPE_CONNECTION, VirGConnectionClass))


typedef struct _VirGConnection VirGConnection;
typedef struct _VirGConnectionPrivate VirGConnectionPrivate;
typedef struct _VirGConnectionClass VirGConnectionClass;

struct _VirGConnection
{
    GObject parent;

    VirGConnectionPrivate *priv;

    /* Do not add fields to this struct */
};

struct _VirGConnectionClass
{
    GObjectClass parent_class;

    gpointer padding[20];
};

GType vir_g_connection_get_type(void);

VirGConnection *vir_g_connection_new(const char *uri);
void vir_g_connection_open(VirGConnection *conn,
                           GCancellable *cancellable,
                           GAsyncReadyCallback callback,
                           gpointer opaque);
gboolean vir_g_connection_open_finish(VirGConnection *conn,
                                      GAsyncResult *result,
                                      GError **err);
gboolean vir_g_connection_is_open(VirGConnection *conn);
void vir_g_connection_close(VirGConnection *conn);

G_END_DECLS

#endif /* __LIBVIRT_GOBJECT_CONNECTION_H__ */
