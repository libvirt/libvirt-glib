/*
 * libvirt-gconfig-domain-video.h: libvirt domain video configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_VIDEO            (gvir_config_domain_video_get_type ())
#define GVIR_CONFIG_DOMAIN_VIDEO(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_VIDEO, GVirConfigDomainVideo))
#define GVIR_CONFIG_DOMAIN_VIDEO_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_VIDEO, GVirConfigDomainVideoClass))
#define GVIR_CONFIG_IS_DOMAIN_VIDEO(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_VIDEO))
#define GVIR_CONFIG_IS_DOMAIN_VIDEO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_VIDEO))
#define GVIR_CONFIG_DOMAIN_VIDEO_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_VIDEO, GVirConfigDomainVideoClass))

typedef struct _GVirConfigDomainVideo GVirConfigDomainVideo;
typedef struct _GVirConfigDomainVideoPrivate GVirConfigDomainVideoPrivate;
typedef struct _GVirConfigDomainVideoClass GVirConfigDomainVideoClass;

struct _GVirConfigDomainVideo
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainVideoPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainVideoClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_VIDEO_MODEL_VGA,
    GVIR_CONFIG_DOMAIN_VIDEO_MODEL_CIRRUS,
    GVIR_CONFIG_DOMAIN_VIDEO_MODEL_VMVGA,
    GVIR_CONFIG_DOMAIN_VIDEO_MODEL_XEN,
    GVIR_CONFIG_DOMAIN_VIDEO_MODEL_VBOX,
    GVIR_CONFIG_DOMAIN_VIDEO_MODEL_QXL,
    GVIR_CONFIG_DOMAIN_VIDEO_MODEL_VIRTIO,
    GVIR_CONFIG_DOMAIN_VIDEO_MODEL_BOCHS,
} GVirConfigDomainVideoModel;

GType gvir_config_domain_video_get_type(void);

GVirConfigDomainVideo *gvir_config_domain_video_new(void);
GVirConfigDomainVideo *gvir_config_domain_video_new_from_xml(const gchar *xml,
                                                             GError **error);
GVirConfigDomainVideoModel gvir_config_domain_video_get_model(GVirConfigDomainVideo *video);
void gvir_config_domain_video_set_model(GVirConfigDomainVideo *video,
                                        GVirConfigDomainVideoModel model);
void gvir_config_domain_video_set_vram(GVirConfigDomainVideo *video,
                                       guint kbytes);

void gvir_config_domain_video_set_ram(GVirConfigDomainVideo *video,
                                       guint kbytes);
void gvir_config_domain_video_set_vgamem(GVirConfigDomainVideo *video,
                                       guint kbytes);
void gvir_config_domain_video_set_heads(GVirConfigDomainVideo *video,
                                        guint head_count);
void gvir_config_domain_video_set_accel3d(GVirConfigDomainVideo *video,
                                          gboolean accel3d);

G_END_DECLS
