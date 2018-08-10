/*
 * libvirt-gconfig-secret.c: libvirt secret configuration
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

#include "libvirt-gconfig/libvirt-gconfig.h"

#define GVIR_CONFIG_SECRET_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_SECRET, GVirConfigSecretPrivate))

struct _GVirConfigSecretPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigSecret, gvir_config_secret, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_secret_class_init(GVirConfigSecretClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_secret_init(GVirConfigSecret *secret)
{
    secret->priv = GVIR_CONFIG_SECRET_GET_PRIVATE(secret);
}


GVirConfigSecret *gvir_config_secret_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_SECRET,
                                    "secret",
                                    DATADIR "/libvirt/schemas/secret.rng");
    return GVIR_CONFIG_SECRET(object);
}

GVirConfigSecret *gvir_config_secret_new_from_xml(const gchar *xml,
                                                  GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_SECRET,
                                             "secret",
                                             DATADIR "/libvirt/schemas/secret.rng",
                                             xml, error);
    return GVIR_CONFIG_SECRET(object);
}
