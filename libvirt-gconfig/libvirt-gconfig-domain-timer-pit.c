/*
 * libvirt-gconfig-domain-timer-pit.c: libvirt domain PIT timer configuration
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

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_TIMER_PIT_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT, GVirConfigDomainTimerPitPrivate))

struct _GVirConfigDomainTimerPitPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainTimerPit, gvir_config_domain_timer_pit, GVIR_CONFIG_TYPE_DOMAIN_TIMER);


static void gvir_config_domain_timer_pit_class_init(GVirConfigDomainTimerPitClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_timer_pit_init(GVirConfigDomainTimerPit *timer)
{
    timer->priv = GVIR_CONFIG_DOMAIN_TIMER_PIT_GET_PRIVATE(timer);
}


GVirConfigDomainTimerPit *gvir_config_domain_timer_pit_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT,
                                    "timer", NULL);
    gvir_config_object_set_attribute(object, "name", "pit", NULL);
    return GVIR_CONFIG_DOMAIN_TIMER_PIT(object);
}

GVirConfigDomainTimerPit *gvir_config_domain_timer_pit_new_from_xml(const gchar *xml,
                                                GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_TIMER_PIT,
                                             "timer", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "type"), "pit") != 0) {
        g_object_unref(G_OBJECT(object));
        g_return_val_if_reached(NULL);
    }
    return GVIR_CONFIG_DOMAIN_TIMER_PIT(object);
}
