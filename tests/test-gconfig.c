/*
 * test-gconfig.c: unit tests for libvirt-gconfig
 *
 * Copyright (C) 2014 Red Hat, Inc.
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

#include <libvirt-gconfig/libvirt-gconfig.h>

#define g_assert_cmpstr_free1(str1, op, str2) G_STMT_START { \
    char *alloced_str = (str1); \
    g_assert_cmpstr(alloced_str, op, (str2)); \
    g_free(alloced_str); \
} G_STMT_END

static char * load_xml(const char *reference_file)
{
    const char *reference_path;
    GError *error = NULL;
    char *reference_xml;

    reference_path = g_test_get_filename(G_TEST_DIST, "xml",
                                         reference_file, NULL);
    g_file_get_contents(reference_path, &reference_xml, NULL, &error);
    g_assert_no_error(error);
    /* I could not generate text files without a trailing \n with vim or
     * gedit, workaround this issue by removing trailing whitespace from
     * the reference file */
    g_strchomp(reference_xml);

    return reference_xml;
}


static void check_xml(GVirConfigDomain *domain, const char *reference_file)
{
    char *reference_xml;
    char *xml;

    reference_xml = load_xml(reference_file);

    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(domain));
    g_assert_cmpstr(xml, ==, reference_xml);
    g_free(xml);
    g_free(reference_xml);
}


static GList *get_devices_by_type(GVirConfigDomain *domain, GType type)
{
    GList *devices;
    GList *it;
    GList *matches = NULL;

    devices = gvir_config_domain_get_devices(domain);
    for (it = devices; it != NULL; it = it->next) {
        if (g_type_is_a(G_OBJECT_TYPE(it->data), type)) {
            matches = g_list_prepend(matches, g_object_ref(G_OBJECT(it->data)));
        }
    }
    g_list_free_full(devices, g_object_unref);

    return g_list_reverse(matches);
}


static void test_domain(void)
{
    GVirConfigDomain *domain;
    const char *features[] = { "foo", "bar", "baz", NULL };
    GStrv feat;
    unsigned int i;

    domain = gvir_config_domain_new();
    g_assert(domain != NULL);

    gvir_config_domain_set_virt_type(domain, GVIR_CONFIG_DOMAIN_VIRT_KVM);
    g_assert_cmpint(gvir_config_domain_get_virt_type(domain), ==, GVIR_CONFIG_DOMAIN_VIRT_KVM);

    gvir_config_domain_set_name(domain, "foo");
    g_assert_cmpstr(gvir_config_domain_get_name(domain), ==, "foo");

    gvir_config_domain_set_memory(domain, 1234);
    g_assert_cmpint(gvir_config_domain_get_memory(domain), ==, 1234);
    gvir_config_domain_set_vcpus(domain, 3);
    g_assert_cmpint(gvir_config_domain_get_vcpus(domain), ==, 3);

    gvir_config_domain_set_features(domain, (const GStrv)features);
    feat = gvir_config_domain_get_features(domain);
    for (i = 0; features[i] != NULL; i++) {
        g_assert(feat[i] != NULL);
        g_assert_cmpstr(feat[i], ==, features[i]);
    }
    g_strfreev(feat);

    gvir_config_domain_set_lifecycle(domain,
                                     GVIR_CONFIG_DOMAIN_LIFECYCLE_ON_POWEROFF,
                                     GVIR_CONFIG_DOMAIN_LIFECYCLE_RESTART);

    gvir_config_domain_set_custom_xml_ns_children(domain, "<libosinfo><os id=\"http://centos.org/centos/7\"/></libosinfo>", "libosinfo", "http://libosinfo.org/xmlns/libvirt/domain/1.0", NULL);
    gvir_config_domain_set_custom_xml_ns_children(domain, "<instance><flavor name=\"m1.small\"><memory>512</memory><disk>10</disk></flavor></instance>", "nova", "http://openstack.org/nova/instance/1", NULL);
    gvir_config_domain_set_custom_xml(domain, "<gnome-boxes><os-state>installed</os-state></gnome-boxes>", "boxes", "https://wiki.gnome.org/Apps/Boxes", NULL);
    gvir_config_domain_set_custom_xml(domain, "<foo/>", "ns", "http://foo", NULL);
    gvir_config_domain_set_custom_xml(domain, "<foo/>", "nsbar", "http://bar", NULL);
    gvir_config_domain_set_custom_xml(domain, "<foo/>", "ns", "http://bar", NULL);
    gvir_config_domain_set_custom_xml(domain, "<bar/>", "ns", "http://foo", NULL);

    g_assert_cmpstr_free1(gvir_config_domain_get_custom_xml(domain, "http://foo"), ==, "<ns:bar xmlns:ns=\"http://foo\"/>");
    g_assert_cmpstr_free1(gvir_config_domain_get_custom_xml(domain, "http://bar"), ==, "<ns:foo xmlns:ns=\"http://bar\"/>");

    check_xml(domain, "gconfig-domain.xml");

    g_object_unref(G_OBJECT(domain));
}


