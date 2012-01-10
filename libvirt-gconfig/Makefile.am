SUBDIRS = . tests

EXTRA_DIST = \
		libvirt-gconfig.sym \
		libvirt-gconfig-enum-types.h.template \
		libvirt-gconfig-enum-types.c.template

lib_LTLIBRARIES = libvirt-gconfig-1.0.la

GCONFIG_HEADER_FILES = \
			libvirt-gconfig.h \
			libvirt-gconfig-main.h \
			libvirt-gconfig-object.h \
			libvirt-gconfig-capabilities.h \
			libvirt-gconfig-domain.h \
			libvirt-gconfig-domain-channel.h \
			libvirt-gconfig-domain-chardev.h \
			libvirt-gconfig-domain-chardev-source.h \
			libvirt-gconfig-domain-chardev-source-pty.h \
			libvirt-gconfig-domain-clock.h \
			libvirt-gconfig-domain-console.h \
			libvirt-gconfig-domain-device.h \
			libvirt-gconfig-domain-disk.h \
			libvirt-gconfig-domain-filesys.h \
			libvirt-gconfig-domain-graphics.h \
			libvirt-gconfig-domain-graphics-sdl.h \
			libvirt-gconfig-domain-graphics-spice.h \
			libvirt-gconfig-domain-graphics-vnc.h \
			libvirt-gconfig-domain-input.h \
			libvirt-gconfig-domain-interface.h \
			libvirt-gconfig-domain-interface-bridge.h \
			libvirt-gconfig-domain-interface-network.h \
			libvirt-gconfig-domain-interface-user.h \
			libvirt-gconfig-domain-memballoon.h \
			libvirt-gconfig-domain-os.h \
			libvirt-gconfig-domain-parallel.h \
			libvirt-gconfig-domain-seclabel.h \
			libvirt-gconfig-domain-serial.h \
			libvirt-gconfig-domain-snapshot.h \
			libvirt-gconfig-domain-sound.h \
			libvirt-gconfig-domain-timer.h \
			libvirt-gconfig-domain-video.h \
			libvirt-gconfig-helpers.h \
			libvirt-gconfig-interface.h \
			libvirt-gconfig-network.h \
			libvirt-gconfig-network-filter.h \
			libvirt-gconfig-node-device.h \
			libvirt-gconfig-secret.h \
			libvirt-gconfig-storage-permissions.h \
			libvirt-gconfig-storage-pool.h \
			libvirt-gconfig-storage-pool-source.h \
			libvirt-gconfig-storage-pool-target.h \
			libvirt-gconfig-storage-vol.h \
			libvirt-gconfig-storage-vol-backing-store.h \
			libvirt-gconfig-storage-vol-target.h
noinst_HEADERS = \
			libvirt-gconfig-private.h \
			libvirt-gconfig-domain-device-private.h \
			libvirt-gconfig-helpers-private.h \
			libvirt-gconfig-object-private.h \
			libvirt-gconfig-xml-doc.h
GCONFIG_SOURCE_FILES = \
			libvirt-gconfig-object.c \
			libvirt-gconfig-main.c \
			libvirt-gconfig-capabilities.c \
			libvirt-gconfig-domain.c \
			libvirt-gconfig-domain-channel.c \
			libvirt-gconfig-domain-chardev.c \
			libvirt-gconfig-domain-chardev-source.c \
			libvirt-gconfig-domain-chardev-source-pty.c \
			libvirt-gconfig-domain-clock.c \
			libvirt-gconfig-domain-console.c \
			libvirt-gconfig-domain-device.c \
			libvirt-gconfig-domain-disk.c \
			libvirt-gconfig-domain-filesys.c \
			libvirt-gconfig-domain-graphics.c \
			libvirt-gconfig-domain-graphics-sdl.c \
			libvirt-gconfig-domain-graphics-spice.c \
			libvirt-gconfig-domain-graphics-vnc.c \
			libvirt-gconfig-domain-input.c \
			libvirt-gconfig-domain-interface.c \
			libvirt-gconfig-domain-interface-bridge.c \
			libvirt-gconfig-domain-interface-network.c \
			libvirt-gconfig-domain-interface-user.c \
			libvirt-gconfig-domain-memballoon.c \
			libvirt-gconfig-domain-os.c \
			libvirt-gconfig-domain-parallel.c \
			libvirt-gconfig-domain-seclabel.c \
			libvirt-gconfig-domain-serial.c \
			libvirt-gconfig-domain-snapshot.c \
			libvirt-gconfig-domain-sound.c \
			libvirt-gconfig-domain-timer.c \
			libvirt-gconfig-domain-video.c \
			libvirt-gconfig-helpers.c \
			libvirt-gconfig-interface.c \
			libvirt-gconfig-network.c \
			libvirt-gconfig-network-filter.c \
			libvirt-gconfig-node-device.c \
			libvirt-gconfig-secret.c \
			libvirt-gconfig-storage-permissions.c \
			libvirt-gconfig-storage-pool.c \
			libvirt-gconfig-storage-pool-source.c \
			libvirt-gconfig-storage-pool-target.c \
			libvirt-gconfig-storage-vol.c \
			libvirt-gconfig-storage-vol-backing-store.c \
			libvirt-gconfig-storage-vol-target.c \
			libvirt-gconfig-xml-doc.c

