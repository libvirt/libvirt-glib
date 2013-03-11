/*
 * libvirt-gconfig-domain-smartcard-host-certificates.h: libvirt domain smartcard host-certificates configuration
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

#if !defined(__LIBVIRT_GCONFIG_H__) && !defined(LIBVIRT_GCONFIG_BUILD)
#error "Only <libvirt-gconfig/libvirt-gconfig.h> can be included directly."
#endif

#ifndef __LIBVIRT_GCONFIG_DOMAIN_SMARTCARD_HOST_CERTIFICATES_H__
#define __LIBVIRT_GCONFIG_DOMAIN_SMARTCARD_HOST_CERTIFICATES_H__

G_BEGIN_DECLS

#define GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST_CERTIFICATES            (gvir_config_domain_smartcard_host_certificates_get_type ())
#define GVIR_CONFIG_DOMAIN_SMARTCARD_HOST_CERTIFICATES(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST_CERTIFICATES, GVirConfigDomainSmartcardHostCertificates))
#define GVIR_CONFIG_DOMAIN_SMARTCARD_HOST_CERTIFICATES_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST_CERTIFICATES, GVirConfigDomainSmartcardHostCertificatesClass))
#define GVIR_CONFIG_IS_DOMAIN_SMARTCARD_HOST_CERTIFICATES(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST_CERTIFICATES))
#define GVIR_CONFIG_IS_DOMAIN_SMARTCARD_HOST_CERTIFICATES_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST_CERTIFICATES))
#define GVIR_CONFIG_DOMAIN_SMARTCARD_HOST_CERTIFICATES_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GVIR_CONFIG_TYPE_DOMAIN_SMARTCARD_HOST_CERTIFICATES, GVirConfigDomainSmartcardHostCertificatesClass))

typedef struct _GVirConfigDomainSmartcardHostCertificates GVirConfigDomainSmartcardHostCertificates;
typedef struct _GVirConfigDomainSmartcardHostCertificatesPrivate GVirConfigDomainSmartcardHostCertificatesPrivate;
typedef struct _GVirConfigDomainSmartcardHostCertificatesClass GVirConfigDomainSmartcardHostCertificatesClass;

struct _GVirConfigDomainSmartcardHostCertificates
{
    GVirConfigDomainSmartcard parent;

    GVirConfigDomainSmartcardHostCertificatesPrivate *priv;

    /* Do not add fields to this struct */
};

struct _GVirConfigDomainSmartcardHostCertificatesClass
{
    GVirConfigDomainSmartcardClass parent_class;

    gpointer padding[20];
};

GType gvir_config_domain_smartcard_host_certificates_get_type(void);

GVirConfigDomainSmartcardHostCertificates *gvir_config_domain_smartcard_host_certificates_new(void);
GVirConfigDomainSmartcardHostCertificates *gvir_config_domain_smartcard_host_certificates_new_from_xml(const gchar *xml,
                                                                                                       GError **error);

void gvir_config_domain_smartcard_host_certificates_set_database(GVirConfigDomainSmartcardHostCertificates *smartcard,
                                                                 const char *path);
void gvir_config_domain_smartcard_host_certificates_set_certificates(GVirConfigDomainSmartcardHostCertificates *smartcard,
                                                                     const char *cert1,
                                                                     const char *cert2,
                                                                     const char *cert3);

G_END_DECLS

#endif /* __LIBVIRT_GCONFIG_DOMAIN_SMARTCARD_HOST_CERTIFICATES_H__ */