static void test_domain_clock(void)
{
    GVirConfigDomain *domain;
    GVirConfigDomainClock *klock;
    GVirConfigDomainTimerPit *pit;
    GVirConfigDomainTimerRtc *rtc;
    GVirConfigDomainTimerHpet *hpet;

    domain = gvir_config_domain_new();

    klock = gvir_config_domain_clock_new();
    gvir_config_domain_clock_set_offset(klock, GVIR_CONFIG_DOMAIN_CLOCK_TIMEZONE);
    gvir_config_domain_clock_set_timezone(klock, "CEST");
    g_assert_cmpint(gvir_config_domain_clock_get_offset(klock), ==, GVIR_CONFIG_DOMAIN_CLOCK_TIMEZONE);
    g_assert_cmpstr(gvir_config_domain_clock_get_timezone(klock), ==, "CEST");

    pit = gvir_config_domain_timer_pit_new();
    gvir_config_domain_timer_set_tick_policy(GVIR_CONFIG_DOMAIN_TIMER(pit),
                                             GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_DELAY);
    gvir_config_domain_clock_add_timer(klock, GVIR_CONFIG_DOMAIN_TIMER(pit));
    g_assert_cmpint(gvir_config_domain_timer_get_tick_policy(GVIR_CONFIG_DOMAIN_TIMER(pit)), ==, GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_DELAY);
    g_assert(gvir_config_domain_timer_get_present(GVIR_CONFIG_DOMAIN_TIMER(pit)) != FALSE);
    g_object_unref(G_OBJECT(pit));

    rtc = gvir_config_domain_timer_rtc_new();
    gvir_config_domain_timer_set_tick_policy(GVIR_CONFIG_DOMAIN_TIMER(rtc),
                                             GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_CATCHUP);
    gvir_config_domain_clock_add_timer(klock, GVIR_CONFIG_DOMAIN_TIMER(rtc));
    g_assert_cmpint(gvir_config_domain_timer_get_tick_policy(GVIR_CONFIG_DOMAIN_TIMER(rtc)), ==, GVIR_CONFIG_DOMAIN_TIMER_TICK_POLICY_CATCHUP);
    g_assert(gvir_config_domain_timer_get_present(GVIR_CONFIG_DOMAIN_TIMER(rtc)) != FALSE);
    g_object_unref(G_OBJECT(rtc));

    hpet = gvir_config_domain_timer_hpet_new();
    gvir_config_domain_timer_set_present(GVIR_CONFIG_DOMAIN_TIMER(hpet), FALSE);
    gvir_config_domain_clock_add_timer(klock, GVIR_CONFIG_DOMAIN_TIMER(hpet));
    g_assert(gvir_config_domain_timer_get_present(GVIR_CONFIG_DOMAIN_TIMER(hpet)) == FALSE);
    g_object_unref(G_OBJECT(hpet));

    gvir_config_domain_set_clock(domain, klock);
    g_object_unref(G_OBJECT(klock));

    klock = gvir_config_domain_get_clock(domain);
    g_assert(klock != NULL);
    g_assert_cmpint(gvir_config_domain_clock_get_offset(klock), ==, GVIR_CONFIG_DOMAIN_CLOCK_TIMEZONE);
    g_assert_cmpstr(gvir_config_domain_clock_get_timezone(klock), ==, "CEST");
    g_object_unref(G_OBJECT(klock));
    /* FIXME: gvir_config_domain_clock_get_timers() */

    check_xml(domain, "gconfig-domain-clock.xml");

    gvir_config_domain_set_clock(domain, NULL);
    klock = gvir_config_domain_get_clock(domain);
    g_assert(klock == NULL);

    g_object_unref(G_OBJECT(domain));
}


static void domain_os_check_boot_devices(GVirConfigDomainOs *os)
{
    GList *devices = NULL;
    GList *it;

    devices = gvir_config_domain_os_get_boot_devices(os);
    it = devices;
    g_assert_cmpint(GPOINTER_TO_INT(it->data), ==, GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_CDROM);
    it = it->next;
    g_assert_cmpint(GPOINTER_TO_INT(it->data), ==, GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_NETWORK);
    g_assert(it->next == NULL);
    g_list_free(devices);
}


static void test_domain_os(void)
{
    GVirConfigDomain *domain;
    GVirConfigDomainOs *os;
    GList *devices = NULL;

    domain = gvir_config_domain_new();
    os = gvir_config_domain_os_new();
    gvir_config_domain_os_set_os_type(os, GVIR_CONFIG_DOMAIN_OS_TYPE_HVM);
    gvir_config_domain_os_set_arch(os, "x86_64");
    gvir_config_domain_os_set_machine(os, "q35");
    gvir_config_domain_os_set_firmware(os, GVIR_CONFIG_DOMAIN_OS_FIRMWARE_EFI);
    g_assert_cmpint(gvir_config_domain_os_get_os_type(os), ==, GVIR_CONFIG_DOMAIN_OS_TYPE_HVM);
    g_assert_cmpstr(gvir_config_domain_os_get_arch(os), ==, "x86_64");
    g_assert_cmpstr(gvir_config_domain_os_get_machine(os), ==, "q35");
    g_assert_cmpint(gvir_config_domain_os_get_firmware(os), ==, GVIR_CONFIG_DOMAIN_OS_FIRMWARE_EFI);
    devices = g_list_append(devices,
                             GINT_TO_POINTER(GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_CDROM));
    devices = g_list_append(devices,
                            GINT_TO_POINTER(GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_NETWORK));
    gvir_config_domain_os_set_boot_devices(os, devices);
    g_list_free(devices);
    devices = NULL;
    domain_os_check_boot_devices(os);
    gvir_config_domain_set_os(domain, os);
    g_object_unref(G_OBJECT(os));

    os = gvir_config_domain_get_os(domain);
    g_assert(os != NULL);
    g_assert_cmpstr(gvir_config_domain_os_get_arch(os), ==, "x86_64");
    g_assert_cmpint(gvir_config_domain_os_get_os_type(os), ==, GVIR_CONFIG_DOMAIN_OS_TYPE_HVM);
    g_assert_cmpstr(gvir_config_domain_os_get_machine(os), ==, "q35");
    g_assert_cmpint(gvir_config_domain_os_get_firmware(os), ==, GVIR_CONFIG_DOMAIN_OS_FIRMWARE_EFI);
    domain_os_check_boot_devices(os);
    g_object_unref(G_OBJECT(os));

    check_xml(domain, "gconfig-domain-os.xml");

    gvir_config_domain_set_os(domain, NULL);
    os = gvir_config_domain_get_os(domain);
    g_assert(os == NULL);

    g_object_unref(G_OBJECT(domain));
}


