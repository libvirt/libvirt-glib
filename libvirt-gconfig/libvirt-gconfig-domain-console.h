/*
 * libvirt-gconfig-domain-console.h: libvirt domain console configuration
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
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_CONSOLE_H__
#define __LIBVIRT_GCONFIG_DOMAIN_CONSOLE_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_CONSOLE            (gvir_config_domain_console_get_type ())
#define GVIR_CONFIG_DOMAIN_CONSOLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_CONSOLE, GVirConfigDomainConsole))
#define GVIR_CONFIG_DOMAIN_CONSOLE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_CONSOLE, GVirConfigDomainConsoleClass))
#define GVIR_CONFIG_IS_DOMAIN_CONSOLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_CONSOLE))
#define GVIR_CONFIG_IS_DOMAIN_CONSOLE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_CONSOLE))
#define GVIR_CONFIG_DOMAIN_CONSOLE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_CONSOLE, GVirConfigDomainConsoleClass))

typedef struct _GVirConfigDomainConsole GVirConfigDomainConsole;
typedef struct _GVirConfigDomainConsolePrivate GVirConfigDomainConsolePrivate;
typedef struct _GVirConfigDomainConsoleClass GVirConfigDomainConsoleClass;

struct _GVirConfigDomainConsole
{
    GVirConfigDomainChardev parent;

    GVirConfigDomainConsolePrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainConsoleClass
{
    GVirConfigDomainChardevClass parent_class;

    gpointer padding[20];
};


typedef enum {
    GVIR_CONFIG_DOMAIN_CONSOLE_TARGET_XEN,
    GVIR_CONFIG_DOMAIN_CONSOLE_TARGET_SERIAL,
    GVIR_CONFIG_DOMAIN_CONSOLE_TARGET_UML,
    GVIR_CONFIG_DOMAIN_CONSOLE_TARGET_VIRTIO,
    GVIR_CONFIG_DOMAIN_CONSOLE_TARGET_LXC,
    GVIR_CONFIG_DOMAIN_CONSOLE_TARGET_OPENVZ,
} GVirConfigDomainConsoleTargetType;

GType gvir_config_domain_console_get_type(void);
GVirConfigDomainConsole *gvir_config_domain_console_new(void);
GVirConfigDomainConsole *gvir_config_domain_console_new_from_xml(const gchar *xml,
                                                                 GError **error);

void gvir_config_domain_console_set_target_type(GVirConfigDomainConsole *console,
                                                GVirConfigDomainConsoleTargetType type);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_CONSOLE_H__ */
