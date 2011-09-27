/*
 * libvirt-gobject-domain_snapshot.c: libvirt glib integration
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

#include <libvirt/virterror.h>
#include <string.h>

#include "libvirt-glib/libvirt-glib.h"
#include "libvirt-gobject/libvirt-gobject.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_DOMAIN_SNAPSHOT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_DOMAIN_SNAPSHOT, GVirDomainSnapshotPrivate))

struct _GVirDomainSnapshotPrivate
{
    virDomainSnapshotPtr handle;
};

G_DEFINE_TYPE(GVirDomainSnapshot, gvir_domain_snapshot, G_TYPE_OBJECT);


enum {
    PROP_0,
    PROP_HANDLE,
};


#define GVIR_DOMAIN_SNAPSHOT_ERROR gvir_domain_snapshot_error_quark()


static GQuark
gvir_domain_snapshot_error_quark(void)
{
    return g_quark_from_static_string("gvir-domain-snapshot");
}

static void gvir_domain_snapshot_get_property(GObject *object,
                                              guint prop_id,
                                              GValue *value,
                                              GParamSpec *pspec)
{
    GVirDomainSnapshot *conn = GVIR_DOMAIN_SNAPSHOT(object);
    GVirDomainSnapshotPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        g_value_set_boxed(value, priv->handle);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_snapshot_set_property(GObject *object,
                                              guint prop_id,
                                              const GValue *value,
                                              GParamSpec *pspec)
{
    GVirDomainSnapshot *conn = GVIR_DOMAIN_SNAPSHOT(object);
    GVirDomainSnapshotPrivate *priv = conn->priv;

    switch (prop_id) {
    case PROP_HANDLE:
        if (priv->handle)
            virDomainSnapshotFree(priv->handle);
        priv->handle = g_value_dup_boxed(value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_domain_snapshot_finalize(GObject *object)
{
    GVirDomainSnapshot *conn = GVIR_DOMAIN_SNAPSHOT(object);
    GVirDomainSnapshotPrivate *priv = conn->priv;

    DEBUG("Finalize GVirDomainSnapshot=%p", conn);

    virDomainSnapshotFree(priv->handle);

    G_OBJECT_CLASS(gvir_domain_snapshot_parent_class)->finalize(object);
}


static void gvir_domain_snapshot_class_init(GVirDomainSnapshotClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = gvir_domain_snapshot_finalize;
    object_class->get_property = gvir_domain_snapshot_get_property;
    object_class->set_property = gvir_domain_snapshot_set_property;

    g_object_class_install_property(object_class,
                                    PROP_HANDLE,
                                    g_param_spec_boxed("handle",
                                                       "Handle",
                                                       "The domain_snapshot handle",
                                                       GVIR_TYPE_DOMAIN_SNAPSHOT_HANDLE,
                                                       G_PARAM_READABLE |
                                                       G_PARAM_WRITABLE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_NAME |
                                                       G_PARAM_STATIC_NICK |
                                                       G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(GVirDomainSnapshotPrivate));
}


static void gvir_domain_snapshot_init(GVirDomainSnapshot *conn)
{
    GVirDomainSnapshotPrivate *priv;

    DEBUG("Init GVirDomainSnapshot=%p", conn);

    priv = conn->priv = GVIR_DOMAIN_SNAPSHOT_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}

static gpointer
gvir_domain_snapshot_handle_copy(gpointer src)
{
#if 0
    virDomainSnapshotRef(src);
#endif
    return src;
}


GType gvir_domain_snapshot_handle_get_type(void)
{
    static GType handle_type = 0;

    if (G_UNLIKELY(handle_type == 0))
        handle_type = g_boxed_type_register_static
            ("GVirDomainSnapshotHandle",
             gvir_domain_snapshot_handle_copy,
             (GBoxedFreeFunc)virDomainSnapshotFree);

    return handle_type;
}


const gchar *gvir_domain_snapshot_get_name(GVirDomainSnapshot *snapshot)
{
#if 0
    GVirDomainSnapshotPrivate *priv = snapshot->priv;
    const char *name;

    if (!(name = virDomainSnapshotGetName(priv->handle))) {
        g_error("Failed to get domain_snapshot name on %p", priv->handle);
    }

    return name;
#else
    if (snapshot || !snapshot)
        return NULL;
#endif
}


/**
 * gvir_domain_snapshot_get_config:
 * @snapshot: the domain_snapshot
 * @flags: the flags
 * Returns: (transfer full): the config
 */
GVirConfigDomainSnapshot *gvir_domain_snapshot_get_config
                                (GVirDomainSnapshot *snapshot,
                                 guint64 flags,
                                 GError **err)
{
    GVirDomainSnapshotPrivate *priv = snapshot->priv;
    gchar *xml;

    if (!(xml = virDomainSnapshotGetXMLDesc(priv->handle, flags))) {
        *err = gvir_error_new_literal(GVIR_DOMAIN_SNAPSHOT_ERROR,
                                      0,
                                      "Unable to get domain_snapshot XML config");
        return NULL;
    }

    GVirConfigDomainSnapshot *conf = gvir_config_domain_snapshot_new(xml);

    g_free(xml);
    return conf;
}
