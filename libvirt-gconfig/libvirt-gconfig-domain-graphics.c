/*
 * libvirt-gobject-config-domain-graphics.c: libvirt glib integration
 *
 * Copyright (C) 2011 Red Hat
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

#include <string.h>

#include <libxml/tree.h>

#include "libvirt-gconfig/libvirt-gconfig.h"

extern gboolean debugFlag;

#define DEBUG(fmt, ...) do { if (G_UNLIKELY(debugFlag)) g_debug(fmt, ## __VA_ARGS__); } while (0)

#define GVIR_CONFIG_DOMAIN_GRAPHICS_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_TYPE_CONFIG_DOMAIN_GRAPHICS, GVirConfigDomainGraphicsPrivate))

struct _GVirConfigDomainGraphicsPrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE(GVirConfigDomainGraphics, gvir_config_domain_graphics, GVIR_TYPE_CONFIG_DOMAIN_DEVICE);


static void gvir_config_domain_graphics_class_init(GVirConfigDomainGraphicsClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainGraphicsPrivate));
}


static void gvir_config_domain_graphics_init(GVirConfigDomainGraphics *graphics)
{
    GVirConfigDomainGraphicsPrivate *priv;

    DEBUG("Init GVirConfigDomainGraphics=%p", graphics);

    priv = graphics->priv = GVIR_CONFIG_DOMAIN_GRAPHICS_GET_PRIVATE(graphics);

    memset(priv, 0, sizeof(*priv));
}