static void test_domain_cpu(void)
{
    GVirConfigDomain *domain;
    /* cpu node */
    GVirConfigDomainCpu *cpu;

    domain = gvir_config_domain_new();
    cpu = gvir_config_domain_cpu_new();
    gvir_config_domain_set_vcpus(domain, 3);

    /* cpu/feature nodes */
    GVirConfigDomainCpuFeature *feature;
    GList *cpu_features = NULL;

    gvir_config_domain_cpu_set_match_policy(cpu, GVIR_CONFIG_DOMAIN_CPU_MATCH_POLICY_STRICT);
    gvir_config_domain_cpu_set_mode(cpu, GVIR_CONFIG_DOMAIN_CPU_MODE_HOST_PASSTHROUGH);
    feature = gvir_config_domain_cpu_feature_new();
    gvir_config_capabilities_cpu_feature_set_name(GVIR_CONFIG_CAPABILITIES_CPU_FEATURE(feature),
                                                  "foo");
    gvir_config_domain_cpu_feature_set_policy(feature, GVIR_CONFIG_DOMAIN_CPU_FEATURE_POLICY_REQUIRE);
    gvir_config_capabilities_cpu_add_feature(GVIR_CONFIG_CAPABILITIES_CPU(cpu),
                                             GVIR_CONFIG_CAPABILITIES_CPU_FEATURE(feature));
    g_object_unref(feature);

    /* cpu/topology nodes */
    GVirConfigCapabilitiesCpuTopology *topology;

    topology = gvir_config_capabilities_cpu_topology_new();
    gvir_config_capabilities_cpu_topology_set_cores(topology, 1);
    gvir_config_capabilities_cpu_topology_set_sockets(topology, 2);
    gvir_config_capabilities_cpu_topology_set_threads(topology, 3);
    gvir_config_capabilities_cpu_set_topology(GVIR_CONFIG_CAPABILITIES_CPU(cpu),
                                              topology);
    g_object_unref(topology);
    gvir_config_domain_set_cpu(domain, cpu);
    g_object_unref(cpu);


    cpu = gvir_config_domain_get_cpu(domain);
    g_assert(cpu != NULL);
    g_assert(GVIR_CONFIG_IS_CAPABILITIES_CPU(cpu));
    g_assert_cmpint(gvir_config_domain_cpu_get_match_policy(cpu), ==, GVIR_CONFIG_DOMAIN_CPU_MATCH_POLICY_STRICT);
    g_assert_cmpint(gvir_config_domain_cpu_get_mode(cpu), ==, GVIR_CONFIG_DOMAIN_CPU_MODE_HOST_PASSTHROUGH);

    cpu_features = gvir_config_capabilities_cpu_get_features(GVIR_CONFIG_CAPABILITIES_CPU(cpu));
    g_assert_cmpint(g_list_length(cpu_features), ==, 1);
    g_assert(GVIR_CONFIG_IS_DOMAIN_CPU_FEATURE(cpu_features->data));
    feature = GVIR_CONFIG_DOMAIN_CPU_FEATURE(cpu_features->data);
    g_assert_cmpstr(gvir_config_capabilities_cpu_feature_get_name(GVIR_CONFIG_CAPABILITIES_CPU_FEATURE(feature)), ==, "foo");
    g_assert_cmpint(gvir_config_domain_cpu_feature_get_policy(feature), ==, GVIR_CONFIG_DOMAIN_CPU_FEATURE_POLICY_REQUIRE);
    g_list_free_full(cpu_features, g_object_unref);

    topology = gvir_config_capabilities_cpu_get_topology(GVIR_CONFIG_CAPABILITIES_CPU(cpu));
    g_assert(topology != NULL);
    g_assert_cmpint(gvir_config_capabilities_cpu_topology_get_cores(topology), ==, 1);
    g_assert_cmpint(gvir_config_capabilities_cpu_topology_get_sockets(topology), ==, 2);
    g_assert_cmpint(gvir_config_capabilities_cpu_topology_get_threads(topology), ==, 3);
    g_object_unref(topology);
    g_object_unref(cpu);

    check_xml(domain, "gconfig-domain-cpu.xml");

    cpu = gvir_config_domain_get_cpu(domain);
    gvir_config_capabilities_cpu_set_topology(GVIR_CONFIG_CAPABILITIES_CPU(cpu),
                                              NULL);
    topology = gvir_config_capabilities_cpu_get_topology(GVIR_CONFIG_CAPABILITIES_CPU(cpu));
    g_assert(topology == NULL);
    g_object_unref(G_OBJECT(cpu));

    gvir_config_domain_set_cpu(domain, NULL);
    cpu = gvir_config_domain_get_cpu(domain);
    g_assert(cpu == NULL);

    g_object_unref(G_OBJECT(domain));
}


