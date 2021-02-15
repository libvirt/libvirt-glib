/*
 * libvirt-gconfig-domain-smartcard.h: libvirt domain smartcard configuration
 *
 * Copyright (C) 2012 Red Hat, Inc.
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
 * Author: Alexander Larsson <alexl@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD            (gvir_config_domain_smartcard_get_type ())
#define GVIR_CONFIG_DOMAIN_SMARTCARD(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD, GVirConfigDomainSmartcard))
#define GVIR_CONFIG_DOMAIN_SMARTCARD_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD, GVirConfigDomainSmartcardClass))
#define GVIR_CONFIG_IS_DOMAIN_SMARTCARD(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD))
#define GVIR_CONFIG_IS_DOMAIN_SMARTCARD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD))
#define GVIR_CONFIG_DOMAIN_SMARTCARD_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD, GVirConfigDomainSmartcardClass))

typedef struct _GVirConfigDomainSmartcard GVirConfigDomainSmartcard;
typedef struct _GVirConfigDomainSmartcardPrivate GVirConfigDomainSmartcardPrivate;
typedef struct _GVirConfigDomainSmartcardClass GVirConfigDomainSmartcardClass;

struct _GVirConfigDomainSmartcard
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainSmartcardPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainSmartcardClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};


GType gvir_config_domain_smartcard_get_type(void);

void gvir_config_domain_smartcard_set_address(GVirConfigDomainRedirdev *redirdev,
                                              GVirConfigDomainAddress *address);

G_END_DECLS
