if ENABLE_TESTS

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
	$(top_builddir)/libvirt-glib/libvirt-glib-1.0.la \
	$(LIBVIRT_LIBS) \
	$(GLIB2_LIBS) \
	$(GOBJECT2_LIBS)

test_programs = test-gconfig test-events

EXTRA_DIST += \
	xml \
	$(NULL)
endif
