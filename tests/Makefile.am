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
	$(top_builddir)/libvirt-glib/libvirt-glib-1.0.la

test_programs = test-gconfig test-events

EXTRA_DIST += \
	xml/gconfig-domain.xml \
	xml/gconfig-domain-clock.xml \
	xml/gconfig-domain-cpu.xml \
	xml/gconfig-domain-device-channel.xml \
	xml/gconfig-domain-device-console.xml \
	xml/gconfig-domain-device-disk.xml \
	xml/gconfig-domain-device-graphics.xml \
	xml/gconfig-domain-device-input.xml \
	xml/gconfig-domain-device-network.xml \
	xml/gconfig-domain-device-sound.xml \
	xml/gconfig-domain-device-usbredir.xml \
	xml/gconfig-domain-device-video.xml \
	xml/gconfig-domain-os.xml \
	$(NULL)
endif
