
EXTRA_DIST = libvirt-glib.sym

lib_LTLIBRARIES = libvirt-glib-1.0.la

libvirt_glib_1_0_ladir = $(includedir)/libvirt-glib-1.0/libvirt-glib
libvirt_glib_1_0_la_HEADERS = \
			libvirt-glib.h \
			libvirt-glib-error.h \
			libvirt-glib-event.h \
			libvirt-glib-main.h
libvirt_glib_1_0_la_SOURCES = \
			$(libvirt_glib_1_0_la_HEADERS) \
			libvirt-glib-error.c \
			libvirt-glib-event.c \
			libvirt-glib-main.c
libvirt_glib_1_0_la_CFLAGS = \
			-DLIBVIRT_GLIB_BUILD \
			-DG_LOG_DOMAIN="\"Libvirt.GLib\"" \
			-DLOCALEDIR="\"$(datadir)/locale\"" \
			$(COVERAGE_CFLAGS) \
			-I$(top_srcdir) \
			$(LIBVIRT_CFLAGS) \
			$(GLIB2_CFLAGS) \
			$(GTHREAD2_CFLAGS) \
			$(WARN_CFLAGS)
libvirt_glib_1_0_la_LIBADD = \
			$(LIBVIRT_LIBS) \
			$(GLIB2_LIBS) \
			$(GTHREAD2_LIBS) \
			$(CYGWIN_EXTRA_LIBADD)
libvirt_glib_1_0_la_DEPENDENCIES = \
			libvirt-glib.sym
libvirt_glib_1_0_la_LDFLAGS = \
			$(COVERAGE_CFLAGS:-f%=-Wc,f%) \
			$(CYGWIN_EXTRA_LDFLAGS) $(MINGW_EXTRA_LDFLAGS) \
			-Wl,--version-script=$(srcdir)/libvirt-glib.sym \
			-version-info $(LIBVIRT_GLIB_VERSION_INFO)

# .libs/libvirt-glib.so is built by libtool as a side-effect of the Makefile
# rule for libosvirt-glib.la.  However, checking symbols relies on Linux ELF layout
if WITH_LINUX
check-symfile: libvirt-glib.sym libvirt-glib-1.0.la
	$(AM_V_GEN)$(PERL) $(top_srcdir)/build-aux/check-symfile.pl $(srcdir)/libvirt-glib.sym \
		.libs/libvirt-glib-1.0.so
else ! WITH_LINUX
check-symfile:
endif ! WITH_LINUX
check-symsorting:
	$(AM_V_GEN)$(PERL) $(top_srcdir)/build-aux/check-symsorting.pl \
		$(srcdir)/libvirt-glib.sym
check-local: check-symfile check-symsorting

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
CLEANFILES=

if HAVE_INTROSPECTION
LibvirtGLib-1.0.gir: libvirt-glib-1.0.la
LibvirtGLib_1_0_gir_INCLUDES = GLib-2.0
LibvirtGLib_1_0_gir_PACKAGES = glib-2.0
LibvirtGLib_1_0_gir_EXPORT_PACKAGES = libvirt-glib-1.0
LibvirtGLib_1_0_gir_LIBS = libvirt-glib-1.0.la
LibvirtGLib_1_0_gir_FILES = $(libvirt_glib_1_0_la_SOURCES)
LibvirtGLib_1_0_gir_CFLAGS = $(libvirt_glib_1_0_la_CFLAGS)
LibvirtGLib_1_0_gir_SCANNERFLAGS = \
	--identifier-prefix=GVir \
	--symbol-prefix=gvir \
	--c-include="libvirt-glib/libvirt-glib.h" \
	$(NULL)
INTROSPECTION_GIRS += LibvirtGLib-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = LibvirtGLib-1.0.gir

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = LibvirtGLib-1.0.typelib

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif # HAVE_INTROSPECTION
