/*
 * libvirt-gconfig-domain.c: libvirt domain configuration
 *
 * Copyright (C) 2008 Daniel P. Berrange
 * Copyright (C) 2010-2011 Red Hat, Inc.
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN, GVirConfigDomainPrivate))

struct _GVirConfigDomainPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE(GVirConfigDomain, gvir_config_domain, GVIR_CONFIG_TYPE_OBJECT);

enum {
    PROP_0,
    PROP_NAME,
    PROP_TITLE,
    PROP_DESCRIPTION,
    PROP_MEMORY,
    PROP_VCPU,
    PROP_FEATURES
};

static void gvir_config_domain_get_property(GObject *object,
                                            guint prop_id,
                                            GValue *value,
                                            GParamSpec *pspec)
{
    GVirConfigDomain *domain = GVIR_CONFIG_DOMAIN(object);

    switch (prop_id) {
    case PROP_NAME:
        g_value_set_string(value, gvir_config_domain_get_name(domain));
        break;
    case PROP_TITLE:
        g_value_set_string(value, gvir_config_domain_get_title(domain));
        break;
    case PROP_DESCRIPTION:
        g_value_set_string(value, gvir_config_domain_get_description(domain));
        break;
    case PROP_MEMORY:
        g_value_set_uint64(value, gvir_config_domain_get_memory(domain));
        break;
    case PROP_VCPU:
        g_value_set_uint64(value, gvir_config_domain_get_vcpus(domain));
        break;
    case PROP_FEATURES:
        g_value_take_boxed(value, gvir_config_domain_get_features(domain));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}

static void gvir_config_domain_set_property(GObject *object,
                                            guint prop_id,
                                            const GValue *value,
                                            GParamSpec *pspec)
{
    GVirConfigDomain *domain = GVIR_CONFIG_DOMAIN(object);

    switch (prop_id) {
    case PROP_NAME:
        gvir_config_domain_set_name(domain, g_value_get_string(value));
        break;
    case PROP_TITLE:
        gvir_config_domain_set_title(domain, g_value_get_string(value));
        break;
    case PROP_DESCRIPTION:
        gvir_config_domain_set_description(domain, g_value_get_string(value));
        break;
    case PROP_MEMORY:
        gvir_config_domain_set_memory(domain, g_value_get_uint64(value));
        break;
    case PROP_VCPU:
        gvir_config_domain_set_vcpus(domain, g_value_get_uint64(value));
        break;
    case PROP_FEATURES:
        gvir_config_domain_set_features(domain, g_value_get_boxed(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}


static void gvir_config_domain_class_init(GVirConfigDomainClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    g_type_class_add_private(klass, sizeof(GVirConfigDomainPrivate));

    object_class->get_property = gvir_config_domain_get_property;
    object_class->set_property = gvir_config_domain_set_property;

    g_object_class_install_property(object_class,
                                    PROP_NAME,
                                    g_param_spec_string("name",
                                                        "Name",
                                                        "Domain Name",
                                                        NULL,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_STATIC_STRINGS));
    g_object_class_install_property(object_class,
                                    PROP_TITLE,
                                    g_param_spec_string("title",
                                                        "Title",
                                                        "A short description - title - of the domain",
                                                        NULL,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_STATIC_STRINGS));
    g_object_class_install_property(object_class,
                                    PROP_DESCRIPTION,
                                    g_param_spec_string("description",
                                                        "Description",
                                                        "Some human readable description (could be anything).",
                                                        NULL,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_STATIC_STRINGS));
    g_object_class_install_property(object_class,
                                    PROP_MEMORY,
                                    g_param_spec_uint64("memory",
                                                        "Memory",
                                                        "Maximum Guest Memory (in kilobytes)",
                                                        0, G_MAXUINT64,
                                                        0,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_STATIC_STRINGS));
    g_object_class_install_property(object_class,
                                    PROP_VCPU,
                                    g_param_spec_uint64("vcpu",
                                                        "Virtual CPUs",
                                                        "Maximum Number of Guest Virtual CPUs",
                                                        0, G_MAXUINT64,
                                                        1,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_STATIC_STRINGS));
    g_object_class_install_property(object_class,
                                    PROP_FEATURES,
                                    g_param_spec_boxed("features",
                                                        "Features",
                                                        "Hypervisor Features",
                                                        G_TYPE_STRV,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_STATIC_STRINGS));
}


static void gvir_config_domain_init(GVirConfigDomain *conn)
{
    g_debug("Init GVirConfigDomain=%p", conn);

    conn->priv = GVIR_CONFIG_DOMAIN_GET_PRIVATE(conn);
}


GVirConfigDomain *gvir_config_domain_new_from_xml(const gchar *xml,
                                                  GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN,
                                             "domain",
                                             DATADIR "/libvirt/schemas/domain.rng",
                                             xml, error);
    return GVIR_CONFIG_DOMAIN(object);
}

GVirConfigDomain *gvir_config_domain_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN,
                                    "domain",
                                    DATADIR "/libvirt/schemas/domain.rng");
    return GVIR_CONFIG_DOMAIN(object);
}

GVirConfigDomainVirtType gvir_config_domain_get_virt_type(GVirConfigDomain *domain)
{
    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN(domain),
                         GVIR_CONFIG_DOMAIN_VIRT_QEMU);

    return gvir_config_object_get_attribute_genum
                                (GVIR_CONFIG_OBJECT(domain),
                                 NULL,
                                 "type",
                                 GVIR_CONFIG_TYPE_DOMAIN_VIRT_TYPE,
                                 GVIR_CONFIG_DOMAIN_VIRT_QEMU);
}

void gvir_config_domain_set_virt_type(GVirConfigDomain *domain, GVirConfigDomainVirtType type)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN(domain));
    gvir_config_object_set_attribute_with_type(GVIR_CONFIG_OBJECT(domain),
                                               "type",
                                               GVIR_CONFIG_TYPE_DOMAIN_VIRT_TYPE,
                                               type, NULL);
}


const char *gvir_config_domain_get_name(GVirConfigDomain *domain)
{
    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(domain),
                                               "name");
}

const char *gvir_config_domain_get_title(GVirConfigDomain *domain)
{
    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(domain),
                                               "title");
}

/**
 * gvir_config_domain_set_name:
 * @domain: a #GVirConfigDomain
 * @name: (allow-none):
 */
