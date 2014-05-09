/*
 * libvirt-gconfig-domain-channel.h: libvirt domain channel configuration
 *
 * Copyright (C) 2011-2012 Red Hat, Inc.
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

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CHANNEL_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CHANNEL_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CHANNEL            (gvir_config_domain_channel_get_type ())
#define GVIR_CONFIG_DOMAIN_CHANNEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHANNEL, GVirConfigDomainChannel))
#define GVIR_CONFIG_DOMAIN_CHANNEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CHANNEL, GVirConfigDomainChannelClass))
#define GVIR_CONFIG_IS_DOMAIN_CHANNEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHANNEL))
#define GVIR_CONFIG_IS_DOMAIN_CHANNEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CHANNEL))
#define GVIR_CONFIG_DOMAIN_CHANNEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CHANNEL, GVirConfigDomainChannelClass))

typedef struct _GVirConfigDomainChannel GVirConfigDomainChannel;
typedef struct _GVirConfigDomainChannelPrivate GVirConfigDomainChannelPrivate;
typedef struct _GVirConfigDomainChannelClass GVirConfigDomainChannelClass;

struct _GVirConfigDomainChannel
{
    GVirConfigDomainChardev parent;

    GVirConfigDomainChannelPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainChannelClass
{
    GVirConfigDomainChardevClass parent_class;

    gpointer padding[20];
};

typedef enum {
    GVIR_CONFIG_DOMAIN_CHANNEL_TARGET_GUESTFWD,
    GVIR_CONFIG_DOMAIN_CHANNEL_TARGET_VIRTIO,
    GVIR_CONFIG_DOMAIN_CHANNEL_TARGET_SPICEPORT,
} GVirConfigDomainChannelTargetType;


GType gvir_config_domain_channel_get_type(void);
GVirConfigDomainChannel *gvir_config_domain_channel_new(void);
GVirConfigDomainChannel *gvir_config_domain_channel_new_from_xml(const gchar *xml,
                                                                 GError **error);

GVirConfigDomainChannelTargetType gvir_config_domain_channel_get_target_type(GVirConfigDomainChannel *channel);
void gvir_config_domain_channel_set_target_type(GVirConfigDomainChannel *channel,
                                                GVirConfigDomainChannelTargetType type);

const gchar *gvir_config_domain_channel_get_target_name(GVirConfigDomainChannel *channel);
void gvir_config_domain_channel_set_target_name(GVirConfigDomainChannel *channel,
                                                const gchar *name);


G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CHANNEL_H__ */
