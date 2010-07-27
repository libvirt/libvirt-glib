
EXTRA_DIST = libvirt-gobject.sym

lib_LTLIBRARIES = libvirt-gobject-1.0.la

libvirt_gobject_1_0_ladir = $(includedir)/libvirt-gobject
libvirt_gobject_1_0_la_HEADERS = \
			libvirt-gobject.h \
			libvirt-gobject-main.h \
			libvirt-gobject-connection.h \
			libvirt-gobject-domain.h
libvirt_gobject_1_0_la_SOURCES = \
			$(libvirt_gobject_1_0_la_HEADERS) \
			libvirt-gobject-main.c \
			libvirt-gobject-connection.c \
			libvirt-gobject-domain.c
libvirt_gobject_1_0_la_CFLAGS = \
			-DLIBVIRT_GOBJECT_BUILD \
			$(COVERAGE_CFLAGS) \
			-I$(top_srcdir) \
			$(LIBVIRT_CFLAGS) \
			$(GOBJECT2_CFLAGS) \
			$(GIO2_CFLAGS) \
			$(WARN_CFLAGS)
libvirt_gobject_1_0_la_LIBADD = \
			$(LIBVIRT_LIBS) \
			$(GOBJECT2_LIBS) \
			$(GIO2_LIBS) \
			../libvirt-glib/libvirt-glib-1.0.la \
			$(CYGWIN_EXTRA_LIBADD)
libvirt_gobject_1_0_la_LDFLAGS = \
			$(COVERAGE_CFLAGS:-f%=-Wc,f%) \
			$(CYGWIN_EXTRA_LDFLAGS) $(MINGW_EXTRA_LDFLAGS) \
			-Wl,--version-script=$(srcdir)/libvirt-gobject.sym \
			-version-info $(LIBVIRT_GLIB_VERSION_INFO)

if WITH_GOBJECT_INTROSPECTION

LibvirtGObject-1.0.gir: libvirt-gobject-1.0.la $(G_IR_SCANNER) Makefile.am
	$(AM_V_GEN)$(G_IR_SCANNER) -v \
                --namespace LibvirtGObject \
                --nsversion 1.0 \
                --include GObject-2.0 \
                --include Gio-2.0 \
                --strip-prefix=VirG \
                --strip-prefix=vir_g \
                --library=$(builddir)/libvirt-gobject-1.0.la \
                --add-include-path=$(top_srcdir) \
                --add-include-path=$(srcdir) \
                --add-include-path=$(builddir) \
                --output $@ \
                -I$(top_srcdir) \
                -I$(srcdir) \
                --verbose \
                --pkg=gobject-2.0 \
                --pkg=gthread-2.0 \
                $(libvirt_gobject_1_0_la_SOURCES:%=$(srcdir)/%)

girdir = $(datadir)/gir-1.0
gir_DATA = LibvirtGObject-1.0.gir

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = LibvirtGObject-1.0.typelib

%.typelib: %.gir
	$(AM_V_GEN)$(G_IR_COMPILER) \
                --includedir=$(srcdir) \
                --includedir=$(builddir) \
                --includedir=$(girdir) \
                -o $@ $<

CLEANFILES = $(gir_DATA) $(typelibs_DATA)

endif # WITH_GOBJECT_INTROSPECTION
