#!/usr/bin/python

from gi.repository import LibvirtGConfig;

domain = LibvirtGConfig.Domain.new()
domain.set_name("foo")
domain.set_memory(1024*1024*1024)
domain.set_vcpus(2)
domain.set_lifecycle(LibvirtGConfig.DomainLifecycleEvent.ON_POWEROFF,
                     LibvirtGConfig.DomainLifecycleAction.DESTROY)

clock = LibvirtGConfig.DomainClock.new()
clock.set_offset(LibvirtGConfig.DomainClockOffset.UTC)
domain.set_clock(clock)

os = LibvirtGConfig.DomainOs.new()
os.set_os_type(LibvirtGConfig.DomainOsType.HVM)
os.set_arch("x86_64")
devices = [ LibvirtGConfig.DomainOsBootDevice.CDROM,
            LibvirtGConfig.DomainOsBootDevice.NETWORK ]
os.set_boot_devices(devices)
domain.set_os(os)

disk = LibvirtGConfig.DomainDisk.new()
disk.set_type(LibvirtGConfig.DomainDiskType.FILE)
disk.set_guest_device_type(LibvirtGConfig.DomainDiskGuestDeviceType.DISK)
disk.set_source("/tmp/foo/bar")
disk.set_driver_name("qemu")
disk.set_driver_type("qcow2")
disk.set_target_bus(LibvirtGConfig.DomainDiskBus.IDE)
disk.set_target_dev("hda")
domain.add_device(disk)

interface = LibvirtGConfig.DomainInterfaceNetwork.new()
interface.set_source("default")
domain.add_device(interface)

interface = LibvirtGConfig.DomainInterfaceUser.new()
interface.set_ifname("eth0")
interface.set_link_state(LibvirtGConfig.DomainInterfaceLinkState.UP)
interface.set_mac("00:11:22:33:44:55")
interface.set_model("foo")
domain.add_device(interface)

input = LibvirtGConfig.DomainInput.new()
input.set_device_type(LibvirtGConfig.DomainInputDeviceType.TABLET)
input.set_bus(LibvirtGConfig.DomainInputBus.USB)
domain.add_device(input)

graphics = LibvirtGConfig.DomainGraphicsSpice.new()
graphics.set_port(1234)
domain.add_device(graphics)

video = LibvirtGConfig.DomainVideo.new()
video.set_model(LibvirtGConfig.DomainVideoModel.QXL)
domain.add_device(video)

console = LibvirtGConfig.DomainConsole.new()
pty = LibvirtGConfig.DomainChardevSourcePty.new()
console.set_source(pty)
domain.add_device(console)

print domain.to_xml()


pool = LibvirtGConfig.StoragePool.new()
pool.set_pool_type(LibvirtGConfig.StoragePoolType.DIR)

pool_source = LibvirtGConfig.StoragePoolSource.new()
pool_source.set_directory("/foo/bar")
pool.set_source(pool_source)

perms = LibvirtGConfig.StoragePermissions.new()
perms.set_owner(1001)
perms.set_group(1005)
perms.set_mode(0744)
perms.set_label("virt_image_t")

pool_target = LibvirtGConfig.StoragePoolTarget.new()
pool_target.set_path("/dev/disk/by-path")
pool_target.set_permissions(perms)
pool.set_target(pool_target)

print pool.to_xml()


vol = LibvirtGConfig.StorageVol.new()
vol.set_name("my-vol")
vol.set_capacity(0xdeadbeef)

vol_target = LibvirtGConfig.StorageVolTarget.new()
vol_target.set_format("qcow2")
vol_target.set_permissions(perms)
vol.set_target(vol_target)

print vol.to_xml()
