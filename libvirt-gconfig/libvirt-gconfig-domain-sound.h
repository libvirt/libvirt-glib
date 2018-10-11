/*
 * libvirt-gconfig-domain-sound.h: libvirt domain sound configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_SOUND_H__
#define __LIBVIRT_GCONFIG_DOMAIN_SOUND_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_SOUND            (gvir_config_domain_sound_get_type ())
#define GVIR_CONFIG_DOMAIN_SOUND(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_SOUND, GVirConfigDomainSound))
#define GVIR_CONFIG_DOMAIN_SOUND_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_SOUND, GVirConfigDomainSoundClass))
#define GVIR_CONFIG_IS_DOMAIN_SOUND(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_SOUND))
#define GVIR_CONFIG_IS_DOMAIN_SOUND_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_SOUND))
#define GVIR_CONFIG_DOMAIN_SOUND_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_SOUND, GVirConfigDomainSoundClass))

typedef struct _GVirConfigDomainSound GVirConfigDomainSound;
typedef struct _GVirConfigDomainSoundPrivate GVirConfigDomainSoundPrivate;
typedef struct _GVirConfigDomainSoundClass GVirConfigDomainSoundClass;

struct _GVirConfigDomainSound
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainSoundPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainSoundClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_SOUND_MODEL_SB16,
    GVIR_CONFIG_DOMAIN_SOUND_MODEL_ES1370,
    GVIR_CONFIG_DOMAIN_SOUND_MODEL_PCSPK,
    GVIR_CONFIG_DOMAIN_SOUND_MODEL_AC97,
    GVIR_CONFIG_DOMAIN_SOUND_MODEL_ICH6,
    GVIR_CONFIG_DOMAIN_SOUND_MODEL_ICH9
} GVirConfigDomainSoundModel;

GType gvir_config_domain_sound_get_type(void);

GVirConfigDomainSound *gvir_config_domain_sound_new(void);
GVirConfigDomainSound *gvir_config_domain_sound_new_from_xml(const gchar *xml,
                                                             GError **error);
void gvir_config_domain_sound_set_model(GVirConfigDomainSound *sound,
                                        GVirConfigDomainSoundModel model);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_SOUND_H__ */