static void test_domain_device_disk(void)
{
    GVirConfigDomain *domain;
    GVirConfigDomainDisk *disk;
    GVirConfigDomainDiskDriver *driver;
    GList *disks;

    domain = gvir_config_domain_new();

    driver = gvir_config_domain_disk_driver_new();
    gvir_config_domain_disk_driver_set_name(driver, "foo");
    gvir_config_domain_disk_driver_set_format(driver, GVIR_CONFIG_DOMAIN_DISK_FORMAT_BOCHS);
    gvir_config_domain_disk_driver_set_name(driver, "qemu");
    gvir_config_domain_disk_driver_set_cache(driver, GVIR_CONFIG_DOMAIN_DISK_CACHE_NONE);
    gvir_config_domain_disk_driver_set_format(driver, GVIR_CONFIG_DOMAIN_DISK_FORMAT_QCOW2);
    gvir_config_domain_disk_driver_set_copy_on_read(driver, TRUE);

    disk = gvir_config_domain_disk_new();
    gvir_config_domain_disk_set_type(disk, GVIR_CONFIG_DOMAIN_DISK_FILE);
    gvir_config_domain_disk_set_guest_device_type(disk, GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_DISK);
    gvir_config_domain_disk_set_source(disk, "/tmp/foo/bar");
    gvir_config_domain_disk_set_startup_policy (disk, GVIR_CONFIG_DOMAIN_DISK_STARTUP_POLICY_REQUISITE);
    gvir_config_domain_disk_set_target_bus(disk, GVIR_CONFIG_DOMAIN_DISK_BUS_IDE);
    gvir_config_domain_disk_set_target_dev(disk, "hda");
    gvir_config_domain_disk_set_driver(disk, driver);
    g_object_unref(G_OBJECT(driver));
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(disk));
    g_object_unref(disk);

    check_xml(domain, "gconfig-domain-device-disk.xml");

    disks = get_devices_by_type(domain, GVIR_CONFIG_TYPE_DOMAIN_DISK);
    g_assert_cmpint(g_list_length(disks), ==, 1);
    g_assert(GVIR_CONFIG_IS_DOMAIN_DISK(disks->data));
    disk = GVIR_CONFIG_DOMAIN_DISK(disks->data);
    g_assert_cmpint(gvir_config_domain_disk_get_disk_type(disk), ==, GVIR_CONFIG_DOMAIN_DISK_FILE);
    g_assert_cmpint(gvir_config_domain_disk_get_guest_device_type(disk), ==, GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_DISK);
    g_assert_cmpint(gvir_config_domain_disk_get_startup_policy (disk), ==, GVIR_CONFIG_DOMAIN_DISK_STARTUP_POLICY_REQUISITE);
    g_assert_cmpstr(gvir_config_domain_disk_get_source(disk), ==, "/tmp/foo/bar");
    driver = gvir_config_domain_disk_get_driver(disk);
    g_assert(driver != NULL);
    g_assert_cmpint(gvir_config_domain_disk_driver_get_cache(driver), ==, GVIR_CONFIG_DOMAIN_DISK_CACHE_NONE);
    g_assert_cmpstr(gvir_config_domain_disk_driver_get_name(driver), ==, "qemu");
    g_assert_cmpint(gvir_config_domain_disk_driver_get_format(driver), ==, GVIR_CONFIG_DOMAIN_DISK_FORMAT_QCOW2);
    g_assert(gvir_config_domain_disk_driver_get_copy_on_read(driver));
    g_assert_cmpint(gvir_config_domain_disk_get_target_bus(disk), ==, GVIR_CONFIG_DOMAIN_DISK_BUS_IDE);
    g_assert_cmpstr(gvir_config_domain_disk_get_target_dev(disk), ==, "hda");
    g_object_unref(G_OBJECT(driver));

    gvir_config_domain_disk_set_driver(disk, NULL);
    driver = gvir_config_domain_disk_get_driver(disk);
    g_assert(driver == NULL);

    g_list_free_full(disks, g_object_unref);

    g_object_unref(G_OBJECT(domain));
}


