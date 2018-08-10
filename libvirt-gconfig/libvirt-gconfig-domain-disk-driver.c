/*
 * libvirt-gconfig-domain-disk-driver.c: libvirt domain disk driver configuration
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_DISK_DRIVER_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER, GVirConfigDomainDiskDriverPrivate))

struct _GVirConfigDomainDiskDriverPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainDiskDriver, gvir_config_domain_disk_driver, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_disk_driver_class_init(GVirConfigDomainDiskDriverClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_disk_driver_init(GVirConfigDomainDiskDriver *driver)
{
    driver->priv = GVIR_CONFIG_DOMAIN_DISK_DRIVER_GET_PRIVATE(driver);
}


GVirConfigDomainDiskDriver *gvir_config_domain_disk_driver_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER,
                                    "driver", NULL);
    return GVIR_CONFIG_DOMAIN_DISK_DRIVER(object);
}

GVirConfigDomainDiskDriver *gvir_config_domain_disk_driver_new_from_xml(const gchar *xml,
                                                                        GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER,
                                             "driver", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_DISK_DRIVER(object);
}


void gvir_config_domain_disk_driver_set_cache(GVirConfigDomainDiskDriver *driver,
                                              GVirConfigDomainDiskCacheType cache_type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(driver),
                                               "cache",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_CACHE_TYPE,
                                               cache_type,
                                               NULL);
}


GVirConfigDomainDiskCacheType gvir_config_domain_disk_driver_get_cache(GVirConfigDomainDiskDriver *driver)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver),
                         GVIR_CONFIG_DOMAIN_DISK_CACHE_DEFAULT);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(driver),
                                                  NULL, "cache",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_CACHE_TYPE,
                                                  GVIR_CONFIG_DOMAIN_DISK_CACHE_DEFAULT);
}


void gvir_config_domain_disk_driver_set_name(GVirConfigDomainDiskDriver *driver,
                                             const char *name)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(driver),
                                     "name", name, NULL);
}


const char *gvir_config_domain_disk_driver_get_name(GVirConfigDomainDiskDriver *driver)

{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver), NULL);

    return gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(driver),
                                            NULL, "name");
}


void gvir_config_domain_disk_driver_set_error_policy(GVirConfigDomainDiskDriver *driver,
                                                     GVirConfigDomainDiskDriverErrorPolicy policy)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(driver),
                                               "error_policy",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER_ERROR_POLICY,
                                               policy,
                                               NULL);
}


GVirConfigDomainDiskDriverErrorPolicy gvir_config_domain_disk_driver_get_error_policy(GVirConfigDomainDiskDriver *driver)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver),
                         GVIR_CONFIG_DOMAIN_DISK_DRIVER_ERROR_POLICY_REPORT);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(driver),
                                                  NULL, "error_policy",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER_ERROR_POLICY,
                                                  GVIR_CONFIG_DOMAIN_DISK_DRIVER_ERROR_POLICY_REPORT);
}


void gvir_config_domain_disk_driver_set_format(GVirConfigDomainDiskDriver *driver,
                                               GVirConfigDomainDiskFormat format)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(driver),
                                               "type",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_FORMAT,
                                               format,
                                               NULL);
}


GVirConfigDomainDiskFormat gvir_config_domain_disk_driver_get_format(GVirConfigDomainDiskDriver *driver)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver),
                         GVIR_CONFIG_DOMAIN_DISK_FORMAT_RAW);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(driver),
                                                  NULL, "type",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_FORMAT,
                                                  GVIR_CONFIG_DOMAIN_DISK_FORMAT_RAW);
}


void gvir_config_domain_disk_driver_set_io_policy(GVirConfigDomainDiskDriver *driver,
                                                  GVirConfigDomainDiskDriverIoPolicy policy)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(driver),
                                               "io",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER_IO_POLICY,
                                               policy,
                                               NULL);
}


GVirConfigDomainDiskDriverIoPolicy gvir_config_domain_disk_driver_get_io_policy(GVirConfigDomainDiskDriver *driver)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver),
                         GVIR_CONFIG_DOMAIN_DISK_DRIVER_IO_POLICY_THREADS);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(driver),
                                                  NULL, "io",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER_IO_POLICY,
                                                  GVIR_CONFIG_DOMAIN_DISK_DRIVER_IO_POLICY_THREADS);
}


void gvir_config_domain_disk_driver_set_copy_on_read(GVirConfigDomainDiskDriver *driver,
                                                     gboolean copy_on_read)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver));

    /* gvir_config_object_set_attribute_with_type(..., G_TYPE_BOOLEAN, ...)
     * cannot be used here as it translates into "yes"/"no", but what we
     * want is "on"/"off"
     */
    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(driver),
                                     "copy_on_read",
                                     copy_on_read?"on":"off",
                                     NULL);
}


gboolean gvir_config_domain_disk_driver_get_copy_on_read(GVirConfigDomainDiskDriver *driver)
{
    const char *copy_on_read;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver), FALSE);

    copy_on_read = gvir_config_object_get_attribute(GVIR_CONFIG_OBJECT(driver),
                                                    NULL, "copy_on_read");

    return (g_strcmp0(copy_on_read, "on") == 0);
}


void gvir_config_domain_disk_driver_set_discard(GVirConfigDomainDiskDriver *driver,
                                                GVirConfigDomainDiskDriverDiscard discard)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(driver),
                                               "discard",
                                               GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER_DISCARD,
                                               discard,
                                               NULL);
}


GVirConfigDomainDiskDriverDiscard gvir_config_domain_disk_driver_get_discard(GVirConfigDomainDiskDriver *driver)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_DISK_DRIVER(driver),
                         GVIR_CONFIG_DOMAIN_DISK_DRIVER_DISCARD_IGNORE);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(driver),
                                                  NULL, "discard",
                                                  GVIR_CONFIG_TYPE_DOMAIN_DISK_DRIVER_DISCARD,
                                                  GVIR_CONFIG_DOMAIN_DISK_DRIVER_DISCARD_IGNORE);
}