libvirt_gconfig_1_0_ladir = $(includedir)/libvirt-gconfig-1.0/libvirt-gconfig
libvirt_gconfig_1_0_la_HEADERS = \
			$(GCONFIG_HEADER_FILES)
nodist_libvirt_gconfig_1_0_la_HEADERS = \
			$(builddir)/libvirt-gconfig-enum-types.h
libvirt_gconfig_1_0_la_SOURCES = \
			$(libvirt_gconfig_1_0_la_HEADERS) \
			$(GCONFIG_SOURCE_FILES)
nodist_libvirt_gconfig_1_0_la_SOURCES = \
			$(builddir)/libvirt-gconfig-enum-types.c
libvirt_gconfig_1_0_la_CFLAGS = \
			-DG_LOG_DOMAIN="\"Libvirt.GConfig\"" \
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

BUILT_SOURCES = \
		libvirt-gconfig-enum-types.c \
		libvirt-gconfig-enum-types.h

CLEANFILES = $(BUILT_SOURCES)

libvirt-gconfig-enum-types.h: $(GCONFIG_HEADER_FILES) libvirt-gconfig-enum-types.h.template
	$(AM_V_GEN) ( $(GLIB_MKENUMS) --template $(srcdir)/libvirt-gconfig-enum-types.h.template $(GCONFIG_HEADER_FILES:%=$(srcdir)/%) ) | \
	    sed -e "s/G_TYPE_VIR_CONFIG/GVIR_CONFIG_TYPE/" -e "s/g_vir/gvir/" > libvirt-gconfig-enum-types.h

libvirt-gconfig-enum-types.c: $(GCONFIG_HEADER_FILES) libvirt-gconfig-enum-types.c.template
	$(AM_V_GEN) ( $(GLIB_MKENUMS) --template $(srcdir)/libvirt-gconfig-enum-types.c.template $(GCONFIG_HEADER_FILES:%=$(srcdir)/%) ) | \
	    sed -e "s/G_TYPE_VIR_CONFIG/GVIR_CONFIG_TYPE/" -e "s/g_vir/gvir/" > libvirt-gconfig-enum-types.c

if WITH_GOBJECT_INTROSPECTION

LibvirtGConfig-1.0.gir: libvirt-gconfig-1.0.la $(G_IR_SCANNER) Makefile.am
	$(AM_V_GEN)$(G_IR_SCANNER) \
                --quiet \
                --warn-all \
                --namespace LibvirtGConfig \
                --nsversion 1.0 \
                --include GObject-2.0 \
                --identifier-prefix=GVirConfig \
                --symbol-prefix=gvir_config \
                --library=$(builddir)/libvirt-gconfig-1.0.la \
                --output $@ \
                -I$(top_builddir) \
                -I$(top_srcdir) \
                --verbose \
                --c-include="libvirt-gconfig/libvirt-gconfig.h" \
                --pkg=libxml-2.0 \
                --pkg-export=libvirt-gconfig-1.0 \
                $(srcdir)/libvirt-gconfig.h \
                $(GCONFIG_SOURCE_FILES:%=$(srcdir)/%) \
                $(GCONFIG_HEADER_FILES:%=$(srcdir)/%) \
                $(builddir)/libvirt-gconfig-enum-types.c \
                $(builddir)/libvirt-gconfig-enum-types.h


girdir = $(datadir)/gir-1.0
gir_DATA = LibvirtGConfig-1.0.gir

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = LibvirtGConfig-1.0.typelib

%.typelib: %.gir
	$(AM_V_GEN)$(G_IR_COMPILER) \
                --includedir=$(builddir) \
                --includedir=$(girdir) \
                -o $@ $<

CLEANFILES += $(gir_DATA) $(typelibs_DATA)

endif # WITH_GOBJECT_INTROSPECTION