static void test_domain_device_filesystem(void)
{
    GVirConfigDomain *domain;
    GVirConfigDomainFilesys *fs;

    domain = gvir_config_domain_new();

    fs = gvir_config_domain_filesys_new();
    gvir_config_domain_filesys_set_type(fs, GVIR_CONFIG_DOMAIN_FILESYS_FILE);
    gvir_config_domain_filesys_set_access_type(fs, GVIR_CONFIG_DOMAIN_FILESYS_ACCESS_MAPPED);
    gvir_config_domain_filesys_set_driver_type(fs, GVIR_CONFIG_DOMAIN_FILESYS_DRIVER_NBD);
    gvir_config_domain_filesys_set_driver_format(fs, GVIR_CONFIG_DOMAIN_DISK_FORMAT_QCOW2);
    gvir_config_domain_filesys_set_source(fs, "/path/to/source");
    gvir_config_domain_filesys_set_target(fs, "/path/to/target1");
    gvir_config_domain_filesys_set_readonly(fs, TRUE);
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(fs));
    g_object_unref(fs);

    /* Add a RAM fs */
    fs = gvir_config_domain_filesys_new();
    gvir_config_domain_filesys_set_type(fs, GVIR_CONFIG_DOMAIN_FILESYS_RAM);
    gvir_config_domain_filesys_set_access_type(fs, GVIR_CONFIG_DOMAIN_FILESYS_ACCESS_PASSTHROUGH);
    gvir_config_domain_filesys_set_driver_type(fs, GVIR_CONFIG_DOMAIN_FILESYS_DRIVER_PATH);
    gvir_config_domain_filesys_set_ram_usage(fs, 1234);
    gvir_config_domain_filesys_set_target(fs, "/path/to/target2");
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(fs));
    g_object_unref(fs);

    check_xml(domain, "gconfig-domain-device-filesys.xml");

    g_object_unref(G_OBJECT(domain));
}


static void test_domain_device_network(void)
{
    GVirConfigDomain *domain;
    GVirConfigDomainInterface *interface;

    domain = gvir_config_domain_new();

    interface = GVIR_CONFIG_DOMAIN_INTERFACE(gvir_config_domain_interface_network_new());
    gvir_config_domain_interface_network_set_source(GVIR_CONFIG_DOMAIN_INTERFACE_NETWORK(interface),
                                                    "default");
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(interface));
    g_object_unref(G_OBJECT(interface));

    interface = GVIR_CONFIG_DOMAIN_INTERFACE(gvir_config_domain_interface_user_new());
    gvir_config_domain_interface_set_ifname(interface, "eth0");
    gvir_config_domain_interface_set_link_state(interface,
                                                GVIR_CONFIG_DOMAIN_INTERFACE_LINK_STATE_UP);
    gvir_config_domain_interface_set_mac(interface, "00:11:22:33:44:55");
    gvir_config_domain_interface_set_model(interface, "foo");
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(interface));
    g_object_unref(G_OBJECT(interface));

    check_xml(domain, "gconfig-domain-device-network.xml");

    g_object_unref(G_OBJECT(domain));
}


static void test_domain_device_input(void)
{
    GVirConfigDomain *domain;
    GVirConfigDomainInput *input;
    GList *inputs;

    domain = gvir_config_domain_new();

    input = gvir_config_domain_input_new();
    gvir_config_domain_input_set_device_type(input,
                                             GVIR_CONFIG_DOMAIN_INPUT_DEVICE_TABLET);
    gvir_config_domain_input_set_bus(input, GVIR_CONFIG_DOMAIN_INPUT_BUS_USB);
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(input));
    g_object_unref(G_OBJECT(input));

    check_xml(domain, "gconfig-domain-device-input.xml");

    inputs = get_devices_by_type(domain, GVIR_CONFIG_TYPE_DOMAIN_INPUT);
    g_assert_cmpint(g_list_length(inputs), ==, 1);
    g_assert(GVIR_CONFIG_IS_DOMAIN_INPUT(inputs->data));
    input = GVIR_CONFIG_DOMAIN_INPUT(inputs->data);
    g_assert_cmpint(gvir_config_domain_input_get_device_type(input), ==, GVIR_CONFIG_DOMAIN_INPUT_DEVICE_TABLET);
    g_assert_cmpint(gvir_config_domain_input_get_bus(input), ==, GVIR_CONFIG_DOMAIN_INPUT_BUS_USB);
    g_list_free_full(inputs, g_object_unref);

    g_object_unref(G_OBJECT(domain));
}


static void test_domain_device_graphics(void)
{
    GVirConfigDomain *domain;
    GVirConfigDomainGraphicsSpice *graphics;

    domain = gvir_config_domain_new();
    graphics = gvir_config_domain_graphics_spice_new();
    gvir_config_domain_graphics_spice_set_port(graphics, 1234);
    g_assert_cmpint(gvir_config_domain_graphics_spice_get_port(graphics), ==, 1234);

    /* SPICE image compression configuration */
    gvir_config_domain_graphics_spice_set_image_compression
        (graphics, GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE_IMAGE_COMPRESSION_AUTO_LZ);
    g_assert_cmpint(gvir_config_domain_graphics_spice_get_image_compression(graphics), ==,
             GVIR_CONFIG_DOMAIN_GRAPHICS_SPICE_IMAGE_COMPRESSION_AUTO_LZ);

    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(graphics));
    g_object_unref(G_OBJECT(graphics));

    check_xml(domain, "gconfig-domain-device-graphics.xml");

    g_object_unref(G_OBJECT(domain));
}


