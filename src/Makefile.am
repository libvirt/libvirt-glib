
EXTRA_DIST = libvirt_glib.sym

lib_LTLIBRARIES = libvirt-glib.la

libvirt_glib_la_SOURCES = events.c
libvirt_glib_la_CFLAGS = \
			$(COVERAGE_CFLAGS) \
			-I$(top_srcdir)/include \
			../include \
			$(LIBVIRT_CFLAGS) \
			$(GLIB2_CFLAGS)
libvirt_glib_la_LIBADD = \
			$(LIBVIRT_LIBS) \
			$(GLIB2_LIBS) \
			$(CYGWIN_EXTRA_LIBADD)
libvirt_glib_la_LDFLAGS = \
			$(COVERAGE_CFLAGS:-f%=-Wc,f%) \
			$(CYGWIN_EXTRA_LDFLAGS) $(MINGW_EXTRA_LDFLAGS) \
			-Wl,--version-script=$(srcdir)/libvirt_glib.sym \
			-version-info $(LIBVIRT_GLIB_VERSION_INFO)
