
EXTRA_DIST = libvirt_glib.sym

lib_LTLIBRARIES = libvirt-glib-1.0.la

libvirt_glib_1_0_la_SOURCES = events.c
libvirt_glib_1_0_la_CFLAGS = \
			$(COVERAGE_CFLAGS) \
			-I$(top_srcdir)/include \
			../include \
			$(LIBVIRT_CFLAGS) \
			$(GLIB2_CFLAGS)
libvirt_glib_1_0_la_LIBADD = \
			$(LIBVIRT_LIBS) \
			$(GLIB2_LIBS) \
			$(CYGWIN_EXTRA_LIBADD)
libvirt_glib_1_0_la_LDFLAGS = \
			$(COVERAGE_CFLAGS:-f%=-Wc,f%) \
			$(CYGWIN_EXTRA_LDFLAGS) $(MINGW_EXTRA_LDFLAGS) \
			-Wl,--version-script=$(srcdir)/libvirt_glib.sym \
			-version-info $(LIBVIRT_GLIB_VERSION_INFO)

if WITH_GOBJECT_INTROSPECTION

LIBVIRT_GLIB_INTROSPECTION_SRCS = \
                        $(srcdir)/events.c \
                        $(srcdir)/../include/libvirt-glib/libvirt-glib.h

LibvirtGLib-1.0.gir: libvirt-glib-1.0.la $(G_IR_SCANNER) Makefile.am
	$(AM_V_GEN)$(G_IR_SCANNER) -v \
                --namespace LibvirtGLib \
                --nsversion 1.0 \
                --include GLib-2.0 \
                --strip-prefix=vir_g \
                --library=$(builddir)/libvirt-glib-1.0.la \
                --add-include-path=$(top_srcdir) \
                --add-include-path=$(srcdir) \
                --add-include-path=$(builddir) \
                --output $@ \
                -I$(top_srcdir) \
                -I$(srcdir) \
                --verbose \
                --pkg=glib-2.0 \
                --pkg=gthread-2.0 \
                $(LIBVIRT_GLIB_INTROSPECTION_SRCS)

girdir = $(datadir)/gir-1.0
gir_DATA = LibvirtGLib-1.0.gir

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = LibvirtGLib-1.0.typelib

%.typelib: %.gir
	$(AM_V_GEN)$(G_IR_COMPILER) \
                --includedir=$(srcdir) \
                --includedir=$(builddir) \
                --includedir=$(girdir) \
                -o $@ $<

CLEANFILES = $(gir_DATA) $(typelibs_DATA)

endif # WITH_GOBJECT_INTROSPECTION
