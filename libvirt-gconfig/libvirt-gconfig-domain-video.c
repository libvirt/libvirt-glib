/*
 * libvirt-gconfig-domain-video.c: libvirt domain video configuration
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

#define GVIR_CONFIG_DOMAIN_VIDEO_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_VIDEO, GVirConfigDomainVideoPrivate))

struct _GVirConfigDomainVideoPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainVideo, gvir_config_domain_video, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_video_class_init(GVirConfigDomainVideoClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainVideoPrivate));
}


static void gvir_config_domain_video_init(GVirConfigDomainVideo *video)
{
    g_debug("Init GVirConfigDomainVideo=%p", video);

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
    xmlNodePtr node;
    char *vram_str;

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(video));
    if (node == NULL)
        return;
    node = gvir_config_xml_get_element(node, "model", NULL);
    if (node == NULL)
        return;
    vram_str = g_strdup_printf("%u", kbytes);
    xmlNewProp(node, (xmlChar*)"vram", (xmlChar*)vram_str);
    g_free(vram_str);
}

void gvir_config_domain_video_set_heads(GVirConfigDomainVideo *video,
                                        guint head_count)
{
    xmlNodePtr node;
    char *heads_str;

    node = gvir_config_object_get_xml_node(GVIR_CONFIG_OBJECT(video));
    if (node == NULL)
        return;
    node = gvir_config_xml_get_element(node, "model", NULL);
    if (node == NULL)
        return;
    heads_str = g_strdup_printf("%u", head_count);
    xmlNewProp(node, (xmlChar*)"heads", (xmlChar*)heads_str);
    g_free(heads_str);
}