static void test_domain_device_video(void)
{
    GVirConfigDomain *domain;

    domain = gvir_config_domain_new();

    /* video node */
    GVirConfigDomainVideo *video;

    video = gvir_config_domain_video_new();
    gvir_config_domain_video_set_model(video,
                                       GVIR_CONFIG_DOMAIN_VIDEO_MODEL_QXL);
    gvir_config_domain_video_set_heads(video, 4);
    gvir_config_domain_video_set_vram(video, 256*1024);
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(video));
    g_object_unref(G_OBJECT(video));

    check_xml(domain, "gconfig-domain-device-video.xml");

    g_object_unref(G_OBJECT(domain));
}


static void test_domain_device_sound(void)
{
    GVirConfigDomain *domain;

    domain = gvir_config_domain_new();
    /* sound node */
    GVirConfigDomainSound *sound;

    sound = gvir_config_domain_sound_new();
    gvir_config_domain_sound_set_model(sound,
                                       GVIR_CONFIG_DOMAIN_SOUND_MODEL_ES1370);
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(sound));
    g_object_unref(G_OBJECT(sound));

    check_xml(domain, "gconfig-domain-device-sound.xml");

    g_object_unref(G_OBJECT(domain));
}


static void test_domain_device_console(void)
{
    GVirConfigDomain *domain;

    domain = gvir_config_domain_new();
    /* console node */
    GVirConfigDomainConsole *console;
    GVirConfigDomainChardevSourcePty *pty;

    console = gvir_config_domain_console_new();
    pty = gvir_config_domain_chardev_source_pty_new();
    gvir_config_domain_chardev_set_source(GVIR_CONFIG_DOMAIN_CHARDEV(console),
                                          GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE(pty));
    g_object_unref(G_OBJECT(pty));
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(console));
    g_object_unref(G_OBJECT(console));

    check_xml(domain, "gconfig-domain-device-console.xml");

    g_object_unref(G_OBJECT(domain));
}


static void test_domain_device_channel(void)
{
    GVirConfigDomain *domain;

    domain = gvir_config_domain_new();
    /* spice agent channel */
    GVirConfigDomainChannel *channel;
    GVirConfigDomainChardevSourceSpiceVmc *spicevmc;
    GVirConfigDomainChardevSourceSpicePort *spiceport;
    GVirConfigDomainChardevSourceUnix *unix_source;

    channel = gvir_config_domain_channel_new();
    gvir_config_domain_channel_set_target_type(channel,
                                               GVIR_CONFIG_DOMAIN_CHANNEL_TARGET_VIRTIO);
    spicevmc = gvir_config_domain_chardev_source_spicevmc_new();
    gvir_config_domain_chardev_set_source(GVIR_CONFIG_DOMAIN_CHARDEV(channel),
                                          GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE(spicevmc));
    g_object_unref(G_OBJECT(spicevmc));
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(channel));
    g_object_unref(G_OBJECT(channel));

    channel = gvir_config_domain_channel_new();
    gvir_config_domain_channel_set_target_type(channel,
                                               GVIR_CONFIG_DOMAIN_CHANNEL_TARGET_VIRTIO);
    gvir_config_domain_channel_set_target_name(channel,
                                               "org.spice-space.webdav.0");
    spiceport = gvir_config_domain_chardev_source_spiceport_new();
    gvir_config_domain_chardev_source_spiceport_set_channel(spiceport, "org.spice-space.webdav.0");
    gvir_config_domain_chardev_set_source(GVIR_CONFIG_DOMAIN_CHARDEV(channel),
                                          GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE(spiceport));
    g_object_unref(G_OBJECT(spiceport));
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(channel));
    g_object_unref(G_OBJECT(channel));

    channel = gvir_config_domain_channel_new();
    gvir_config_domain_channel_set_target_type(channel,
                                               GVIR_CONFIG_DOMAIN_CHANNEL_TARGET_VIRTIO);
    gvir_config_domain_channel_set_target_name(channel, "org.qemu.guest_agent.0");
    unix_source = gvir_config_domain_chardev_source_unix_new();
    gvir_config_domain_chardev_set_source(GVIR_CONFIG_DOMAIN_CHARDEV(channel),
                                          GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE(unix_source));
    g_object_unref(G_OBJECT(unix_source));
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(channel));
    g_object_unref(G_OBJECT(channel));
    check_xml(domain, "gconfig-domain-device-channel.xml");

    g_object_unref(G_OBJECT(domain));
}


static GVirConfigDomainControllerUsb *
domain_device_usb_redir_create_usb_controller(GVirConfigDomainControllerUsbModel model,
                                              guint indx,
                                              GVirConfigDomainControllerUsb *master,
                                              guint start_port,
                                              guint domain, guint bus,
                                              guint slot, guint function,
                                              gboolean multifunction)
{
    GVirConfigDomainControllerUsb *controller;
    GVirConfigDomainAddressPci *address;

    controller = gvir_config_domain_controller_usb_new();
    gvir_config_domain_controller_usb_set_model(controller, model);
    gvir_config_domain_controller_set_index(GVIR_CONFIG_DOMAIN_CONTROLLER(controller), indx);
    if (master)
        gvir_config_domain_controller_usb_set_master(controller, master, start_port);
    address = gvir_config_domain_address_pci_new();
    gvir_config_domain_address_pci_set_domain(address, domain);
    gvir_config_domain_address_pci_set_bus(address, bus);
    gvir_config_domain_address_pci_set_slot(address, slot);
    gvir_config_domain_address_pci_set_function(address, function);
    if (multifunction)
        gvir_config_domain_address_pci_set_multifunction(address, multifunction);
    gvir_config_domain_controller_set_address(GVIR_CONFIG_DOMAIN_CONTROLLER(controller),
                                              GVIR_CONFIG_DOMAIN_ADDRESS(address));
    g_object_unref(G_OBJECT(address));

    return controller;
}


