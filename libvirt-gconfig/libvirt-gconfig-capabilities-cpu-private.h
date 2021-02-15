/*
 * libvirt-gconfig-capabilities-cpu-private.h: libvirt CPU capabilities
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
 * Author: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
 */

#pragma once

G_BEGIN_DECLS

#include <libvirt-gconfig/libvirt-gconfig.h>

GList *
gvir_config_capabilities_cpu_get_features_with_type(GVirConfigCapabilitiesCpu *cpu,
                                                    GType type);

G_END_DECLS
