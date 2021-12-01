/*
 * libvirt-gconfig-domain-audio.h: libvirt domain audio configuration
 *
 * Copyright (C) 2021 Red Hat, Inc.
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
 * Author: Felipe Borges <feborges@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_AUDIO            (gvir_config_domain_audio_get_type ())
#define GVIR_CONFIG_DOMAIN_AUDIO(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_AUDIO, GVirConfigDomainAudio))
#define GVIR_CONFIG_DOMAIN_AUDIO_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_AUDIO, GVirConfigDomainAudioClass))
#define GVIR_CONFIG_IS_DOMAIN_AUDIO(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_AUDIO))
#define GVIR_CONFIG_IS_DOMAIN_AUDIO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_AUDIO))
#define GVIR_CONFIG_DOMAIN_AUDIO_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_AUDIO, GVirConfigDomainAudioClass))

typedef struct _GVirConfigDomainAudio GVirConfigDomainAudio;
typedef struct _GVirConfigDomainAudioPrivate GVirConfigDomainAudioPrivate;
typedef struct _GVirConfigDomainAudioClass GVirConfigDomainAudioClass;

struct _GVirConfigDomainAudio
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainAudioPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainAudioClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_AUDIO_BACKEND_NONE,
    GVIR_CONFIG_DOMAIN_AUDIO_BACKEND_ALSA,
    GVIR_CONFIG_DOMAIN_AUDIO_BACKEND_COREAUDIO,
    GVIR_CONFIG_DOMAIN_AUDIO_BACKEND_JACK,
    GVIR_CONFIG_DOMAIN_AUDIO_BACKEND_OSS,
    GVIR_CONFIG_DOMAIN_AUDIO_BACKEND_PULSEAUDIO,
    GVIR_CONFIG_DOMAIN_AUDIO_BACKEND_SDL,
    GVIR_CONFIG_DOMAIN_AUDIO_BACKEND_SPICE,
    GVIR_CONFIG_DOMAIN_AUDIO_BACKEND_FILE
} GVirConfigDomainAudioBackend;

GType gvir_config_domain_audio_get_type(void);

GVirConfigDomainAudio *gvir_config_domain_audio_new(void);
GVirConfigDomainAudio *gvir_config_domain_audio_new_from_xml(const gchar *xml,
                                                             GError **error);
void gvir_config_domain_audio_set_backend(GVirConfigDomainAudio *audio,
                                          GVirConfigDomainAudioBackend backend);
void gvir_config_domain_audio_set_server_name(GVirConfigDomainAudio *audio,
                                              const gchar *server_name);

G_END_DECLS
