/*
 * test-capabilities-parse.c: test libvirt-gconfig capabilities parsing
 *
 * Copyright (C) 2011-2012 Red Hat, Inc.
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
 * Authors: Zeeshan Ali <zeenix@redhat.com>
 *          Christophe Fergeau <cfergeau@redhat.com>
 */

#include <config.h>

#include <string.h>
#include <libvirt-gconfig/libvirt-gconfig.h>

static void verify_host_caps(GVirConfigCapabilitiesHost *host_caps)
{
    GVirConfigCapabilitiesCpu *cpu_caps;
    GVirConfigCapabilitiesCpuTopology *topology;
    GList *features, *iter, *secmodels;
    const char *str;

    g_assert(host_caps != NULL);
    str = gvir_config_capabilities_host_get_uuid(host_caps);
    g_assert(g_strcmp0(str, "cd6a24b3-46f8-01aa-bb39-c39aa2123730") == 0);
    cpu_caps = gvir_config_capabilities_host_get_cpu(host_caps);
    g_assert(cpu_caps != NULL);
    str = gvir_config_capabilities_cpu_get_arch(cpu_caps);
    g_assert(g_strcmp0(str, "x86_64") == 0);

    features = gvir_config_capabilities_cpu_get_features(cpu_caps);
    for (iter = features; iter != NULL; iter = iter->next) {
        g_assert(iter->data != NULL);
        g_object_unref(G_OBJECT(iter->data));
    }
    g_list_free(features);

    topology = gvir_config_capabilities_cpu_get_topology(cpu_caps);
    g_assert(topology != NULL);
    g_assert(gvir_config_capabilities_cpu_topology_get_sockets(topology) == 1);
    g_assert(gvir_config_capabilities_cpu_topology_get_cores(topology) == 2);
    g_assert(gvir_config_capabilities_cpu_topology_get_threads(topology) == 2);
    g_object_unref(G_OBJECT(topology));
    g_object_unref(G_OBJECT(cpu_caps));

    secmodels = gvir_config_capabilities_host_get_secmodels(host_caps);
    g_assert(g_list_length(secmodels) == 2);
    for (iter = secmodels; iter != NULL; iter = iter->next) {
        GVirConfigCapabilitiesHostSecModel *secmodel;

        g_assert(iter->data != NULL);
        secmodel = GVIR_CONFIG_CAPABILITIES_HOST_SECMODEL(iter->data);
        g_assert(gvir_config_capabilities_host_secmodel_get_model(secmodel) != NULL);
        g_assert(gvir_config_capabilities_host_secmodel_get_doi(secmodel) != NULL);
        g_object_unref(G_OBJECT(iter->data));
    }
    g_list_free(secmodels);
}

static void verify_guest_caps(GVirConfigCapabilitiesGuest *guest_caps)
{
    GVirConfigCapabilitiesGuestArch *arch_caps;
    GList *features, *domains, *iter;
    const char *str;

    g_assert(guest_caps != NULL);
    g_assert(gvir_config_capabilities_guest_get_os_type(guest_caps) ==
             GVIR_CONFIG_DOMAIN_OS_TYPE_HVM);

    features = gvir_config_capabilities_guest_get_features(guest_caps);
    for (iter = features; iter != NULL; iter = iter->next) {
        GVirConfigCapabilitiesGuestFeature *feature_caps;

        feature_caps = GVIR_CONFIG_CAPABILITIES_GUEST_FEATURE(iter->data);
        g_assert(feature_caps != NULL);
        str = gvir_config_capabilities_guest_feature_get_name(feature_caps);
        g_assert(str != NULL);
        g_object_unref(G_OBJECT(feature_caps));
    }
    g_list_free(features);

    arch_caps = gvir_config_capabilities_guest_get_arch(guest_caps);
    g_assert(arch_caps != NULL);
    str = gvir_config_capabilities_guest_arch_get_name(arch_caps);
    g_assert(str != NULL);
    str = gvir_config_capabilities_guest_arch_get_emulator(arch_caps);
    g_assert(str != NULL);

    domains = gvir_config_capabilities_guest_arch_get_domains(arch_caps);
    for (iter = domains; iter != NULL; iter = iter->next) {
        GVirConfigCapabilitiesGuestDomain *domain_caps;
        GVirConfigDomainVirtType virt_type;

        domain_caps = GVIR_CONFIG_CAPABILITIES_GUEST_DOMAIN(iter->data);
        g_assert(domain_caps != NULL);
        virt_type = gvir_config_capabilities_guest_domain_get_virt_type(domain_caps);
        str = gvir_config_capabilities_guest_domain_get_emulator(domain_caps);
        g_assert((virt_type == GVIR_CONFIG_DOMAIN_VIRT_QEMU && str == NULL) ||
                 (virt_type == GVIR_CONFIG_DOMAIN_VIRT_KVM &&
                  g_strcmp0(str, "/usr/bin/qemu-kvm") == 0));
        g_object_unref(G_OBJECT(domain_caps));
    }
    g_list_free(domains);
    g_object_unref(G_OBJECT(arch_caps));
}

int main(int argc, char **argv)
{
    GVirConfigCapabilities *caps;
    GVirConfigCapabilitiesHost *host_caps;
    GList *guests_caps, *iter;
    char *xml;
    GError *error = NULL;

    gvir_config_init(&argc, &argv);
    if (argc != 2) {
        g_print("Usage: %s filename\n", argv[0]);
        g_print("Attempt to parse the libvirt XML definition from filename\n");
        return 1;
    }

    g_file_get_contents(argv[1], &xml, NULL, &error);
    if (error != NULL) {
        g_print("Couldn't read %s: %s\n", argv[1], error->message);
        return 2;
    }

    caps = gvir_config_capabilities_new_from_xml(xml, &error);
    g_free(xml);
    if (error != NULL) {
        g_print("Couldn't parse %s: %s\n", argv[1], error->message);
        return 3;
    }
    g_assert(caps != NULL);
    gvir_config_object_validate(GVIR_CONFIG_OBJECT(caps), &error);
    if (error != NULL) {
        g_print("%s format is invalid: %s\n", argv[1], error->message);
        g_clear_error(&error);
    }

    host_caps = gvir_config_capabilities_get_host(caps);
    verify_host_caps(host_caps);
    g_object_unref(G_OBJECT(host_caps));

    guests_caps = gvir_config_capabilities_get_guests(caps);
    for (iter = guests_caps; iter != NULL; iter = iter->next) {
        GVirConfigCapabilitiesGuest *guest_caps;

        guest_caps = GVIR_CONFIG_CAPABILITIES_GUEST(iter->data);
        verify_guest_caps(guest_caps);
        g_object_unref(G_OBJECT(guest_caps));
    }
    g_list_free(guests_caps);

    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(caps));
    g_print("%s\n", xml);
    g_free(xml);
    g_object_unref(G_OBJECT(caps));

    return 0;
}
