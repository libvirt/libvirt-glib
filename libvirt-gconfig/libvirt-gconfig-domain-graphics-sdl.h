/*
 * libvirt-gconfig-domain-graphics-sdl.h: libvirt domain SDL configuration
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

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SDL            (gvir_config_domain_graphics_sdl_get_type ())
#define GVIR_CONFIG_DOMAIN_GRAPHICS_SDL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SDL, GVirConfigDomainGraphicsSdl))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_SDL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SDL, GVirConfigDomainGraphicsSdlClass))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SDL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SDL))
#define GVIR_CONFIG_IS_DOMAIN_GRAPHICS_SDL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SDL))
#define GVIR_CONFIG_DOMAIN_GRAPHICS_SDL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_GRAPHICS_SDL, GVirConfigDomainGraphicsSdlClass))

typedef struct _GVirConfigDomainGraphicsSdl GVirConfigDomainGraphicsSdl;
typedef struct _GVirConfigDomainGraphicsSdlPrivate GVirConfigDomainGraphicsSdlPrivate;
typedef struct _GVirConfigDomainGraphicsSdlClass GVirConfigDomainGraphicsSdlClass;

struct _GVirConfigDomainGraphicsSdl
{
    GVirConfigDomainGraphics parent;

    GVirConfigDomainGraphicsSdlPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainGraphicsSdlClass
{
    GVirConfigDomainGraphicsClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_graphics_sdl_get_type(void);

GVirConfigDomainGraphicsSdl *gvir_config_domain_graphics_sdl_new(void);
GVirConfigDomainGraphicsSdl *gvir_config_domain_graphics_sdl_new_from_xml(const gchar *xml,
                                                                          GError **error);
void gvir_config_domain_graphics_sdl_set_xauthority(GVirConfigDomainGraphicsSdl *graphics,
                                                    const gchar *path);
void gvir_config_domain_graphics_sdl_set_display(GVirConfigDomainGraphicsSdl *graphics,
                                                 const gchar *disp);
void gvir_config_domain_graphics_sdl_set_fullscreen(GVirConfigDomainGraphicsSdl *graphics,
                                                    gboolean fullscreen);

G_END_DECLS