static GVirConfigDomainRedirdev *
domain_device_usb_redir_create_redirdev(guint bus, guint port)
{
    GVirConfigDomainRedirdev *redirdev;
    GVirConfigDomainAddressUsb *address;
    GVirConfigDomainChardevSourceSpiceVmc *spicevmc;
    gchar *port_str;

    redirdev = gvir_config_domain_redirdev_new();
    gvir_config_domain_redirdev_set_bus(redirdev,
                                        GVIR_CONFIG_DOMAIN_REDIRDEV_BUS_USB);
    spicevmc = gvir_config_domain_chardev_source_spicevmc_new();
    gvir_config_domain_chardev_set_source(GVIR_CONFIG_DOMAIN_CHARDEV(redirdev),
                                          GVIR_CONFIG_DOMAIN_CHARDEV_SOURCE(spicevmc));
    g_object_unref(G_OBJECT(spicevmc));

    address = gvir_config_domain_address_usb_new();
    gvir_config_domain_address_usb_set_bus(address, bus);
    port_str = g_strdup_printf("%d", port);
    gvir_config_domain_address_usb_set_port(address, port_str);
    g_free(port_str);
    gvir_config_domain_redirdev_set_address(redirdev,
                                            GVIR_CONFIG_DOMAIN_ADDRESS(address));
    g_object_unref(G_OBJECT(address));

    return redirdev;
}


static void test_domain_device_usb_redir(void)
{
    GVirConfigDomain *domain;

    domain = gvir_config_domain_new();
    /* spice usb redirection */
    GVirConfigDomainControllerUsb *ehci;
    GVirConfigDomainControllerUsb *uhci1;
    GVirConfigDomainControllerUsb *uhci2;
    GVirConfigDomainControllerUsb *uhci3;
    GVirConfigDomainRedirdev *redirdev;

    ehci = domain_device_usb_redir_create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_EHCI1,
                                                         1, NULL, 0, 0, 0, 8, 7, FALSE);
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(ehci));
    uhci1 = domain_device_usb_redir_create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI1,
                                                          7, ehci, 0, 0, 0, 8, 0, TRUE);
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(uhci1));
    uhci2 = domain_device_usb_redir_create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI2,
                                                          7, ehci, 2, 0, 0, 8, 1, FALSE);
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(uhci2));
    uhci3 = domain_device_usb_redir_create_usb_controller(GVIR_CONFIG_DOMAIN_CONTROLLER_USB_MODEL_ICH9_UHCI3,
                                                          7, ehci, 4, 0, 0, 8, 2, FALSE);
    g_assert_cmpint(gvir_config_domain_controller_get_index(GVIR_CONFIG_DOMAIN_CONTROLLER(uhci1)), ==, 1);
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(uhci3));
    g_object_unref(G_OBJECT(uhci1));
    g_object_unref(G_OBJECT(uhci2));
    g_object_unref(G_OBJECT(uhci3));
    g_object_unref(G_OBJECT(ehci));


    /* three redirdev channels allows to redirect a maximum of 3 USB
     * devices at a time. The address which create_redirdev assigns to the
     * redirdev object is optional
     */
    redirdev = domain_device_usb_redir_create_redirdev(0, 3);
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(redirdev));
    g_object_unref(G_OBJECT(redirdev));
    redirdev = domain_device_usb_redir_create_redirdev(0, 4);
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(redirdev));
    g_object_unref(G_OBJECT(redirdev));
    redirdev = domain_device_usb_redir_create_redirdev(0, 5);
    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE(redirdev));
    g_object_unref(G_OBJECT(redirdev));

    check_xml(domain, "gconfig-domain-device-usbredir.xml");

    g_object_unref(G_OBJECT(domain));
}

