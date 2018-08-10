/*
 * libvirt-gconfig-domain-video.c: libvirt domain video configuration
 *
 * Copyright (C) 2011 Red Hat, Inc.
 * Copyright (C) 2015 T A Mahadevan
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

#define GVIR_CONFIG_DOMAIN_VIDEO_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_VIDEO, GVirConfigDomainVideoPrivate))

struct _GVirConfigDomainVideoPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainVideo, gvir_config_domain_video, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_video_class_init(GVirConfigDomainVideoClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_video_init(GVirConfigDomainVideo *video)
{
    video->priv = GVIR_CONFIG_DOMAIN_VIDEO_GET_PRIVATE(video);
}


GVirConfigDomainVideo *gvir_config_domain_video_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_VIDEO,
                                    "video", NULL);
    return GVIR_CONFIG_DOMAIN_VIDEO(object);
}

GVirConfigDomainVideo *gvir_config_domain_video_new_from_xml(const gchar *xml,
                                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_VIDEO,
                                             "video", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_VIDEO(object);
}

GVirConfigDomainVideoModel gvir_config_domain_video_get_model(GVirConfigDomainVideo *video)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_VIDEO(video), GVIR_CONFIG_DOMAIN_VIDEO_MODEL_VGA);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(video),
                                                  "model",
                                                  "type",
                                                  GVIR_CONFIG_TYPE_DOMAIN_VIDEO_MODEL,
                                                  GVIR_CONFIG_DOMAIN_VIDEO_MODEL_VGA);
}

void gvir_config_domain_video_set_model(GVirConfigDomainVideo *video,
                                        GVirConfigDomainVideoModel model)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_VIDEO(video));
    node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(video),
                                            "model");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute_with_type(node, "type",
                                               GVIR_CONFIG_TYPE_DOMAIN_VIDEO_MODEL,
                                               model,
                                               NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_video_set_vram(GVirConfigDomainVideo *video,
                                       guint kbytes)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_VIDEO(video));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(video), "model");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute_with_type(node, "vram",
                                               G_TYPE_UINT, kbytes,
                                               NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_video_set_ram(GVirConfigDomainVideo *video,
                                       guint kbytes)
{
    GVirConfigObject *node;
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_VIDEO(video));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(video), "model");
    g_return_if_fail(GVIR_CONFIG_OBJECT(node));
    gvir_config_object_set_attribute_with_type(node, "ram", G_TYPE_UINT,
                                               kbytes, NULL);
    g_object_unref(G_OBJECT(node));
}


void gvir_config_domain_video_set_vgamem(GVirConfigDomainVideo *video,
                                       guint kbytes)
{
    GVirConfigObject *node;
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_VIDEO(video));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(video), "model");
    g_return_if_fail(GVIR_CONFIG_OBJECT(node));
    gvir_config_object_set_attribute_with_type(node, "vgamem", G_TYPE_UINT,
                                               kbytes, NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_video_set_heads(GVirConfigDomainVideo *video,
                                        guint head_count)
{
    GVirConfigObject *node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_VIDEO(video));
    node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(video), "model");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(node));
    gvir_config_object_set_attribute_with_type(node, "heads",
                                               G_TYPE_UINT, head_count,
                                               NULL);
    g_object_unref(G_OBJECT(node));
}

void gvir_config_domain_video_set_accel3d(GVirConfigDomainVideo *video,
                                          gboolean accel3d)
{
    GVirConfigObject *model, *accel;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_VIDEO(video));
    model = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(video), "model");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(model));
    accel = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(model), "acceleration");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(accel));

    gvir_config_object_set_attribute_with_type(accel, "accel3d",
                                               G_TYPE_BOOLEAN, accel3d,
                                               NULL);
    g_object_unref(G_OBJECT(model));
    g_object_unref(G_OBJECT(accel));
}
