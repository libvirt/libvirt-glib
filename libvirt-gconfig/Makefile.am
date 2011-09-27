
EXTRA_DIST = libvirt-gconfig.sym

lib_LTLIBRARIES = libvirt-gconfig-1.0.la

GCONFIG_HEADER_FILES = \
			libvirt-gconfig.h \
			libvirt-gconfig-object.h \
			libvirt-gconfig-capabilities.h \
			libvirt-gconfig-domain.h \
			libvirt-gconfig-domain-snapshot.h \
			libvirt-gconfig-interface.h \
			libvirt-gconfig-network.h \
			libvirt-gconfig-network-filter.h \
			libvirt-gconfig-node-device.h \
			libvirt-gconfig-secret.h \
			libvirt-gconfig-storage-pool.h \
			libvirt-gconfig-storage-vol.h
GCONFIG_SOURCE_FILES = \
			libvirt-gconfig-object.c \
			libvirt-gconfig-capabilities.c \
			libvirt-gconfig-domain.c \
			libvirt-gconfig-domain-snapshot.c \
			libvirt-gconfig-interface.c \
			libvirt-gconfig-network.c \
			libvirt-gconfig-network-filter.c \
			libvirt-gconfig-node-device.c \
			libvirt-gconfig-secret.c \
			libvirt-gconfig-storage-pool.c \
			libvirt-gconfig-storage-vol.c

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
			$(LIBXML2_CFLAGS) \
			$(WARN_CFLAGS)
libvirt_gconfig_1_0_la_LIBADD = \
			$(GOBJECT2_LIBS) \
			$(LIBXML2_LIBS) \
			$(CYGWIN_EXTRA_LIBADD)
libvirt_gconfig_1_0_la_DEPENDENCIES = \
			libvirt-gconfig.sym
libvirt_gconfig_1_0_la_LDFLAGS = \
			$(COVERAGE_CFLAGS:-f%=-Wc,f%) \
			$(CYGWIN_EXTRA_LDFLAGS) $(MINGW_EXTRA_LDFLAGS) \
			-Wl,--version-script=$(srcdir)/libvirt-gconfig.sym \
			-version-info $(LIBVIRT_GLIB_VERSION_INFO)

if WITH_GOBJECT_INTROSPECTION

LibvirtGConfig-1.0.gir: libvirt-gconfig-1.0.la $(G_IR_SCANNER) Makefile.am
	$(AM_V_GEN)$(G_IR_SCANNER) \
                --quiet \
                --warn-all \
                --namespace LibvirtGConfig \
                --nsversion 1.0 \
                --include GObject-2.0 \
                --identifier-prefix=GVirConfig \
                --symbol-prefix=gvir \
                --library=$(builddir)/libvirt-gconfig-1.0.la \
                --output $@ \
                -I$(top_srcdir) \
                -I$(top_builddir) \
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
