/*
 * libvirt-gconfig-domain-console.c: libvirt domain console configuration
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_CONSOLE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CONSOLE, GVirConfigDomainConsolePrivate))

struct _GVirConfigDomainConsolePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainConsole, gvir_config_domain_console, GVIR_CONFIG_TYPE_DOMAIN_CHARDEV);


static void gvir_config_domain_console_class_init(GVirConfigDomainConsoleClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_console_init(GVirConfigDomainConsole *console)
{
    console->priv = GVIR_CONFIG_DOMAIN_CONSOLE_GET_PRIVATE(console);
}

GVirConfigDomainConsole *gvir_config_domain_console_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CONSOLE,
                                    "console", NULL);
    return GVIR_CONFIG_DOMAIN_CONSOLE(object);
}

GVirConfigDomainConsole *gvir_config_domain_console_new_from_xml(const gchar *xml,
                                                                 GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CONSOLE,
                                             "console", NULL, xml, error);
    if (object == NULL)
        return NULL;
    return GVIR_CONFIG_DOMAIN_CONSOLE(object);
}

void gvir_config_domain_console_set_target_type(GVirConfigDomainConsole *console,
                                                GVirConfigDomainConsoleTargetType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CONSOLE(console));

    gvir_config_object_add_child_with_attribute_enum(GVIR_CONFIG_OBJECT(console),
                                                     "target", "type",
                                                     GVIR_CONFIG_TYPE_DOMAIN_CONSOLE_TARGET_TYPE,
                                                     type);
}
