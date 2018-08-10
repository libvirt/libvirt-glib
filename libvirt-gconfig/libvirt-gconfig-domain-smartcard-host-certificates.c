/*
 * libvirt-gconfig-domain-smartcard-host-certificates.c: libvirt domain smartcard host-certificates configuration
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
 * Author: Alexander Larsson <alexl@redhat.com>
 */

#include <config.h>

#include "libvirt-gconfig/libvirt-gconfig.h"
#include "libvirt-gconfig/libvirt-gconfig-private.h"

#define GVIR_CONFIG_DOMAIN_SMARTCARD_HOST_CERTIFICATES_GET_PRIVATE(obj)                         \
        (G_TYPE_INSTANCE_GET_PRIVATE((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST_CERTIFICATES, GVirConfigDomainSmartcardHostCertificatesPrivate))

struct _GVirConfigDomainSmartcardHostCertificatesPrivate
{
    gboolean unused;
};

G_DEFINE_TYPE_WITH_PRIVATE(GVirConfigDomainSmartcardHostCertificates, gvir_config_domain_smartcard_host_certificates, GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD);


static void gvir_config_domain_smartcard_host_certificates_class_init(GVirConfigDomainSmartcardHostCertificatesClass *klass G_GNUC_UNUSED)
{
}


static void gvir_config_domain_smartcard_host_certificates_init(GVirConfigDomainSmartcardHostCertificates *smartcard)
{
    smartcard->priv = GVIR_CONFIG_DOMAIN_SMARTCARD_HOST_CERTIFICATES_GET_PRIVATE(smartcard);
}


GVirConfigDomainSmartcardHostCertificates *gvir_config_domain_smartcard_host_certificates_new(void)
{
    GVirConfigObject *object;

    object = gvir_config_object_new(GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST_CERTIFICATES,
                                    "smartcard", NULL);
    gvir_config_object_set_attribute(object, "mode", "host-certificates", NULL);
    return GVIR_CONFIG_DOMAIN_SMARTCARD_HOST_CERTIFICATES(object);
}

GVirConfigDomainSmartcardHostCertificates *gvir_config_domain_smartcard_host_certificates_new_from_xml(const gchar *xml,
                                                                              GError **error)
{
    GVirConfigObject *object;

    object = gvir_config_object_new_from_xml(GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST_CERTIFICATES,
                                             "smartcard", NULL, xml, error);
    if (g_strcmp0(gvir_config_object_get_attribute(object, NULL, "mode"), "host-certificates") != 0) {
        g_object_unref(G_OBJECT(object));
        return NULL;
    }
    return GVIR_CONFIG_DOMAIN_SMARTCARD_HOST_CERTIFICATES(object);
}

/**
 * gvir_config_domain_smartcard_host_certificates_set_database:
 * @path: (allow-none):
 */
void
gvir_config_domain_smartcard_host_certificates_set_database (GVirConfigDomainSmartcardHostCertificates *smartcard,
                                                             const char *path)
{
    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SMARTCARD_HOST_CERTIFICATES(smartcard));

    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(smartcard),
                                        "database", path);
}

/**
 * gvir_config_domain_smartcard_host_certificates_set_certificates:
 * @cert1: (allow-none):
 * @cert2: (allow-none):
 * @cert3: (allow-none):
 */
void
gvir_config_domain_smartcard_host_certificates_set_certificates (GVirConfigDomainSmartcardHostCertificates *smartcard,
                                                                 const char *cert1,
                                                                 const char *cert2,
                                                                 const char *cert3)
{
    GVirConfigObject *child;

    g_return_if_fail(GVIR_CONFIG_IS_DOMAIN_SMARTCARD_HOST_CERTIFICATES(smartcard));

    gvir_config_object_delete_children(GVIR_CONFIG_OBJECT(smartcard),
                                       "certificate", NULL);

    if (cert1 == NULL)
        return;

    child = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(smartcard), "certificate");
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(smartcard), NULL, cert1);
    g_object_unref(G_OBJECT(child));

    child = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(smartcard), "certificate");
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(smartcard), NULL, cert2);
    g_object_unref(G_OBJECT(child));

    child = gvir_config_object_add_child(GVIR_CONFIG_OBJECT(smartcard), "certificate");
    gvir_config_object_set_node_content(GVIR_CONFIG_OBJECT(smartcard), NULL, cert3);
    g_object_unref(G_OBJECT(child));
}
