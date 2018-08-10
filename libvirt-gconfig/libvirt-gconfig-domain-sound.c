/*
 * libvirt-gconfig-domain-sound.c: libvirt domain sound configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_SOUND_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_SOUND, GVirConfigDomainSoundPrivate))

struct _GVirConfigDomainSoundPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainSound, gvir_config_domain_sound, GVIR_CONFIG_TYPE_DOMAIN_DEVICE);


static void gvir_config_domain_sound_class_init(GVirConfigDomainSoundClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_sound_init(GVirConfigDomainSound *sound)
{
    sound->priv = GVIR_CONFIG_DOMAIN_SOUND_GET_PRIVATE(sound);
}


GVirConfigDomainSound *gvir_config_domain_sound_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_SOUND,
                                    "sound", NULL);
    return GVIR_CONFIG_DOMAIN_SOUND(object);
}

GVirConfigDomainSound *gvir_config_domain_sound_new_from_xml(const gchar *xml,
                                                             GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_SOUND,
                                             "sound", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_SOUND(object);
}

void gvir_config_domain_sound_set_model(GVirConfigDomainSound *sound,
                                        GVirConfigDomainSoundModel model)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SOUND(sound));
    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(sound), "model",
                                               GVIR_CONFIG_TYPE_DOMAIN_SOUND_MODEL,
                                               model,
                                               NULL);
}
