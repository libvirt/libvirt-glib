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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Authors: Christophe Fergeau <cfergeau@gmail.com>
 *          Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
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

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainGraphicsSpice, gvir_config_domain_graphics_spice, GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS);


static void gvir_config_domain_graphics_spice_class_init(GVirConfigDomainGraphicsSpiceClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_graphics_spice_init(GVirConfigDomainGraphicsSpice *graphics_spice)
{
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
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "spice") != 0) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
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

int gvir_config_domain_graphics_spice_get_port(GVirConfigDomainGraphicsSpice *graphics)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SPICE(graphics), 0);

    return gvir_config_object_get_attribute_uint64(GVIR_CONFIG_OBJECT(graphics),
                                                   NULL, "port", 0);
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

/**
 * gvir_config_domain_graphics_spice_get_image_compression:
 * @graphics: a #GVirConfigDomainGraphicsSpice
 *
 * Returns: (type GVirConfigDomainGraphicsSpiceImageCompression): image
 * compression configuration of @graphics
 */
int
gvir_config_domain_graphics_spice_get_image_compression(GVirConfigDomainGraphicsSpice *graphics)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SPICE(graphics),
                         GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE_IMAGE_COMPRESSION_OFF);

    return gvir_config_object_get_attribute_genum
        (GVIR_CONFIG_OBJECT(graphics),
         "image",
         "compression",
         GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE_IMAGE_COMPRESSION,
         GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE_IMAGE_COMPRESSION_GLZ);
}

void gvir_config_domain_graphics_spice_set_image_compression
    (GVirConfigDomainGraphicsSpice *graphics,
     GVirConfigDomainGraphicsSpiceImageCompression compression)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SPICE(graphics));

    gvir_config_object_replace_child_with_attribute_enum
       (GVIR_CONFIG_OBJECT(graphics),
        "image",
        "compression",
        GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SPICE_IMAGE_COMPRESSION,
        compression);
}

void gvir_config_domain_graphics_spice_set_gl(GVirConfigDomainGraphicsSpice *graphics,
                                              gboolean gl)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SPICE(graphics));

    gvir_config_object_replace_child_with_attribute_enum
      (GVIR_CONFIG_OBJECT(graphics), "gl", "enable", G_TYPE_BOOLEAN, gl);
}
