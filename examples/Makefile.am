
noinst_PROGRAMS = event-test

EXTRA_DIST = event-test.py

event_test_CFLAGS = \
		$(WARN_CFLAGS) \
		$(LIBVIRT_CFLAGS) \
		$(GLIB2_CFLAGS) \
		-I$(top_srcdir)/include \
		-I../include
event_test_SOURCES = \
		event-test.c
event_test_LDADD = \
		../src/libvirt-glib.la \
		$(LIBVIRT_LIBS) \
		$(GLIB2_LIBS)
