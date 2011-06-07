
EXTRA_DIST = libvirt-gconfig.sym

lib_LTLIBRARIES = libvirt-gconfig-1.0.la

GCONFIG_HEADER_FILES = \
			libvirt-gconfig.h \
			libvirt-gconfig-object.h \
			libvirt-gconfig-domain.h
GCONFIG_SOURCE_FILES = \
			libvirt-gconfig-object.c \
			libvirt-gconfig-domain.c

libvirt_gconfig_1_0_ladir = $(includedir)/libvirt-gconfig-1.0/libvirt-gconfig
libvirt_gconfig_1_0_la_HEADERS = \
			$(GCONFIG_HEADER_FILES)
libvirt_gconfig_1_0_la_SOURCES = \
			$(libvirt_gconfig_1_0_la_HEADERS) \
			$(GCONFIG_SOURCE_FILES)
libvirt_gconfig_1_0_la_CFLAGS = \
			-DDATADIR="\"$(datadir)\"" \
			-DLIBVIRT_GCONFIG_BUILD \
			$(COVERAGE_CFLAGS) \
			-I$(top_srcdir) \
			$(GOBJECT2_CFLAGS) \
			$(WARN_CFLAGS)
libvirt_gconfig_1_0_la_LIBADD = \
			$(GOBJECT2_LIBS) \
			$(CYGWIN_EXTRA_LIBADD)
libvirt_gconfig_1_0_la_LDFLAGS = \
			$(COVERAGE_CFLAGS:-f%=-Wc,f%) \
			$(CYGWIN_EXTRA_LDFLAGS) $(MINGW_EXTRA_LDFLAGS) \
			-Wl,--version-script=$(srcdir)/libvirt-gconfig.sym \
			-version-info $(LIBVIRT_GLIB_VERSION_INFO)

if WITH_GOBJECT_INTROSPECTION

LibvirtGConfig-1.0.gir: libvirt-gconfig-1.0.la $(G_IR_SCANNER) Makefile.am
	$(AM_V_GEN)$(G_IR_SCANNER) \
                --quiet \
                --namespace LibvirtGConfig \
                --nsversion 1.0 \
                --include GObject-2.0 \
                --strip-prefix=gvir \
                --library=$(builddir)/libvirt-gconfig-1.0.la \
                --output $@ \
                -I$(top_srcdir) \
                -I$(top_buildir) \
                --verbose \
                --pkg=gobject-2.0 \
		$(srcdir)/libvirt-gconfig.h \
                $(GCONFIG_SOURCE_FILES:%=$(srcdir)/%) \
                $(GCONFIG_HEADER_FILES:%=$(srcdir)/%)

girdir = $(datadir)/gir-1.0
gir_DATA = LibvirtGConfig-1.0.gir

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = LibvirtGConfig-1.0.typelib

%.typelib: %.gir
	$(AM_V_GEN)$(G_IR_COMPILER) \
                --includedir=$(builddir) \
                --includedir=$(girdir) \
                -o $@ $<

CLEANFILES = $(gir_DATA) $(typelibs_DATA)

endif # WITH_GOBJECT_INTROSPECTION
