
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

if WITH_GOBJECT_INTROSPECTION

LibvirtGLib-1.0.gir: libvirt-glib-1.0.la $(G_IR_SCANNER) Makefile.am
	$(AM_V_GEN)$(G_IR_SCANNER) \
                --quiet \
                --warn-all \
                --namespace LibvirtGLib \
                --nsversion 1.0 \
                --include GLib-2.0 \
                --identifier-prefix=GVir \
                --symbol-prefix=gvir \
                --library=$(builddir)/libvirt-glib-1.0.la \
                --output $@ \
                -I$(top_srcdir) \
                --verbose \
                --pkg=gthread-2.0 \
                --c-include="libvirt-glib/libvirt-glib.h" \
                --pkg-export=libvirt-glib-1.0 \
                $(libvirt_glib_1_0_la_SOURCES:%=$(srcdir)/%)

girdir = $(datadir)/gir-1.0
gir_DATA = LibvirtGLib-1.0.gir

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = LibvirtGLib-1.0.typelib

%.typelib: %.gir
	$(AM_V_GEN)$(G_IR_COMPILER) \
                --includedir=$(builddir) \
                --includedir=$(girdir) \
                -o $@ $<

CLEANFILES = $(gir_DATA) $(typelibs_DATA)

endif # WITH_GOBJECT_INTROSPECTION
