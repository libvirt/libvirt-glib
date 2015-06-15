/*
 * libvirt-gconfig-domain-filesys.h: libvirt domain filesystem configuration
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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_FILESYS_H__
#define __LIBVIRT_GCONFIG_DOMAIN_FILESYS_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_FILESYS            (gvir_config_domain_filesys_get_type ())
#define GVIR_CONFIG_DOMAIN_FILESYS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_FILESYS, GVirConfigDomainFilesys))
#define GVIR_CONFIG_DOMAIN_FILESYS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_FILESYS, GVirConfigDomainFilesysClass))
#define GVIR_CONFIG_IS_DOMAIN_FILESYS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_FILESYS))
#define GVIR_CONFIG_IS_DOMAIN_FILESYS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_FILESYS))
#define GVIR_CONFIG_DOMAIN_FILESYS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_FILESYS, GVirConfigDomainFilesysClass))

typedef struct _GVirConfigDomainFilesys GVirConfigDomainFilesys;
typedef struct _GVirConfigDomainFilesysPrivate GVirConfigDomainFilesysPrivate;
typedef struct _GVirConfigDomainFilesysClass GVirConfigDomainFilesysClass;

struct _GVirConfigDomainFilesys
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainFilesysPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainFilesysClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_FILESYS_MOUNT,
    GVIR_CONFIG_DOMAIN_FILESYS_BLOCK,
    GVIR_CONFIG_DOMAIN_FILESYS_FILE,
    GVIR_CONFIG_DOMAIN_FILESYS_TEMPLATE,
    GVIR_CONFIG_DOMAIN_FILESYS_RAM,
    GVIR_CONFIG_DOMAIN_FILESYS_BIND,
} GVirConfigDomainFilesysType;

typedef enum {
    GVIR_CONFIG_DOMAIN_FILESYS_ACCESS_PASSTHROUGH,
    GVIR_CONFIG_DOMAIN_FILESYS_ACCESS_MAPPED,
    GVIR_CONFIG_DOMAIN_FILESYS_ACCESS_SQUASH,
} GVirConfigDomainFilesysAccessType;

typedef enum {
    GVIR_CONFIG_DOMAIN_FILESYS_DRIVER_DEFAULT,
    GVIR_CONFIG_DOMAIN_FILESYS_DRIVER_PATH,
    GVIR_CONFIG_DOMAIN_FILESYS_DRIVER_HANDLE,
    GVIR_CONFIG_DOMAIN_FILESYS_DRIVER_LOOP,
    GVIR_CONFIG_DOMAIN_FILESYS_DRIVER_NBD,
    GVIR_CONFIG_DOMAIN_FILESYS_DRIVER_PLOOP,
} GVirConfigDomainFilesysDriverType;

GType gvir_config_domain_filesys_get_type(void);

GVirConfigDomainFilesys *gvir_config_domain_filesys_new(void);
GVirConfigDomainFilesys *gvir_config_domain_filesys_new_from_xml(const gchar *xml,
                                                                 GError **error);

void gvir_config_domain_filesys_set_type(GVirConfigDomainFilesys *filesys,
                                         GVirConfigDomainFilesysType type);
void gvir_config_domain_filesys_set_access_type(GVirConfigDomainFilesys *filesys,
                                                GVirConfigDomainFilesysAccessType type);
void gvir_config_domain_filesys_set_driver_type(GVirConfigDomainFilesys *filesys,
                                                GVirConfigDomainFilesysDriverType type);
void gvir_config_domain_filesys_set_driver_format(GVirConfigDomainFilesys *filesys,
                                                  GVirConfigDomainDiskFormat format);
void gvir_config_domain_filesys_set_source(GVirConfigDomainFilesys *filesys,
                                           const char *source);
void gvir_config_domain_filesys_set_ram_usage(GVirConfigDomainFilesys *filesys,
                                              guint64 bytes);
void gvir_config_domain_filesys_set_target(GVirConfigDomainFilesys *filesys,
                                           const char *target);
void gvir_config_domain_filesys_set_readonly(GVirConfigDomainFilesys *filesys,
                                             gboolean readonly);
G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_FILESYS_H__ */