void gvir_config_domain_set_name(GVirConfigDomain *domain, const char *name)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(domain),
                                        "name", name);
    g_object_notify(G_OBJECT(domain), "name");
}

/**
 * gvir_config_domain_set_title:
 * @domain: a #GVirConfigDomain
 * @title: (allow-none): title of the domain
 *
 * Sets the title of the domain. This is an optional short textual description of the domain. Passing a NULL @title
 * unsets the current domain title.
 */
void gvir_config_domain_set_title(GVirConfigDomain *domain, const char *title)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(domain),
                                        "title", title);
    g_object_notify(G_OBJECT(domain), "title");
}

const char *gvir_config_domain_get_description(GVirConfigDomain *domain)
{
    return gvir_config_object_get_node_content(GVIR_CONFIG_OBJECT(domain),
                                               "description");
}

/**
 * gvir_config_domain_set_description:
 * @domain: a #GVirConfigDomain
 * @description: (allow-none):
 */
void gvir_config_domain_set_description(GVirConfigDomain *domain, const char *description)
{
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(domain),
                                        "description", description);
    g_object_notify(G_OBJECT(domain), "description");
}

/**
 * gvir_config_domain_get_memory:
 * @domain: a #GVirConfigDomain
 *
 * Returns: amount of RAM in kilobytes (i.e. blocks of 1024 bytes).
 */
guint64 gvir_config_domain_get_memory(GVirConfigDomain *domain)
{
    return gvir_config_object_get_node_content_uint64(GVIR_CONFIG_OBJECT(domain),
                                                      "memory");
}

