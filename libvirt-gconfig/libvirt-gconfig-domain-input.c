/*
 * libvirt-gconfig-device-input.c: libvirt domain input configuration
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
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_INPUT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_INPUT, GVirConfigDomainInputPrivate))

struct _GVirConfigDomainInputPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainInput, gvir_config_domain_input, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_input_class_init(GVirConfigDomainInputClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_input_init(GVirConfigDomainInput *device_input)
{
    device_input->priv = GVIR_CONFIG_DOMAIN_INPUT_GET_PRIVATE(device_input);
}


GVirConfigDomainInput *gvir_config_domain_input_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_INPUT,
                                    "input", NULL);
    return GVIR_CONFIG_DOMAIN_INPUT(object);
}

GVirConfigDomainInput *gvir_config_domain_input_new_from_xml(const gchar *xml,
                                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_INPUT,
                                             "input", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_INPUT(object);
}


GVirConfigDomainInputDeviceType gvir_config_domain_input_get_device_type(GVirConfigDomainInput *input)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_INPUT(input),
                         GVIR_CONFIG_DOMAIN_INPUT_DEVICE_MOUSE);
    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(input),
                                                  NULL, "type",
                                                  GVIR_CONFIG_TYPE_DOMAIN_INPUT_DEVICE_TYPE,
                                                  GVIR_CONFIG_DOMAIN_INPUT_DEVICE_MOUSE);
}


void gvir_config_domain_input_set_device_type(GVirConfigDomainInput *input,
                                              GVirConfigDomainInputDeviceType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INPUT(input));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(input), "type",
                                               GVIR_CONFIG_TYPE_DOMAIN_INPUT_DEVICE_TYPE,
                                               type, NULL);
}


GVirConfigDomainInputBus gvir_config_domain_input_get_bus(GVirConfigDomainInput *input)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_INPUT(input),
                         GVIR_CONFIG_DOMAIN_INPUT_BUS_PS2);
    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(input),
                                                  NULL, "bus",
                                                  GVIR_CONFIG_TYPE_DOMAIN_INPUT_BUS,
                                                  GVIR_CONFIG_DOMAIN_INPUT_BUS_PS2);
}


void gvir_config_domain_input_set_bus(GVirConfigDomainInput *input,
                                      GVirConfigDomainInputBus bus)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_INPUT(input));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(input), "bus",
                                               GVIR_CONFIG_TYPE_DOMAIN_INPUT_BUS,
                                               bus, NULL);
}
