/*
 * libvirt-gconfig-domain-timer.c: libvirt domain timer configuration
 *
 * Copyright (C) 2011, 2014 Red Hat, Inc.
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
 * Author: Christophe Fergeau <cfergeau@gmail.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_TIMER_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_TIMER, GVirConfigDomainTimerPrivate))

struct _GVirConfigDomainTimerPrivate
{
    gboolean unused;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(GVirConfigDomainTimer, gvir_config_domain_timer, GVIR_CONFIG_TYPE_OBJECT);


static void gvir_config_domain_timer_class_init(GVirConfigDomainTimerClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_timer_init(GVirConfigDomainTimer *timer)
{
    timer->priv = GVIR_CONFIG_DOMAIN_TIMER_GET_PRIVATE(timer);
}

void gvir_config_domain_timer_set_tick_policy(GVirConfigDomainTimer *timer,
                                              GVirConfigDomainTimerTickPolicy policy)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_TIMER(timer));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(timer),
                                               "tickpolicy",
                                               GVIR_CONFIG_TYPE_DOMAIN_TIMER_TICK_POLICY,
                                               policy,
                                               NULL);
}

GVirConfigDomainTimerTickPolicy
gvir_config_domain_timer_get_tick_policy(GVirConfigDomainTimer *timer)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_TIMER(timer),
                         GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_DELAY);

    return gvir_config_object_get_attribute_genum(GVIR_CONFIG_OBJECT(timer),
                                                  NULL,
                                                  "tickpolicy",
                                                  GVIR_CONFIG_TYPE_DOMAIN_TIMER_TICK_POLICY,
                                                  GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_DELAY);
}


void gvir_config_domain_timer_set_present(GVirConfigDomainTimer *timer,
                                          gboolean present)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_TIMER(timer));

    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(timer),
                                               "present",
                                               G_TYPE_BOOLEAN,
                                               present,
                                               NULL);
}


gboolean gvir_config_domain_timer_get_present(GVirConfigDomainTimer *timer)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN_TIMER(timer), TRUE);

    return gvir_config_object_get_attribute_boolean(GVIR_CONFIG_OBJECT(timer),
                                                    NULL,
                                                    "present",
                                                    TRUE);
}
