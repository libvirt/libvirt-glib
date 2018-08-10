/*
 * libvirt-gconfig-domain-graphics-sdl.c: libvirt domain SDL configuration
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

#define GVIR_CONFIG_DOMAIN_GRAPHICS_SDL_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SDL, GVirConfigDomainGraphicsSdlPrivate))

struct _GVirConfigDomainGraphicsSdlPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainGraphicsSdl, gvir_config_domain_graphics_sdl, GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS);


static void gvir_config_domain_graphics_sdl_class_init(GVirConfigDomainGraphicsSdlClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_graphics_sdl_init(GVirConfigDomainGraphicsSdl *graphics_sdl)
{
    graphics_sdl->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_SDL_GET_PRIVATE(graphics_sdl);
}


GVirConfigDomainGraphicsSdl *gvir_config_domain_graphics_sdl_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SDL,
                                    "graphics", NULL);
    gvir_config_object_set_attribute(object, "type", "sdl", NULL);
    return GVIR_CONFIG_DOMAIN_GRAPHICS_SDL(object);
}

GVirConfigDomainGraphicsSdl *
gvir_config_domain_graphics_sdl_new_from_xml(const gchar *xml,
                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SDL,
                                             "graphics", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "sdl") != 0) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
    return GVIR_CONFIG_DOMAIN_GRAPHICS_SDL(object);
}

void gvir_config_domain_graphics_sdl_set_xauthority(GVirConfigDomainGraphicsSdl *graphics,
                                                    const gchar *path)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SDL(graphics));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(graphics),
                                     "xauth", path,
                                     NULL);
}

void gvir_config_domain_graphics_sdl_set_display(GVirConfigDomainGraphicsSdl *graphics,
                                                 const gchar *disp)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SDL(graphics));

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(graphics),
                                     "display", disp,
                                     NULL);
}

void gvir_config_domain_graphics_sdl_set_fullscreen(GVirConfigDomainGraphicsSdl *graphics,
                                                    gboolean fullscreen)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SDL(graphics));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(graphics),
                                               "fullscreen",
                                               G_TYPE_BOOLEAN,
                                               fullscreen,
                                               NULL);
}
