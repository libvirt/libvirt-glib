/*
 * test-domain-create.c: test libvirt-gconfig domain parsing
 *
 * Copyright (C) 2011 Red Hat, Inc.
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

#include <config.h>

#include <string.h>
#include <libvirt-gconfig/libvirt-gconfig.h>


int main(int argc, char **argv)
{
    GVirConfigDomain *domain;
    const char *name;
    GStrv features;
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

    domain = gvir_config_domain_new_from_xml(xml, &error);
    if (error != NULL) {
        g_print("Couldn't parse %s: %s\n", argv[1], error->message);
        return 3;
    }
    g_assert(domain != NULL);
    gvir_config_object_validate(GVIR_CONFIG_OBJECT(domain), &error);
    if (error != NULL) {
        g_print("%s format is invalid: %s\n", argv[1], error->message);
        g_clear_error(&error);
    }

    name = gvir_config_domain_get_name(domain);
    g_assert(name != NULL);
    g_assert(strcmp(name, "foo") == 0);

    g_assert(gvir_config_domain_get_memory(domain) == 987654321);

    features = gvir_config_domain_get_features(domain);
    g_assert(g_strv_length(features) == 2);
    g_assert(strcmp(features[0], "f1") == 0);
    g_assert(strcmp(features[1], "f2") == 0);
    g_strfreev(features);

    g_free(xml);

    xml = gvir_config_object_to_xml(GVIR_CONFIG_OBJECT(domain));
    g_print("%s\n", xml);
    g_free(xml);
    g_object_unref(G_OBJECT(domain));

    return 0;
}
