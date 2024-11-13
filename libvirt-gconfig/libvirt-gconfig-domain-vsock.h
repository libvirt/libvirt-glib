/*
 * libvirt-gconfig-domain-vsock.h: libvirt domain Vsock configuration
 *
 * Copyright (C) 2024 Red Hat, Inc.
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
 * Author: Felipe Borges <felipeborges@gnome.org>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#pragma once

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_VSOCK            (gvir_config_domain_vsock_get_type ())
#define GVIR_CONFIG_DOMAIN_VSOCK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_VSOCK, GVirConfigDomainVsock))
#define GVIR_CONFIG_DOMAIN_VSOCK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_VSOCK, GVirConfigDomainVsockClass))
#define GVIR_CONFIG_IS_DOMAIN_VSOCK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_VSOCK))
#define GVIR_CONFIG_IS_DOMAIN_VSOCK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_VSOCK))
#define GVIR_CONFIG_DOMAIN_VSOCK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_VSOCK, GVirConfigDomainVsockClass))

typedef struct _GVirConfigDomainVsock GVirConfigDomainVsock;
typedef struct _GVirConfigDomainVsockPrivate GVirConfigDomainVsockPrivate;
typedef struct _GVirConfigDomainVsockClass GVirConfigDomainVsockClass;

struct _GVirConfigDomainVsock
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainVsockPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainVsockClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_vsock_get_type(void);

GVirConfigDomainVsock *gvir_config_domain_vsock_new(void);

void gvir_config_domain_vsock_set_cid_address(GVirConfigDomainVsock *vsock,
                                              unsigned int address);

G_END_DECLS
