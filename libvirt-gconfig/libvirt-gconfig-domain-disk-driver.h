/*
 * libvirt-gconfig-domain-disk-driver.h: libvirt disk driver configuration
 *
 * Copyright (C) 2011, 2013 Red Hat, Inc.
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

#ifndef __LIBVIRT_GCONFIG_DOMAIN_DISK_DRIVER_H__
#define __LIBVIRT_GCONFIG_DOMAIN_DISK_DRIVER_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER            (gvir_config_domain_disk_driver_get_type ())
#define GVIR_CONFIG_DOMAIN_DISK_DRIVER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER, GVirConfigDomainDiskDriver))
#define GVIR_CONFIG_DOMAIN_DISK_DRIVER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER, GVirConfigDomainDiskDriverClass))
#define GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER))
#define GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER))
#define GVIR_CONFIG_DOMAIN_DISK_DRIVER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER, GVirConfigDomainDiskDriverClass))

typedef struct _GVirConfigDomainDiskDriver GVirConfigDomainDiskDriver;
typedef struct _GVirConfigDomainDiskDriverPrivate GVirConfigDomainDiskDriverPrivate;
typedef struct _GVirConfigDomainDiskDriverClass GVirConfigDomainDiskDriverClass;

struct _GVirConfigDomainDiskDriver
{
    GVirConfigObject parent;

    GVirConfigDomainDiskDriverPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainDiskDriverClass
{
    GVirConfigObjectClass parent_class;

    gpointer padding[20];
};


typedef enum {
    GVIR_CONFIG_DOMAIN_DISK_CACHE_DEFAULT,
    GVIR_CONFIG_DOMAIN_DISK_CACHE_NONE,
    GVIR_CONFIG_DOMAIN_DISK_CACHE_WRITETHROUGH,
    GVIR_CONFIG_DOMAIN_DISK_CACHE_WRITEBACK,
    GVIR_CONFIG_DOMAIN_DISK_CACHE_DIRECTSYNC,
    GVIR_CONFIG_DOMAIN_DISK_CACHE_UNSAFE
} GVirConfigDomainDiskCacheType;


typedef enum {
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_RAW,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_DIR,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_BOCHS,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_CLOOP,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_COW,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_DMG,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_ISO,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_QCOW,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_QCOW2,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_QED,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_VMDK,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_VPC,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_FAT,
      GVIR_CONFIG_DOMAIN_DISK_FORMAT_VHD,
} GVirConfigDomainDiskFormat;


typedef enum {
    GVIR_CONFIG_DOMAIN_DISK_DRIVER_ERROR_POLICY_STOP,
    GVIR_CONFIG_DOMAIN_DISK_DRIVER_ERROR_POLICY_REPORT,
    GVIR_CONFIG_DOMAIN_DISK_DRIVER_ERROR_POLICY_IGNORE,
    GVIR_CONFIG_DOMAIN_DISK_DRIVER_ERROR_POLICY_ENOSPACE
} GVirConfigDomainDiskDriverErrorPolicy;


typedef enum {
    GVIR_CONFIG_DOMAIN_DISK_DRIVER_IO_POLICY_THREADS,
    GVIR_CONFIG_DOMAIN_DISK_DRIVER_IO_POLICY_NATIVE
} GVirConfigDomainDiskDriverIoPolicy;


typedef enum {
    GVIR_CONFIG_DOMAIN_DISK_DRIVER_DISCARD_UNMAP,
    GVIR_CONFIG_DOMAIN_DISK_DRIVER_DISCARD_IGNORE
} GVirConfigDomainDiskDriverDiscard;


GType gvir_config_domain_disk_driver_get_type(void);

GVirConfigDomainDiskDriver *gvir_config_domain_disk_driver_new(void);
GVirConfigDomainDiskDriver *gvir_config_domain_disk_driver_new_from_xml(const gchar *xml,
                                                                        GError **error);

void gvir_config_domain_disk_driver_set_cache(GVirConfigDomainDiskDriver *driver,
                                              GVirConfigDomainDiskCacheType cache_type);
GVirConfigDomainDiskCacheType gvir_config_domain_disk_driver_get_cache(GVirConfigDomainDiskDriver *driver);

void gvir_config_domain_disk_driver_set_name(GVirConfigDomainDiskDriver *driver,
                                             const char *name);
const char *gvir_config_domain_disk_driver_get_name(GVirConfigDomainDiskDriver *driver);

void gvir_config_domain_disk_driver_set_error_policy(GVirConfigDomainDiskDriver *driver,
                                                     GVirConfigDomainDiskDriverErrorPolicy policy);
GVirConfigDomainDiskDriverErrorPolicy gvir_config_domain_disk_driver_get_error_policy(GVirConfigDomainDiskDriver *driver);

void gvir_config_domain_disk_driver_set_format(GVirConfigDomainDiskDriver *driver,
                                               GVirConfigDomainDiskFormat format);
GVirConfigDomainDiskFormat gvir_config_domain_disk_driver_get_format(GVirConfigDomainDiskDriver *driver);

void gvir_config_domain_disk_driver_set_io_policy(GVirConfigDomainDiskDriver *driver,
                                                  GVirConfigDomainDiskDriverIoPolicy policy);
GVirConfigDomainDiskDriverIoPolicy gvir_config_domain_disk_driver_get_io_policy(GVirConfigDomainDiskDriver *driver);

void gvir_config_domain_disk_driver_set_copy_on_read(GVirConfigDomainDiskDriver *driver,
                                                     gboolean copy_on_read);
gboolean gvir_config_domain_disk_driver_get_copy_on_read(GVirConfigDomainDiskDriver *driver);

void gvir_config_domain_disk_driver_set_discard(GVirConfigDomainDiskDriver *driver,
                                                GVirConfigDomainDiskDriverDiscard discard);
GVirConfigDomainDiskDriverDiscard gvir_config_domain_disk_driver_get_discard(GVirConfigDomainDiskDriver *driver);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_DISK_DRIVER_H__ */
