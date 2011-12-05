/*
 * libvirt-gconfig-storage-pool-source.c: libvirt storage pool source configuration
 *
 * Copyright (C) 2011 Red Hat, Inc.
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-object-private.h"


#define GVIR_CONFIG_STORAGE_POOL_SOURCE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_STORAGE_POOL_SOURCE, GVirConfigStoragePoolSourcePrivate))

struct _GVirConfigStoragePoolSourcePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigStoragePoolSource, gvir_config_storage_pool_source, GVIR_TYPE_CONFIG_OBJECT);


static void gvir_config_storage_pool_source_class_init(GVirConfigStoragePoolSourceClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigStoragePoolSourcePrivate));
}


static void gvir_config_storage_pool_source_init(GVirConfigStoragePoolSource *source)
{
    g_debug("Init GVirConfigStoragePoolSource=%p", source);

    source->priv = GVIR_CONFIG_STORAGE_POOL_SOURCE_GET_PRIVATE(source);
}


GVirConfigStoragePoolSource *gvir_config_storage_pool_source_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_TYPE_CONFIG_STORAGE_POOL_SOURCE,
                                    "source", NULL);
    return GVIR_CONFIG_STORAGE_POOL_SOURCE(object);
}

GVirConfigStoragePoolSource *gvir_config_storage_pool_source_new_from_xml(const gchar *xml,
                                                           GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_TYPE_CONFIG_STORAGE_POOL_SOURCE,
                                             "source", NULL,
                                             xml, error);
    return GVIR_CONFIG_STORAGE_POOL_SOURCE(object);
}
