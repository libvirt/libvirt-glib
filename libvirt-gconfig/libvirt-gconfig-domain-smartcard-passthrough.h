/*
 * libvirt-gconfig-domain-smartcard-passthrough.h: libvirt domain smartcard passthrough configuration
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

#define GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_PASSTHROUGH            (gvir_config_domain_smartcard_passthrough_get_type ())
#define GVIR_CONFIG_DOMAIN_SMARTCARD_PASSTHROUGH(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_PASSTHROUGH, GVirConfigDomainSmartcardPassthrough))
#define GVIR_CONFIG_DOMAIN_SMARTCARD_PASSTHROUGH_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_PASSTHROUGH, GVirConfigDomainSmartcardPassthroughClass))
#define GVIR_CONFIG_IS_DOMAIN_SMARTCARD_PASSTHROUGH(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_PASSTHROUGH))
#define GVIR_CONFIG_IS_DOMAIN_SMARTCARD_PASSTHROUGH_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_PASSTHROUGH))
#define GVIR_CONFIG_DOMAIN_SMARTCARD_PASSTHROUGH_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_PASSTHROUGH, GVirConfigDomainSmartcardPassthroughClass))

typedef struct _GVirConfigDomainSmartcardPassthrough GVirConfigDomainSmartcardPassthrough;
typedef struct _GVirConfigDomainSmartcardPassthroughPrivate GVirConfigDomainSmartcardPassthroughPrivate;
typedef struct _GVirConfigDomainSmartcardPassthroughClass GVirConfigDomainSmartcardPassthroughClass;

struct _GVirConfigDomainSmartcardPassthrough
{
    GVirConfigDomainSmartcard parent;

    GVirConfigDomainSmartcardPassthroughPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainSmartcardPassthroughClass
{
    GVirConfigDomainSmartcardClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_smartcard_passthrough_get_type(void);

GVirConfigDomainSmartcardPassthrough *gvir_config_domain_smartcard_passthrough_new(void);
GVirConfigDomainSmartcardPassthrough *gvir_config_domain_smartcard_passthrough_new_from_xml(const gchar *xml,
                                                                                            GError **error);
void gvir_config_domain_smartcard_passthrough_set_source(GVirConfigDomainSmartcardPassthrough *smartcard,
                                                         GVirConfigDomainChardevSource *source);


G_END_DECLS
