include $(top_srcdir)/build-aux/glib-tap.mk

AM_CFLAGS = \
	$(COVERAGE_CFLAGS) \
	-I$(top_srcdir) \
	$(GOBJECT2_CFLAGS) \
	$(LIBVIRT_CFLAGS) \
	$(LIBXML2_CFLAGS) \
	$(WARN_CFLAGS)

LDADD = \
	$(top_builddir)/libvirt-gconfig/libvirt-gconfig-1.0.la \
	$(top_builddir)/libvirt-glib/libvirt-glib-1.0.la

test_programs = test-gconfig test-events

dist_test_data = \
	xml/gconfig-domain.xml \
	xml/gconfig-domain-clock.xml \
	xml/gconfig-domain-os.xml