static void test_domain_device_pci_hostdev(void)
{
    GVirConfigDomain *domain;
    GVirConfigDomainAddressPci *address;
    GVirConfigDomainHostdevPci *hostdev;

    domain = gvir_config_domain_new();

    hostdev = gvir_config_domain_hostdev_pci_new();
    gvir_config_domain_hostdev_set_boot_order(GVIR_CONFIG_DOMAIN_HOSTDEV(hostdev), 1);
    g_assert_cmpint(gvir_config_domain_hostdev_get_boot_order(GVIR_CONFIG_DOMAIN_HOSTDEV(hostdev)), ==, 1);
    gvir_config_domain_hostdev_pci_set_managed(hostdev, TRUE);
    g_assert(gvir_config_domain_hostdev_pci_get_managed(hostdev) == TRUE);
    gvir_config_domain_hostdev_pci_set_rom_bar(hostdev, TRUE);
    gvir_config_domain_hostdev_pci_set_rom_file(hostdev, "/etc/fake/boot.bin");
    g_assert_cmpstr(gvir_config_domain_hostdev_pci_get_rom_file(hostdev), ==, "/etc/fake/boot.bin");
    g_assert(gvir_config_domain_hostdev_pci_get_rom_bar(hostdev));

    address = gvir_config_domain_address_pci_new();
    gvir_config_domain_address_pci_set_domain(address, 1);
    gvir_config_domain_address_pci_set_bus(address, 2);
    gvir_config_domain_address_pci_set_slot(address, 3);
    gvir_config_domain_address_pci_set_function(address, 4);
    gvir_config_domain_hostdev_pci_set_address(hostdev, address);
    g_object_unref(G_OBJECT(address));

    address = gvir_config_domain_hostdev_pci_get_address(hostdev);
    g_assert(address != NULL);
    g_assert_cmpint(gvir_config_domain_address_pci_get_domain(address), ==, 1);
    g_assert_cmpint(gvir_config_domain_address_pci_get_bus(address), ==, 2);
    g_assert_cmpint(gvir_config_domain_address_pci_get_slot(address), ==, 3);
    g_assert_cmpint(gvir_config_domain_address_pci_get_function(address), ==, 4);
    g_object_unref(G_OBJECT(address));

    gvir_config_domain_add_device(domain, GVIR_CONFIG_DOMAIN_DEVICE (hostdev));
    g_object_unref(G_OBJECT(hostdev));

    check_xml(domain, "gconfig-domain-device-pci-hostdev.xml");

    g_object_unref(G_OBJECT(domain));
}

static void test_domain_device_unknown(void)
{
    GVirConfigDomain *domain;
    GList *devices;
    GError *error = NULL;
    char *xml;

    xml = load_xml("gconfig-domain-device-unknown.xml");

    domain = gvir_config_domain_new_from_xml(xml, &error);
    g_assert_no_error(error);

    devices = gvir_config_domain_get_devices(domain);
    g_assert_nonnull(devices);
    gvir_config_domain_set_devices(domain, devices);

    check_xml(domain, "gconfig-domain-device-unknown.xml");

    g_list_free_full(devices, g_object_unref);
    g_object_unref(G_OBJECT(domain));
    g_free(xml);
}

static void test_domain_capabilities_os(void)
{
    GVirConfigDomainCapabilities *domain_caps;
    GVirConfigDomainCapabilitiesOs *os;
    GList *firmwares, *l;
    gsize i;
    GVirConfigDomainOsFirmware expected_firmwares[] = {GVIR_CONFIG_DOMAIN_OS_FIRMWARE_BIOS,
                                                       GVIR_CONFIG_DOMAIN_OS_FIRMWARE_EFI};
    GError *error = NULL;
    gchar *xml;

    xml = load_xml("gconfig-domain-capabilities-os.xml");

    domain_caps =  gvir_config_domain_capabilities_new_from_xml(xml, &error);
    g_assert_no_error(error);

    os = gvir_config_domain_capabilities_get_os(domain_caps);
    g_assert_nonnull(os);

    firmwares = gvir_config_domain_capabilities_os_get_firmwares(os);
    g_assert_nonnull(firmwares);

    for (l = firmwares, i = 0; l != NULL; l = l->next, i++)
        g_assert_cmpint(GPOINTER_TO_INT(l->data), ==, expected_firmwares[i]);


    g_list_free(firmwares);
    g_object_unref(os);
    g_object_unref(domain_caps);
    g_free(xml);
}

int main(int argc, char **argv)
{
    gvir_config_init(&argc, &argv);
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/libvirt-gconfig/domain", test_domain);
    g_test_add_func("/libvirt-gconfig/domain-clock", test_domain_clock);
    g_test_add_func("/libvirt-gconfig/domain-os", test_domain_os);
    g_test_add_func("/libvirt-gconfig/domain-cpu", test_domain_cpu);
    g_test_add_func("/libvirt-gconfig/domain-device-disk",
                    test_domain_device_disk);
    g_test_add_func("/libvirt-gconfig/domain-device-filesystem",
                    test_domain_device_filesystem);
    g_test_add_func("/libvirt-gconfig/domain-device-network",
                    test_domain_device_network);
    g_test_add_func("/libvirt-gconfig/domain-device-input",
                    test_domain_device_input);
    g_test_add_func("/libvirt-gconfig/domain-device-graphics",
                    test_domain_device_graphics);
    g_test_add_func("/libvirt-gconfig/domain-device-video",
                    test_domain_device_video);
    g_test_add_func("/libvirt-gconfig/domain-device-sound",
                    test_domain_device_sound);
    g_test_add_func("/libvirt-gconfig/domain-device-console",
                    test_domain_device_console);
    g_test_add_func("/libvirt-gconfig/domain-device-channel",
                    test_domain_device_channel);
    g_test_add_func("/libvirt-gconfig/domain-device-usb-redir",
                    test_domain_device_usb_redir);
    g_test_add_func("/libvirt-gconfig/domain-device-pci-hostdev",
                    test_domain_device_pci_hostdev);
    g_test_add_func("/libvirt-gconfig/domain-device-unknown",
                    test_domain_device_unknown);
    g_test_add_func("/libvirt-gconfig/domain-capabilities-os",
                    test_domain_capabilities_os);

    return g_test_run();
}
