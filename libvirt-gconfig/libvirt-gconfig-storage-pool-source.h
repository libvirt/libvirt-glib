/*
 * libvirt-gconfig-storage-pool-target.c: libvirt storage pool target configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_STORAGE_POOL_SOURCE            (gvir_config_storage_pool_source_get_type ())
#define GVIR_CONFIG_STORAGE_POOL_SOURCE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_STORAGE_POOL_SOURCE, GVirConfigStoragePoolSource))
#define GVIR_CONFIG_STORAGE_POOL_SOURCE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_STORAGE_POOL_SOURCE, GVirConfigStoragePoolSourceClass))
#define GVIR_CONFIG_IS_STORAGE_POOL_SOURCE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_STORAGE_POOL_SOURCE))
#define GVIR_CONFIG_IS_STORAGE_POOL_SOURCE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_STORAGE_POOL_SOURCE))
#define GVIR_CONFIG_STORAGE_POOL_SOURCE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_STORAGE_POOL_SOURCE, GVirConfigStoragePoolSourceClass))

typedef struct _GVirConfigStoragePoolSource GVirConfigStoragePoolSource;
typedef struct _GVirConfigStoragePoolSourcePrivate GVirConfigStoragePoolSourcePrivate;
typedef struct _GVirConfigStoragePoolSourceClass GVirConfigStoragePoolSourceClass;

struct _GVirConfigStoragePoolSource
{
    GVirConfigObject parent;

    GVirConfigStoragePoolSourcePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigStoragePoolSourceClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


GType gvir_config_storage_pool_source_get_type(void);

GVirConfigStoragePoolSource *gvir_config_storage_pool_source_new(void);
GVirConfigStoragePoolSource *gvir_config_storage_pool_source_new_from_xml(const gchar *xml,
                                                                          GError **error);

const char *gvir_config_storage_pool_source_get_adapter(GVirConfigStoragePoolSource *source);
void gvir_config_storage_pool_source_set_adapter(GVirConfigStoragePoolSource *source,
                                                 const char *adapter);
const char *gvir_config_storage_pool_source_get_device_path(GVirConfigStoragePoolSource *source);
void gvir_config_storage_pool_source_set_device_path(GVirConfigStoragePoolSource *source,
                                                     const char *device_path);
const char *gvir_config_storage_pool_source_get_directory(GVirConfigStoragePoolSource *source);
void gvir_config_storage_pool_source_set_directory(GVirConfigStoragePoolSource *source,
                                                   const char *directory);
const char *gvir_config_storage_pool_source_get_format(GVirConfigStoragePoolSource *source);
void gvir_config_storage_pool_source_set_format(GVirConfigStoragePoolSource *source,
                                                const char *format);
const char *gvir_config_storage_pool_source_get_host(GVirConfigStoragePoolSource *source);
void gvir_config_storage_pool_source_set_host(GVirConfigStoragePoolSource *source,
                                              const char *host);
const char *gvir_config_storage_pool_source_get_name(GVirConfigStoragePoolSource *source);
void gvir_config_storage_pool_source_set_name(GVirConfigStoragePoolSource *source,
                                              const char *name);
const char *gvir_config_storage_pool_source_get_product(GVirConfigStoragePoolSource *source);
void gvir_config_storage_pool_source_set_product(GVirConfigStoragePoolSource *source,
                                                 const char *product);
const char *gvir_config_storage_pool_source_get_vendor(GVirConfigStoragePoolSource *source);
void gvir_config_storage_pool_source_set_vendor(GVirConfigStoragePoolSource *source,
                                                const char *vendor);

G_END_DECLS
