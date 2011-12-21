/*
 * libvirt-gconfig-domain-graphics-spice.c: libvirt domain SPICE configuration
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
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE, GVirConfigDomainGraphicsSpicePrivate))

struct _GVirConfigDomainGraphicsSpicePrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainGraphicsSpice, gvir_config_domain_graphics_spice, GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS);


static void gvir_config_domain_graphics_spice_class_init(GVirConfigDomainGraphicsSpiceClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainGraphicsSpicePrivate));
}


static void gvir_config_domain_graphics_spice_init(GVirConfigDomainGraphicsSpice *graphics_spice)
{
    g_debug("Init GVirConfigDomainGraphicsSpice=%p", graphics_spice);

    graphics_spice->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE_GET_PRIVATE(graphics_spice);
}


GVirConfigDomainGraphicsSpice *gvir_config_domain_graphics_spice_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE,
                                    "graphics", NULL);
    gvir_config_object_set_attribute(object, "type", "spice", NULL);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE(object);
}

GVirConfigDomainGraphicsSpice *
gvir_config_domain_graphics_spice_new_from_xml(const gchar *xml,
                                               GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE,
                                             "graphics", NULL, xml, error);
    if (object == NULL)
        return NULL;
    gvir_config_object_set_attribute(object, "type", "spice", NULL);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE(object);
}

void gvir_config_domain_graphics_spice_set_autoport(GVirConfigDomainGraphicsSpice *graphics,
                                                    gboolean autoport)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SPICE(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "autoport", G_TYPE_BOOLEAN, autoport,
                                               NULL);
}

void gvir_config_domain_graphics_spice_set_password(GVirConfigDomainGraphicsSpice *graphics,
                                                    const char *password)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SPICE(graphics));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(graphics),
                                     "passwd", password,
                                     NULL);
}

void gvir_config_domain_graphics_spice_set_port(GVirConfigDomainGraphicsSpice *graphics,
                                                int port)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SPICE(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "port", G_TYPE_INT, port,
                                               NULL);
}

void gvir_config_domain_graphics_spice_set_tls_port(GVirConfigDomainGraphicsSpice *graphics,
                                                    int port)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SPICE(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "tlsPort", G_TYPE_INT, port,
                                               NULL);
}
