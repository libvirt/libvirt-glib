/*
 * libvirt-gconfig-domain-controller.h: libvirt domain controller configuration
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
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CONTROLLER_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CONTROLLER_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER            (gvir_config_domain_controller_get_type ())
#define GVIR_CONFIG_DOMAIN_CONTROLLER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER, GVirConfigDomainController))
#define GVIR_CONFIG_DOMAIN_CONTROLLER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER, GVirConfigDomainControllerClass))
#define GVIR_CONFIG_IS_DOMAIN_CONTROLLER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER))
#define GVIR_CONFIG_IS_DOMAIN_CONTROLLER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER))
#define GVIR_CONFIG_DOMAIN_CONTROLLER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CONTROLLER, GVirConfigDomainControllerClass))

typedef struct _GVirConfigDomainController GVirConfigDomainController;
typedef struct _GVirConfigDomainControllerPrivate GVirConfigDomainControllerPrivate;
typedef struct _GVirConfigDomainControllerClass GVirConfigDomainControllerClass;

struct _GVirConfigDomainController
{
    GVirConfigDomainDevice parent;

    GVirConfigDomainControllerPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainControllerClass
{
    GVirConfigDomainDeviceClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_controller_get_type(void);

void gvir_config_domain_controller_set_index(GVirConfigDomainController *controller,
                                             guint index);
guint gvir_config_domain_controller_get_index(GVirConfigDomainController *controller);
void gvir_config_domain_controller_set_address(GVirConfigDomainController *controller,
                                               GVirConfigDomainAddress *address);
void gvir_config_domain_controller_set_ports(GVirConfigDomainController *controller,
                                             guint ports);
guint gvir_config_domain_controller_get_ports(GVirConfigDomainController *controller);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CONTROLLER_H__ */
