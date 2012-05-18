/*
 * libvirt-gobject-domain_snapshot.c: libvirt glib integration
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
#include "libvirt-gobject-compat.h"

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

    g_debug("Finalize GVirDomainSnapshot=%p", conn);

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
                                                       G_PARAM_STATIC_STRINGS));

    g_type_class_add_private(klass, sizeof(GVirDomainSnapshotPrivate));
}


static void gvir_domain_snapshot_init(GVirDomainSnapshot *conn)
{
    g_debug("Init GVirDomainSnapshot=%p", conn);

    conn->priv = GVIR_DOMAIN_SNAPSHOT_GET_PRIVATE(conn);
}

typedef struct virDomainSnapshot GVirDomainSnapshotHandle;

static GVirDomainSnapshotHandle*
gvir_domain_snapshot_handle_copy(GVirDomainSnapshotHandle *src)
{
#if 0
    virDomainSnapshotRef((virDomainSnapshotPtr)src);
#endif
    return src;
}

static void
gvir_domain_snapshot_handle_free(GVirDomainSnapshotHandle *src)
{
    virDomainSnapshotFree((virDomainSnapshotPtr)src);
}

G_DEFINE_BOXED_TYPE(GVirDomainSnapshotHandle, gvir_domain_snapshot_handle,
                    gvir_domain_snapshot_handle_copy, gvir_domain_snapshot_handle_free)

const gchar *gvir_domain_snapshot_get_name(GVirDomainSnapshot *snapshot)
{
    g_return_val_if_fail(GVIR_IS_DOMAIN_SNAPSHOT(snapshot), NULL);
#if 0
    GVirDomainSnapshotPrivate *priv = snapshot->priv;
    const char *name;

    if (!(name = virDomainSnapshotGetName(priv->handle))) {
        g_warning("Failed to get domain_snapshot name on %p", priv->handle);
        return NULL;
    }

    return name;
#else
    if (snapshot)
        return NULL;
#endif

    g_return_val_if_reached(NULL);
}


/**
 * gvir_domain_snapshot_get_config:
 * @snapshot: the domain_snapshot
 * @flags: the flags
 *
 * Returns: (transfer full): the config. The returned object should be
 * unreffed with g_object_unref() when no longer needed.

 */
GVirConfigDomainSnapshot *gvir_domain_snapshot_get_config
                                (GVirDomainSnapshot *snapshot,
                                 guint flags,
                                 GError **err)
{
    GVirDomainSnapshotPrivate *priv;
    gchar *xml;

    g_return_val_if_fail(GVIR_IS_DOMAIN_SNAPSHOT(snapshot), NULL);
    g_return_val_if_fail(err == NULL || *err == NULL, NULL);

    priv = snapshot->priv;
    if (!(xml = virDomainSnapshotGetXMLDesc(priv->handle, flags))) {
        gvir_set_error_literal(err, GVIR_DOMAIN_SNAPSHOT_ERROR,
                               0,
                               "Unable to get domain_snapshot XML config");
        return NULL;
    }

    GVirConfigDomainSnapshot *conf = gvir_config_domain_snapshot_new_from_xml(xml, err);

    free(xml);
    return conf;
}