/**
 * gvir_config_domain_set_memory:
 * @domain: a #GVirConfigDomain
 * @memory: The amount of RAM in kilobytes.
 *
 * Sets the amount of RAM allocated to @domain in kilobytes (i.e. blocks of 1024 bytes).
 */
void gvir_config_domain_set_memory(GVirConfigDomain *domain, guint64 memory)
{
    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(domain),
                                               "memory", memory);
    g_object_notify(G_OBJECT(domain), "memory");
}

guint64 gvir_config_domain_get_vcpus(GVirConfigDomain *domain)
{
    return gvir_config_object_get_node_content_uint64(GVIR_CONFIG_OBJECT(domain),
                                                      "vcpu");
}

void gvir_config_domain_set_vcpus(GVirConfigDomain *domain, guint64 vcpu_count)
{
    gvir_config_object_set_node_content_uint64(GVIR_CONFIG_OBJECT(domain),
                                               "vcpu", vcpu_count);
    g_object_notify(G_OBJECT(domain), "vcpu");
}

static gboolean add_feature(xmlNodePtr node, gpointer opaque)
{
    GPtrArray *features;
    g_return_val_if_fail(opaque != NULL, FALSE);

    features = (GPtrArray *)opaque;

    g_ptr_array_add(features, g_strdup((char *)node->name));

    return TRUE;
}

/**
 * gvir_config_domain_get_features:
 * @domain: a #GVirConfigDomain
 *
 * Returns: (transfer full): The returned list should be freed with
 * g_strfreev() when no longer needed.

 */
GStrv gvir_config_domain_get_features(GVirConfigDomain *domain)
{
    GPtrArray *features;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN(domain), NULL);

    features = g_ptr_array_new();
    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(domain), "features",
                                     add_feature, features);
    g_ptr_array_add(features, NULL);

    return (GStrv)g_ptr_array_free(features, FALSE);
}

void gvir_config_domain_set_features(GVirConfigDomain *domain,
                                     const GStrv features)
{
    GVirConfigObject *features_node;
    GStrv it;

    features_node = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(domain),
                                                     "features");
    g_return_if_fail(GVIR_CONFIG_IS_OBJECT(features_node));
    for (it = features; *it != NULL; it++) {
        GVirConfigObject *feature;
        feature = gvir_config_object_replace_child(GVIR_CONFIG_OBJECT(features_node),
                                                   *it);
        g_object_unref(G_OBJECT(feature));
    }
    g_object_unref(G_OBJECT(features_node));
    g_object_notify(G_OBJECT(domain), "features");
}

/**
 * gvir_config_domain_set_clock:
 * @domain: a #GVirConfigDomain
 * @klock: (allow-none):
 */
void gvir_config_domain_set_clock(GVirConfigDomain *domain,
                                  GVirConfigDomainClock *klock)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN(domain));
    g_return_if_fail(klock != NULL || GVIR_CONFIG_IS_DOMAIN_CLOCK(klock));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(domain),
                                      "clock",
                                      GVIR_CONFIG_OBJECT(klock));
}

/**
 * gvir_config_domain_get_os:
 * @domain: a #GVirConfigDomain
 *
 * Gets the operating system configuration of @domain
 *
 * Returns: (transfer full): A #GVirConfigDomainOs. The returned
 * object should be unreffed with g_object_unref() when no longer needed.
 */
GVirConfigDomainOs *gvir_config_domain_get_os(GVirConfigDomain *domain)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN(domain), NULL);

    object = gvir_config_object_get_child_with_type(GVIR_CONFIG_OBJECT(domain),
                                                    "os",
                                                    GVIR_CONFIG_TYPE_DOMAIN_OS);

    return GVIR_CONFIG_DOMAIN_OS(object);
}

/**
 * gvir_config_domain_set_os:
 * @domain: a #GVirConfigDomain
 * @os: (allow-none): the os configuration to set
 */
void gvir_config_domain_set_os(GVirConfigDomain *domain,
                               GVirConfigDomainOs *os)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN(domain));
    g_return_if_fail(os == NULL || GVIR_CONFIG_IS_DOMAIN_OS(os));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(domain),
                                      "os",
                                      GVIR_CONFIG_OBJECT(os));
}

