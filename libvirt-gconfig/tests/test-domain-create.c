/*
 * test-domain-create.c: test libvirt-gconfig domain creation
 *
 * Copyright (C) 2011 Red Hat
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * The Software is provided "as is", without warranty of any kind, express
 * or implied, including but not limited to the warranties of
 * merchantability, fitness for a particular purpose and noninfringement.
 * In no event shall the authors or copyright holders be liable for any
 * claim, damages or other liability, whether in an action of contract,
 * tort or otherwise, arising from, out of or in connection with the
 * software or the use or other dealings in the Software.
 *
 * Author: Christophe Fergeau <cfergeau@redhat.com>
 */

#include <string.h>
#include <libvirt-gconfig/libvirt-gconfig.h>

const char *features[] = { "foo", "bar", "baz", NULL };

int main(void)
{
    GVirConfigDomain *domain;
    char *name;
    GStrv feat;
    unsigned int i;
    char *xml;

    g_type_init();

    domain = gvir_config_domain_new();
    g_assert(domain != NULL);
    gvir_config_domain_set_name(domain, "foo");
    name = gvir_config_domain_get_name(domain);
    g_assert(name != NULL);
    g_assert(strcmp(name, "foo") == 0);
    g_free(name);

    gvir_config_domain_set_memory(domain, 1234);
    gvir_config_domain_set_vcpus(domain, 3);
    g_assert(gvir_config_domain_get_memory(domain) == 1234);

    gvir_config_domain_set_features(domain, (const GStrv)features);
    feat = gvir_config_domain_get_features(domain);
    for (i = 0; features[i] != NULL; i++) {
        g_assert(feat[i] != NULL);
        g_assert(strcmp(feat[i], features[i]) == 0);
    }
    g_strfreev(feat);

    /* clock node */
    GVirConfigDomainClock *klock;

    klock = gvir_config_domain_clock_new();
    gvir_config_domain_clock_set_offset(klock, GVIR_CONFIG_DOMAIN_CLOCK_UTC);
    gvir_config_domain_set_clock(domain, klock);

    /* os node */
    GVirConfigDomainOs *os;
    GList *devices = NULL;

    os = gvir_config_domain_os_new();
    gvir_config_domain_os_set_os_type(os, GVIR_CONFIG_DOMAIN_OS_TYPE_HVM);
    gvir_config_domain_os_set_arch(os, "x86_64");
    devices = g_list_append(devices,
                             GINT_TO_POINTER(GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_CDROM));
    devices = g_list_append(devices,
                            GINT_TO_POINTER(GVIR_CONFIG_DOMAIN_OS_BOOT_DEVICE_NETWORK));
    gvir_config_domain_os_set_boot_devices(os, devices);
    g_list_free(devices);
    devices = NULL;
    gvir_config_domain_set_os(domain, os);

    /* disk node */
    GVirConfigDomainDisk *disk;

    disk = gvir_config_domain_disk_new();
    gvir_config_domain_disk_set_type(disk, GVIR_CONFIG_DOMAIN_DISK_FILE);
    gvir_config_domain_disk_set_guest_device_type(disk, GVIR_CONFIG_DOMAIN_DISK_GUEST_DEVICE_DISK);
    gvir_config_domain_disk_set_source(disk, "/tmp/foo/bar");
    gvir_config_domain_disk_set_driver_name(disk, "qemu");
    gvir_config_domain_disk_set_driver_type(disk, "qcow2");
    gvir_config_domain_disk_set_target_bus(disk, "ide");
    gvir_config_domain_disk_set_target_dev(disk, "hda");
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(disk));


    /* network interface node */
    GVirConfigDomainInterfaceNetwork *interface;

    interface = gvir_config_domain_interface_network_new();
    gvir_config_domain_interface_network_set_source(interface, "default");
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(interface));

    /* input node */
    GVirConfigDomainInput *input;

    input = gvir_config_domain_input_new();
    gvir_config_domain_input_set_device_type(input,
                                             GVIR_CONFIG_DOMAIN_INPUT_DEVICE_TABLET);
    gvir_config_domain_input_set_bus(input, GVIR_CONFIG_DOMAIN_INPUT_BUS_USB);
    devices = g_list_append(devices, GVIR_CONFIG_DOMAIN_DEVICE(input));


    gvir_config_domain_set_devices(domain, devices);
    g_list_free(devices);
    devices = NULL;


    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(domain));
    g_print("%s\n", xml);
    g_free(xml);
    g_object_unref(G_OBJECT(domain));

    return 0;
}
