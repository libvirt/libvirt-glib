/*
 * libvirt-gobject-connection.h: libvirt gobject integration
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

#define GVIR_TYPE_CONNECTION            (gvir_connection_get_type ())
#define GVIR_CONNECTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_TYPE_CONNECTION, GVirConnection))
#define GVIR_CONNECTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_TYPE_CONNECTION, GVirConnectionClass))
#define GVIR_IS_CONNECTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_TYPE_CONNECTION))
#define GVIR_IS_CONNECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_TYPE_CONNECTION))
#define GVIR_CONNECTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_TYPE_CONNECTION, GVirConnectionClass))

#define GVIR_TYPE_CONNECTION_HANDLE      (gvir_connection_handle_get_type ())

typedef struct _GVirNodeInfo GVirNodeInfo;
struct _GVirNodeInfo
{
    gchar model[32]; /* string indicating the CPU model */
    gulong memory;   /* memory size in kilobytes */
    guint cpus;      /* the number of active CPUs */
    guint mhz;       /* expected CPU frequency */
    guint nodes;     /* the number of NUMA cell, 1 for unusual NUMA topologies or uniform memo */
    guint sockets;   /* number of CPU sockets per node if nodes > 1, total number of CPU socke */
    guint cores;     /* number of cores per socket */
    guint threads;   /* number of threads per core */
};

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

    /* signals */
    void (*connection_opened)(GVirConnection *conn);
    void (*connection_closed)(GVirConnection *conn);

    void (*domain_added)(GVirConnection *conn, GVirDomain *dom);
    void (*domain_removed)(GVirConnection *conn, GVirDomain *dom);

    GVirStream *(*stream_new)(GVirConnection *conn, gpointer handle);

    gpointer padding[20];
};

GType gvir_connection_get_type(void);
GType gvir_connection_handle_get_type(void);
GType gvir_node_info_get_type(void);

GVirConnection *gvir_connection_new(const char *uri);
gboolean gvir_connection_open(GVirConnection *conn,
                              GCancellable *cancellable,
                              GError **err);
gboolean gvir_connection_open_read_only(GVirConnection *conn,
                                        GCancellable *cancellable,
                                        GError **err);
void gvir_connection_open_async(GVirConnection *conn,
                                GCancellable *cancellable,
                                GAsyncReadyCallback callback,
                                gpointer user_data);
gboolean gvir_connection_open_finish(GVirConnection *conn,
                                     GAsyncResult *result,
                                     GError **err);
void gvir_connection_open_read_only_async(GVirConnection *conn,
                                          GCancellable *cancellable,
                                          GAsyncReadyCallback callback,
                                          gpointer user_data);
gboolean gvir_connection_open_read_only_finish(GVirConnection *conn,
                                               GAsyncResult *result,
                                               GError **err);
gboolean gvir_connection_is_open(GVirConnection *conn);
gboolean gvir_connection_is_read_only(GVirConnection *conn);
void gvir_connection_close(GVirConnection *conn);

gboolean gvir_connection_fetch_domains(GVirConnection *conn,
                                       GCancellable *cancellable,
                                       GError **err);
void gvir_connection_fetch_domains_async(GVirConnection *conn,
                                         GCancellable *cancellable,
                                         GAsyncReadyCallback callback,
                                         gpointer user_data);
gboolean gvir_connection_fetch_domains_finish(GVirConnection *conn,
                                              GAsyncResult *result,
                                              GError **err);


const gchar *gvir_connection_get_uri(GVirConnection *conn);

gchar *gvir_connection_get_hypervisor_name(GVirConnection *conn,
                                           GError **err);
gulong gvir_connection_get_version(GVirConnection *conn,
                                   GError **err);

GList *gvir_connection_get_domains(GVirConnection *conn);
GVirDomain *gvir_connection_get_domain(GVirConnection *conn,
                                       const gchar *uuid);
GVirDomain *gvir_connection_find_domain_by_id(GVirConnection *conn,
                                              gint id);
GVirDomain *gvir_connection_find_domain_by_name(GVirConnection *conn,
                                                const gchar *name);

GVirDomain *gvir_connection_create_domain(GVirConnection *conn,
                                          GVirConfigDomain *conf,
                                          GError **err);
GVirDomain *gvir_connection_start_domain(GVirConnection *conn,
                                         GVirConfigDomain *conf,
                                         guint flags,
                                         GError **err);

gboolean gvir_connection_fetch_interfaces(GVirConnection *conn,
                                          GCancellable *cancellable,
                                          GError **err);
void gvir_connection_fetch_interfaces_async(GVirConnection *conn,
                                            GCancellable *cancellable,
                                            GAsyncReadyCallback callback,
                                            gpointer user_data);
gboolean gvir_connection_fetch_interfaces_finish(GVirConnection *conn,
                                                 GAsyncResult *result,
                                                 GError **err);