/**
 * gvir_config_domain_set_seclabel:
 * @domain: a #GVirConfigDomain
 * @seclabel: (allow-none): the security label configuration to set
 */
void gvir_config_domain_set_seclabel(GVirConfigDomain *domain,
                                     GVirConfigDomainSeclabel *seclabel)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN(domain));
    g_return_if_fail(seclabel == NULL ||
                     GVIR_CONFIG_IS_DOMAIN_SECLABEL(seclabel));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(domain),
                                      "seclabel",
                                      GVIR_CONFIG_OBJECT(seclabel));
}

void gvir_config_domain_set_lifecycle(GVirConfigDomain *domain,
                                      GVirConfigDomainLifecycleEvent event,
                                      GVirConfigDomainLifecycleAction action)
{
    const char *event_str;
    const char *action_str;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN(domain));
    g_return_if_fail((event == GVIR_CONFIG_DOMAIN_LIFECYCLE_ON_CRASH) ||
                     ((action != GVIR_CONFIG_DOMAIN_LIFECYCLE_COREDUMP_DESTROY) &&
                      (action != GVIR_CONFIG_DOMAIN_LIFECYCLE_COREDUMP_RESTART)));

    event_str = gvir_config_genum_get_nick(GVIR_CONFIG_TYPE_DOMAIN_LIFECYCLE_EVENT, event);
    g_return_if_fail(event_str != NULL);
    action_str = gvir_config_genum_get_nick(GVIR_CONFIG_TYPE_DOMAIN_LIFECYCLE_ACTION, action);
    g_return_if_fail(action_str != NULL);

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(domain),
                                        event_str, action_str);
}

/**
 * gvir_config_domain_set_devices:
 * @domain: a #GVirConfigDomain
 * @devices: (in) (element-type LibvirtGConfig.DomainDevice):
 */
void gvir_config_domain_set_devices(GVirConfigDomain *domain,
                                    GList *devices)
{
    GVirConfigObject *devices_node;
    GList *it;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN(domain));

    if (devices == NULL) {
        gvir_config_object_delete_children(GVIR_CONFIG_OBJECT(domain),
                                           "devices",
                                           NULL);
        return;
    }
    devices_node = gvir_config_object_new(GVIR_CONFIG_TYPE_OBJECT,
                                          "devices", NULL);

    for (it = devices; it != NULL; it = it->next) {
        if (!GVIR_CONFIG_IS_DOMAIN_DEVICE(it->data)) {
            g_warn_if_reached();
            continue;
        }
        gvir_config_object_attach_add(devices_node,
                                      GVIR_CONFIG_OBJECT(it->data));
    }

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(domain),
                                      "devices",
                                      devices_node);
    g_object_unref(G_OBJECT(devices_node));
}

void gvir_config_domain_add_device(GVirConfigDomain *domain,
                                   GVirConfigDomainDevice *device)
{
    GVirConfigObject *devices_node;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN(domain));
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_DEVICE(device));

    devices_node = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(domain),
                                                "devices");

    gvir_config_object_attach_add(devices_node, GVIR_CONFIG_OBJECT(device));
    g_object_unref(G_OBJECT(devices_node));
}

struct GetDeviceData {
    GVirConfigXmlDoc *doc;
    GList *devices;
};

static gboolean add_device(xmlNodePtr node, gpointer opaque)
{
    struct GetDeviceData* data = (struct GetDeviceData*)opaque;
    GVirConfigDomainDevice *device;

    device = gvir_config_domain_device_new_from_tree(data->doc, node);
    if (device != NULL)
        data->devices = g_list_append(data->devices, device);
    else
        g_debug("Failed to parse %s node", node->name);

    return TRUE;
}

/**
 * gvir_config_domain_get_devices:
 * @domain: a #GVirConfigDomain
 *
 * Gets the list of devices attached to @domain.  The returned list should
 * be freed with g_list_free(), after its elements have been unreffed with
 * g_object_unref().
 *
 * Returns: (element-type LibvirtGConfig.DomainDevice) (transfer full):
 * a newly allocated #GList of #GVirConfigDomainDevice.
 */
