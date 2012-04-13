/*
 * libvirt-gconfig-domain-clock.c: libvirt domain clock configuration
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_CLOCK_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_CLOCK, GVirConfigDomainClockPrivate))

struct _GVirConfigDomainClockPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomainClock, gvir_config_domain_clock, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_clock_class_init(GVirConfigDomainClockClass *klass)
{
    g_type_class_add_private(klass, sizeof(GVirConfigDomainClockPrivate));
}


static void gvir_config_domain_clock_init(GVirConfigDomainClock *klock)
{
    g_debug("Init GVirConfigDomainClock=%p", klock);

    klock->priv = GVIR_CONFIG_DOMAIN_CLOCK_GET_PRIVATE(klock);
}


GVirConfigDomainClock *gvir_config_domain_clock_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_CLOCK,
                                    "clock", NULL);
    return GVIR_CONFIG_DOMAIN_CLOCK(object);
}

GVirConfigDomainClock *gvir_config_domain_clock_new_from_xml(const gchar *xml,
                                                GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_CLOCK,
                                             "clock", NULL, xml, error);
    return GVIR_CONFIG_DOMAIN_CLOCK(object);
}

void gvir_config_domain_clock_set_offset(GVirConfigDomainClock *klock,
                                         GVirConfigDomainClockOffset offset)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CLOCK(klock));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(klock),
                                               "offset",
                                               GVIR_CONFIG_TYPE_DOMAIN_CLOCK_OFFSET,
                                               offset,
                                               NULL);
}

void gvir_config_domain_clock_set_timezone(GVirConfigDomainClock *klock,
                                           const char *tz)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CLOCK(klock));
    g_return_if_fail(tz != NULL);

    gvir_config_object_set_attribute(GVIR_CONFIG_OBJECT(klock),
                                     "timezone", tz, NULL);
}

void gvir_config_domain_clock_set_variable_offset(GVirConfigDomainClock *klock,
                                                  gint seconds)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CLOCK(klock));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(klock),
                                               "offset", G_TYPE_STRING,  "variable",
                                               "adjustment", G_TYPE_INT, seconds,
                                               NULL);
}

void gvir_config_domain_clock_add_timer(GVirConfigDomainClock *klock,
                                        GVirConfigDomainTimer *timer)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_CLOCK(klock));
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_TIMER(timer));

    gvir_config_object_attach_add(GVIR_CONFIG_OBJECT(klock),
                                  GVIR_CONFIG_OBJECT(timer));
}
