/*
 * libvirt-gobject-domain_config.c: libvirt glib integration
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

#define GVIR_DOMAIN_CONFIG_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_DOMAIN_CONFIG, GVirDomainConfigPrivate))

struct _GVirDomainConfigPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirDomainConfig, gvir_domain_config, GVIR_TYPE_XML_CONFIG);


static void gvir_domain_config_class_init(GVirDomainConfigClass *klass)
{

    g_type_class_add_private(klass, sizeof(GVirDomainConfigPrivate));
}


static void gvir_domain_config_init(GVirDomainConfig *conn)
{
    GVirDomainConfigPrivate *priv;

    DEBUG("Init GVirDomainConfig=%p", conn);

    priv = conn->priv = GVIR_DOMAIN_CONFIG_GET_PRIVATE(conn);

    memset(priv, 0, sizeof(*priv));
}


GVirDomainConfig *gvir_domain_config_new(const gchar *xml)
{
    return GVIR_DOMAIN_CONFIG(g_object_new(GVIR_TYPE_DOMAIN_CONFIG,
                                           "doc", xml,
                                           "schema", DATADIR "/libvirt/schemas/domain.rng",
                                           NULL));
}