GList *gvir_config_domain_get_devices(GVirConfigDomain *domain)
{
    struct GetDeviceData data;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN(domain), NULL);

    g_object_get(G_OBJECT(domain), "doc", &data.doc, NULL);
    data.devices = NULL;
    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(domain), "devices",
                                     add_device, &data);
    if (data.doc != NULL) {
        g_object_unref(G_OBJECT(data.doc));
    }

    return data.devices;
}

gboolean gvir_config_domain_set_custom_xml(GVirConfigDomain *domain,
                                           const gchar *xml,
                                           const gchar *ns,
                                           const gchar *ns_uri,
                                           GError **error)
{
    GVirConfigObject *metadata;
    GVirConfigObject *custom_xml;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN(domain), FALSE);
    g_return_val_if_fail(xml != NULL, FALSE);
    g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

    metadata = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(domain),
                                            "metadata");

    custom_xml = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_OBJECT,
                                                 NULL, NULL, xml, error);
    if (custom_xml == NULL) {
        g_assert_not_reached();
        g_object_unref(G_OBJECT(metadata));
        return FALSE;
    }

    gvir_config_object_set_namespace(custom_xml, ns, ns_uri);

    gvir_config_object_delete_children(metadata, NULL, ns_uri);
    gvir_config_object_attach_add(metadata, custom_xml);
    g_object_unref(G_OBJECT(metadata));
    g_object_unref(G_OBJECT(custom_xml));

    return TRUE;
}

struct LookupNamespacedNodeData {
    const char *ns_uri;
    xmlNodePtr node;
};

static gboolean lookup_namespaced_node(xmlNodePtr node, gpointer opaque)
{
    struct LookupNamespacedNodeData* data = opaque;

    if (node->ns == NULL)
        return TRUE;

    if (g_strcmp0((char *)node->ns->href, data->ns_uri) == 0) {
        data->node = node;
        return FALSE;
    }

    return TRUE;
}

gchar *gvir_config_domain_get_custom_xml(GVirConfigDomain *domain,
                                         const gchar *ns_uri)
{
    struct LookupNamespacedNodeData data = { NULL, NULL };

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN(domain), NULL);
    g_return_val_if_fail(ns_uri != NULL, NULL);

    data.ns_uri = ns_uri;
    gvir_config_object_foreach_child(GVIR_CONFIG_OBJECT(domain), "metadata",
                                     lookup_namespaced_node, &data);
    return gvir_config_xml_node_to_string(data.node);
}

/**
 * gvir_config_domain_get_cpu:
 * @domain: a #GVirConfigDomain
 *
 * Gets the CPU configuration of @domain
 *
 * Returns: (transfer full): A #GVirConfigDomainCpu. The returned object
 * should be unreffed with g_object_unref() when no longer needed.
 */
GVirConfigDomainCpu *gvir_config_domain_get_cpu(GVirConfigDomain *domain)
{
    GVirConfigObject *object;

    g_return_val_if_fail(GVIR_CONFIG_IS_DOMAIN(domain), NULL);

    object = gvir_config_object_get_child_with_type(GVIR_CONFIG_OBJECT(domain),
                                                    "cpu",
                                                    GVIR_CONFIG_TYPE_DOMAIN_CPU);

    return GVIR_CONFIG_DOMAIN_CPU(object);
}

/**
 * gvir_config_domain_set_cpu:
 * @domain: a #GVirConfigDomain
 * @cpu: (allow-none):
 */
void gvir_config_domain_set_cpu(GVirConfigDomain *domain,
                                GVirConfigDomainCpu *cpu)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN(domain));
    g_return_if_fail(cpu != NULL || GVIR_CONFIG_IS_DOMAIN_CPU(cpu));

    gvir_config_object_attach_replace(GVIR_CONFIG_OBJECT(domain),
                                      "cpu",
                                      GVIR_CONFIG_OBJECT(cpu));
}
