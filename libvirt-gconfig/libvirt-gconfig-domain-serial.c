/*
 * libvirt-gconfig-domain-serial.c: libvirt domain serial configuration
 *
 * Copyright (C) 2011-2012 Red Hat, Inc.
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

#define GVIR_CONFIG_DOMAIN_SERIAL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_SERIAL, GVirConfigDomainSerialPrivate))

struct _GVirConfigDomainSerialPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainSerial, gvir_config_domain_serial, GVIR_CONFIG_TYPE_DOMAIN_CHARDEV);


static void gvir_config_domain_serial_class_init(GVirConfigDomainSerialClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_serial_init(GVirConfigDomainSerial *serial)
{
    serial->priv = GVIR_CONFIG_DOMAIN_SERIAL_GET_PRIVATE(serial);
}

GVirConfigDomainSerial *gvir_config_domain_serial_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_SERIAL,
                                    "serial", NULL);
    return GVIR_CONFIG_DOMAIN_SERIAL(object);
}

GVirConfigDomainSerial *gvir_config_domain_serial_new_from_xml(const gchar *xml,
                                                                 GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_SERIAL,
                                             "serial", NULL, xml, error);
    if (object == NULL)
        return NULL;
    return GVIR_CONFIG_DOMAIN_SERIAL(object);
}
