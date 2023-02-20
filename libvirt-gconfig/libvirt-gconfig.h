/*
 * libvirt-gconfig.h: libvirt gconfig integration
 *
 * Copyright (C) 2010-2014 Red Hat, Inc.
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

#pragma once

#define __LIBVIRT_GCONFIG_H__

#include <glib-object.h>
#include <libxml/tree.h>

#include <libvirt-gconfig/libvirt-gconfig-main.h>
#include <libvirt-gconfig/libvirt-gconfig-object.h>
#include <libvirt-gconfig/libvirt-gconfig-capabilities.h>
#include <libvirt-gconfig/libvirt-gconfig-capabilities-cpu.h>
#include <libvirt-gconfig/libvirt-gconfig-capabilities-cpu-feature.h>
#include <libvirt-gconfig/libvirt-gconfig-capabilities-cpu-model.h>
#include <libvirt-gconfig/libvirt-gconfig-capabilities-cpu-topology.h>
#include <libvirt-gconfig/libvirt-gconfig-capabilities-guest.h>
#include <libvirt-gconfig/libvirt-gconfig-capabilities-guest-arch.h>
#include <libvirt-gconfig/libvirt-gconfig-capabilities-guest-domain.h>
#include <libvirt-gconfig/libvirt-gconfig-capabilities-guest-feature.h>
#include <libvirt-gconfig/libvirt-gconfig-capabilities-host.h>
#include <libvirt-gconfig/libvirt-gconfig-capabilities-host-secmodel.h>
#include <libvirt-gconfig/libvirt-gconfig-domain.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-address.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-address-pci.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-address-usb.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-audio.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-capabilities.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-capabilities-os.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-chardev.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-chardev-source.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-chardev-source-pty.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-chardev-source-spiceport.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-chardev-source-spicevmc.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-chardev-source-unix.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-channel.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-clock.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-console.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-controller.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-controller-usb.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-cpu.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-cpu-feature.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-cpu-model.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-device.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-disk.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-disk-driver.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-filesys.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-graphics.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-graphics-desktop.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-graphics-rdp.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-graphics-sdl.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-graphics-spice.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-graphics-vnc.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-graphics-dbus.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-hostdev.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-hostdev-pci.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-input.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-interface.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-interface-bridge.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-interface-filterref-parameter.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-interface-filterref.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-interface-network.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-interface-user.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-memballoon.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-os.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-parallel.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-power-management.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-redirdev.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-seclabel.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-serial.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-smartcard.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-smartcard-host.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-smartcard-host-certificates.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-smartcard-passthrough.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-snapshot.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-snapshot-disk.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-sound.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-timer.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-timer-hpet.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-timer-pit.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-timer-rtc.h>
#include <libvirt-gconfig/libvirt-gconfig-domain-video.h>
#include <libvirt-gconfig/libvirt-gconfig-enum-types.h>
#include <libvirt-gconfig/libvirt-gconfig-helpers.h>
#include <libvirt-gconfig/libvirt-gconfig-interface.h>
#include <libvirt-gconfig/libvirt-gconfig-network.h>
#include <libvirt-gconfig/libvirt-gconfig-network-filter.h>
#include <libvirt-gconfig/libvirt-gconfig-node-device.h>
#include <libvirt-gconfig/libvirt-gconfig-secret.h>
#include <libvirt-gconfig/libvirt-gconfig-storage-permissions.h>
#include <libvirt-gconfig/libvirt-gconfig-storage-pool.h>
#include <libvirt-gconfig/libvirt-gconfig-storage-pool-source.h>
#include <libvirt-gconfig/libvirt-gconfig-storage-pool-target.h>
#include <libvirt-gconfig/libvirt-gconfig-storage-vol.h>
#include <libvirt-gconfig/libvirt-gconfig-storage-vol-backing-store.h>
#include <libvirt-gconfig/libvirt-gconfig-storage-vol-target.h>
