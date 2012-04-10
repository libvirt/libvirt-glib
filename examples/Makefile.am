
noinst_PROGRAMS = event-test conn-test

EXTRA_DIST = event-test.py config-demo.py

event_test_CFLAGS = \
		$(WARN_CFLAGS) \
		$(GLIB2_CFLAGS) \
		$(LIBVIRT_CFLAGS) \
		-I$(top_srcdir)
event_test_SOURCES = \
		event-test.c
event_test_LDADD = \
		../libvirt-glib/libvirt-glib-1.0.la \
		$(LIBVIRT_LIBS) \
		$(GLIB2_LIBS)

conn_test_CFLAGS = \
		$(WARN_CFLAGS) \
		$(GLIB2_CFLAGS) \
		$(LIBVIRT_CFLAGS) \
		$(LIBXML2_CFLAGS) \
		-I$(top_srcdir)
conn_test_SOURCES = \
		conn-test.c
conn_test_LDADD = \
		../libvirt-gobject/libvirt-gobject-1.0.la \
		../libvirt-gconfig/libvirt-gconfig-1.0.la \
		../libvirt-glib/libvirt-glib-1.0.la \
		$(LIBVIRT_LIBS) \
		$(GLIB2_LIBS) \
		$(GOBJECT2_LIBS)