GList *gvir_connection_get_interfaces(GVirConnection *conn);
GVirInterface *gvir_connection_get_interface(GVirConnection *conn,
                                             const gchar *name);
GVirInterface *gvir_connection_find_interface_by_mac(GVirConnection *conn,
                                                     const gchar *macaddr);

gboolean gvir_connection_fetch_networks(GVirConnection *conn,
                                          GCancellable *cancellable,
                                          GError **err);
void gvir_connection_fetch_networks_async(GVirConnection *conn,
                                          GCancellable *cancellable,
                                          GAsyncReadyCallback callback,
                                          gpointer user_data);
gboolean gvir_connection_fetch_networks_finish(GVirConnection *conn,
                                               GAsyncResult *result,
                                               GError **err);
GList *gvir_connection_get_networks(GVirConnection *conn);
GVirNetwork *gvir_connection_get_network(GVirConnection *conn,
                                         const gchar *uuid);
GVirNetwork *gvir_connection_find_network_by_name(GVirConnection *conn,
                                                  const gchar *name);

#if 0
GList *gvir_connection_get_network_filters(GVirConnection *conn);
GVirNetworkFilter *gvir_connection_get_network_filter(GVirConnection *conn,
                                                      const gchar *uuid);
GVirNetworkFilter *gvir_connection_find_network_filter_by_name(GVirConnection *conn,
                                                               const gchar *name);


GList *gvir_connection_get_node_devices(GVirConnection *conn);
GVirNodeDevice *gvir_connection_get_node_device(GVirConnection *conn,
                                                const gchar *name);


GList *gvir_connection_get_secrets(GVirConnection *conn);
GVirSecret *gvir_connection_get_secret(GVirConnection *conn,
                                       const gchar *uuid);
#endif

gboolean gvir_connection_fetch_storage_pools(GVirConnection *conn,
                                             GCancellable *cancellable,
                                             GError **err);
void gvir_connection_fetch_storage_pools_async(GVirConnection *conn,
                                               GCancellable *cancellable,
                                               GAsyncReadyCallback callback,
                                               gpointer user_data);
gboolean gvir_connection_fetch_storage_pools_finish(GVirConnection *conn,
                                                    GAsyncResult *result,
                                                    GError **err);

GList *gvir_connection_get_storage_pools(GVirConnection *conn);
GVirStoragePool *gvir_connection_get_storage_pool(GVirConnection *conn,
                                                  const gchar *uuid);
GVirStoragePool *gvir_connection_find_storage_pool_by_name(GVirConnection *conn,
                                                           const gchar *name);

GVirStoragePool *gvir_connection_create_storage_pool
                                (GVirConnection *conn,
                                 GVirConfigStoragePool *conf,
                                 guint flags,
                                 GError **err);


GVirStream *gvir_connection_get_stream(GVirConnection *conn,
                                       guint flags);

GVirNodeInfo *gvir_connection_get_node_info(GVirConnection *conn,
                                            GError **err);

GVirConfigCapabilities *gvir_connection_get_capabilities(GVirConnection *conn,
                                                         GError **err);
void gvir_connection_get_capabilities_async(GVirConnection *conn,
                                            GCancellable *cancellable,
                                            GAsyncReadyCallback callback,
                                            gpointer user_data);
GVirConfigCapabilities *
gvir_connection_get_capabilities_finish(GVirConnection *conn,
                                        GAsyncResult *result,
                                        GError **err);

gboolean
gvir_connection_restore_domain_from_file(GVirConnection *conn,
                                         gchar *filename,
                                         GVirConfigDomain *custom_conf,
                                         guint flags,
                                         GError **err);

void
gvir_connection_restore_domain_from_file_async(GVirConnection *conn,
                                               gchar *filename,
                                               GVirConfigDomain *custom_conf,
                                               guint flags,
                                               GCancellable *cancellable,
                                               GAsyncReadyCallback callback,
                                               gpointer user_data);

gboolean
gvir_connection_restore_domain_from_file_finish(GVirConnection *conn,
                                                GAsyncResult *result,
                                                GError **err);

GVirConfigDomainCapabilities *gvir_connection_get_domain_capabilities(GVirConnection *conn,
                                                                      const gchar *emulatorbin,
                                                                      const gchar *arch,
                                                                      const gchar *machine,
                                                                      const gchar *virttype,
                                                                      guint flags,
                                                                      GError **err);
void gvir_connection_get_domain_capabilities_async(GVirConnection *conn,
                                                   const gchar *emulatorbin,
                                                   const gchar *arch,
                                                   const gchar *machine,
                                                   const gchar *virttype,
                                                   guint flags,
                                                   GCancellable *cancellable,
                                                   GAsyncReadyCallback callback,
                                                   gpointer user_data);
GVirConfigDomainCapabilities *
gvir_connection_get_domain_capabilities_finish(GVirConnection *conn,
                                               GAsyncResult *result,
                                               GError **err);

G_END_DECLS
