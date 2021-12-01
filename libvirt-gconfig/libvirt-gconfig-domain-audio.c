/*
 * libvirt-gconfig-domain-audio.c: libvirt domain audio configuration
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_AUDIO_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_AUDIO, GVirConfigDomainAudioPrivate))

struct _GVirConfigDomainAudioPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainAudio, gvir_config_domain_audio, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_audio_class_init(GVirConfigDomainAudioClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_audio_init(GVirConfigDomainAudio *audio)
{
    audio->priv = GVIR_CONFIG_DOMAIN_AUDIO_GET_PRIVATE(audio);
}


GVirConfigDomainAudio *gvir_config_domain_audio_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_AUDIO,
                                    "audio", NULL);
    return GVIR_CONFIG_DOMAIN_AUDIO(object);
}

GVirConfigDomainAudio *gvir_config_domain_audio_new_from_xml(const gchar *xml,
                                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_AUDIO,
                                             "audio", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_AUDIO(object);
}

void gvir_config_domain_audio_set_backend(GVirConfigDomainAudio *audio,
                                          GVirConfigDomainAudioBackend backend)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_AUDIO(audio));
    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(audio), "type",
                                               GVIR_CONFIG_TYPE_DOMAIN_AUDIO_BACKEND,
                                               backend,
                                               NULL);
}

void gvir_config_domain_audio_set_server_name(GVirConfigDomainAudio *audio,
                                              const gchar *server_name)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_AUDIO(audio));
    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(audio), "serverName",
                                               G_TYPE_STRING,
                                               server_name,
                                               NULL);

}
