/*
 * libvirt-gconfig-domain-timer-hpet.c: libvirt domain HPET timer configuration
 *
 * Copyright (C) 2012, 2014 Red Hat, Inc.
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

#define GVIR_CONFIG_DOMAIN_TIMER_HPET_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_TIMER_HPET, GVirConfigDomainTimerHpetPrivate))

struct _GVirConfigDomainTimerHpetPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainTimerHpet, gvir_config_domain_timer_hpet, GVIR_CONFIG_TYPE_DOMAIN_TIMER);


static void gvir_config_domain_timer_hpet_class_init(GVirConfigDomainTimerHpetClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_timer_hpet_init(GVirConfigDomainTimerHpet *timer)
{
    timer->priv = GVIR_CONFIG_DOMAIN_TIMER_HPET_GET_PRIVATE(timer);
}


GVirConfigDomainTimerHpet *gvir_config_domain_timer_hpet_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_TIMER_HPET,
                                    "timer", NULL);
    gvir_config_object_set_attribute(object, "name", "hpet", NULL);
    return GVIR_CONFIG_DOMAIN_TIMER_HPET(object);
}

GVirConfigDomainTimerHpet *gvir_config_domain_timer_hpet_new_from_xml(const gchar *xml,
                                                GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_TIMER_HPET,
                                             "timer", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "hpet") != 0) {
        g_object_unref(G_OBJECT(object));
        g_return_val_if_reached(NULL);
    }
    return GVIR_CONFIG_DOMAIN_TIMER_HPET(object);
}
