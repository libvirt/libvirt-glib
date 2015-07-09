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
			libvirt-gconfig-capabilities-host.h \
			libvirt-gconfig-capabilities-cpu.h \
			libvirt-gconfig-capabilities-cpu-feature.h \
			libvirt-gconfig-capabilities-cpu-model.h \
			libvirt-gconfig-capabilities-cpu-topology.h \
			libvirt-gconfig-capabilities-guest.h \
			libvirt-gconfig-capabilities-guest-arch.h \
			libvirt-gconfig-capabilities-guest-domain.h \
			libvirt-gconfig-capabilities-guest-feature.h \
			libvirt-gconfig-capabilities-host-secmodel.h \
			libvirt-gconfig-domain.h \
			libvirt-gconfig-domain-address.h \
			libvirt-gconfig-domain-address-pci.h \
			libvirt-gconfig-domain-address-usb.h \
			libvirt-gconfig-domain-channel.h \
			libvirt-gconfig-domain-chardev.h \
			libvirt-gconfig-domain-chardev-source.h \
			libvirt-gconfig-domain-chardev-source-pty.h \
			libvirt-gconfig-domain-chardev-source-spiceport.h \
			libvirt-gconfig-domain-chardev-source-spicevmc.h \
			libvirt-gconfig-domain-chardev-source-unix.h \
			libvirt-gconfig-domain-clock.h \
			libvirt-gconfig-domain-console.h \
			libvirt-gconfig-domain-controller.h \
			libvirt-gconfig-domain-controller-usb.h \
			libvirt-gconfig-domain-cpu.h \
			libvirt-gconfig-domain-cpu-feature.h \
			libvirt-gconfig-domain-cpu-model.h \
			libvirt-gconfig-domain-device.h \
			libvirt-gconfig-domain-disk.h \
			libvirt-gconfig-domain-disk-driver.h \
			libvirt-gconfig-domain-filesys.h \
			libvirt-gconfig-domain-graphics.h \
			libvirt-gconfig-domain-graphics-desktop.h \
			libvirt-gconfig-domain-graphics-rdp.h \
			libvirt-gconfig-domain-graphics-sdl.h \
			libvirt-gconfig-domain-graphics-spice.h \
			libvirt-gconfig-domain-graphics-vnc.h \
			libvirt-gconfig-domain-input.h \
			libvirt-gconfig-domain-interface.h \
			libvirt-gconfig-domain-interface-bridge.h \
			libvirt-gconfig-domain-interface-filterref.h \
			libvirt-gconfig-domain-interface-filterref-parameter.h \
			libvirt-gconfig-domain-interface-network.h \
			libvirt-gconfig-domain-interface-user.h \
			libvirt-gconfig-domain-memballoon.h \
			libvirt-gconfig-domain-os.h \
			libvirt-gconfig-domain-parallel.h \
			libvirt-gconfig-domain-power-management.h \
			libvirt-gconfig-domain-redirdev.h \
			libvirt-gconfig-domain-seclabel.h \
			libvirt-gconfig-domain-serial.h \
			libvirt-gconfig-domain-smartcard.h \
			libvirt-gconfig-domain-smartcard-host.h \
			libvirt-gconfig-domain-smartcard-host-certificates.h \
			libvirt-gconfig-domain-smartcard-passthrough.h \
			libvirt-gconfig-domain-snapshot.h \
			libvirt-gconfig-domain-snapshot-disk.h \
			libvirt-gconfig-domain-sound.h \
			libvirt-gconfig-domain-timer.h \
			libvirt-gconfig-domain-timer-hpet.h \
			libvirt-gconfig-domain-timer-pit.h \
			libvirt-gconfig-domain-timer-rtc.h \
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
			libvirt-gconfig-capabilities-cpu-private.h \
			libvirt-gconfig-compat.h \
			libvirt-gconfig-domain-chardev-source-private.h \
			libvirt-gconfig-domain-device-private.h \
			libvirt-gconfig-domain-snapshot-private.h \
			libvirt-gconfig-helpers-private.h \
			libvirt-gconfig-object-private.h \
			libvirt-gconfig-xml-doc.h
GCONFIG_SOURCE_FILES = \
			libvirt-gconfig-object.c \
			libvirt-gconfig-main.c \
			libvirt-gconfig-capabilities.c \
			libvirt-gconfig-capabilities-host.c \
			libvirt-gconfig-capabilities-cpu.c \
			libvirt-gconfig-capabilities-cpu-feature.c \
			libvirt-gconfig-capabilities-cpu-model.c \
			libvirt-gconfig-capabilities-cpu-topology.c \
			libvirt-gconfig-capabilities-guest.c \
			libvirt-gconfig-capabilities-guest-arch.c \
			libvirt-gconfig-capabilities-guest-domain.c \
			libvirt-gconfig-capabilities-guest-feature.c \
			libvirt-gconfig-capabilities-host-secmodel.c \
			libvirt-gconfig-domain.c \
			libvirt-gconfig-domain-address.c \
			libvirt-gconfig-domain-address-pci.c \
			libvirt-gconfig-domain-address-usb.c \
			libvirt-gconfig-domain-channel.c \
			libvirt-gconfig-domain-chardev.c \
			libvirt-gconfig-domain-chardev-source.c \
			libvirt-gconfig-domain-chardev-source-pty.c \
			libvirt-gconfig-domain-chardev-source-spiceport.c \
			libvirt-gconfig-domain-chardev-source-spicevmc.c \
			libvirt-gconfig-domain-chardev-source-unix.c \
			libvirt-gconfig-domain-clock.c \
			libvirt-gconfig-domain-console.c \
			libvirt-gconfig-domain-controller.c \
			libvirt-gconfig-domain-controller-usb.c \
			libvirt-gconfig-domain-cpu.c \
			libvirt-gconfig-domain-cpu-feature.c \
			libvirt-gconfig-domain-cpu-model.c \
			libvirt-gconfig-domain-device.c \
			libvirt-gconfig-domain-disk.c \
			libvirt-gconfig-domain-disk-driver.c \
			libvirt-gconfig-domain-filesys.c \
			libvirt-gconfig-domain-graphics.c \
			libvirt-gconfig-domain-graphics-desktop.c \
			libvirt-gconfig-domain-graphics-rdp.c \
			libvirt-gconfig-domain-graphics-sdl.c \
			libvirt-gconfig-domain-graphics-spice.c \
			libvirt-gconfig-domain-graphics-vnc.c \
			libvirt-gconfig-domain-input.c \
			libvirt-gconfig-domain-interface.c \
			libvirt-gconfig-domain-interface-bridge.c \
			libvirt-gconfig-domain-interface-filterref.c \
			libvirt-gconfig-domain-interface-filterref-parameter.c \
			libvirt-gconfig-domain-interface-network.c \
			libvirt-gconfig-domain-interface-user.c \
			libvirt-gconfig-domain-memballoon.c \
			libvirt-gconfig-domain-os.c \
			libvirt-gconfig-domain-parallel.c \
			libvirt-gconfig-domain-power-management.c \
			libvirt-gconfig-domain-redirdev.c \
			libvirt-gconfig-domain-seclabel.c \
			libvirt-gconfig-domain-serial.c \
			libvirt-gconfig-domain-smartcard.c \
			libvirt-gconfig-domain-smartcard-host.c \
			libvirt-gconfig-domain-smartcard-host-certificates.c \
			libvirt-gconfig-domain-smartcard-passthrough.c \
			libvirt-gconfig-domain-snapshot.c \
			libvirt-gconfig-domain-snapshot-disk.c \
			libvirt-gconfig-domain-sound.c \
			libvirt-gconfig-domain-timer.c \
			libvirt-gconfig-domain-timer-hpet.c \
			libvirt-gconfig-domain-timer-pit.c \
			libvirt-gconfig-domain-timer-rtc.c \
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

GCONFIG_GENERATED_FILES = \
			libvirt-gconfig-enum-types.c \
			libvirt-gconfig-enum-types.h

libvirt_gconfig_1_0_ladir = $(includedir)/libvirt-gconfig-1.0/libvirt-gconfig
libvirt_gconfig_1_0_la_HEADERS = \
			$(GCONFIG_HEADER_FILES)
nodist_libvirt_gconfig_1_0_la_HEADERS = \
			$(builddir)/libvirt-gconfig-enum-types.h
libvirt_gconfig_1_0_la_SOURCES = \
			$(libvirt_gconfig_1_0_la_HEADERS) \
			$(GCONFIG_SOURCE_FILES)
nodist_libvirt_gconfig_1_0_la_SOURCES = \
			$(GCONFIG_GENERATED_FILES)
libvirt_gconfig_1_0_la_CFLAGS = \
			-DG_LOG_DOMAIN="\"Libvirt.GConfig\"" \
			-DDATADIR="\"$(datadir)\"" \
			-DLIBVIRT_GCONFIG_BUILD \
			$(COVERAGE_CFLAGS) \
			-I$(top_srcdir) \
			-I$(top_builddir) \
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

BUILT_SOURCES = $(GCONFIG_GENERATED_FILES)

CLEANFILES = $(BUILT_SOURCES)

libvirt-gconfig-enum-types.h: $(GCONFIG_HEADER_FILES) libvirt-gconfig-enum-types.h.template
	$(AM_V_GEN) ( $(GLIB_MKENUMS) --template $(srcdir)/libvirt-gconfig-enum-types.h.template $(GCONFIG_HEADER_FILES:%=$(srcdir)/%) ) | \
	    sed -e "s/G_TYPE_VIR_CONFIG/GVIR_CONFIG_TYPE/" -e "s/g_vir/gvir/" > libvirt-gconfig-enum-types.h

libvirt-gconfig-enum-types.c: $(GCONFIG_HEADER_FILES) libvirt-gconfig-enum-types.c.template
	$(AM_V_GEN) ( $(GLIB_MKENUMS) --template $(srcdir)/libvirt-gconfig-enum-types.c.template $(GCONFIG_HEADER_FILES:%=$(srcdir)/%) ) | \
	    sed -e "s/G_TYPE_VIR_CONFIG/GVIR_CONFIG_TYPE/" -e "s/g_vir/gvir/" > libvirt-gconfig-enum-types.c

# .libs/libvirt-gconfig.so is built by libtool as a side-effect of the Makefile
# rule for libosvirt-gconfig.la.  However, checking symbols relies on Linux ELF layout
if WITH_LINUX
check-symfile: libvirt-gconfig.sym libvirt-gconfig-1.0.la
	$(AM_V_GEN)$(PERL) $(top_srcdir)/build-aux/check-symfile.pl $(srcdir)/libvirt-gconfig.sym \
		.libs/libvirt-gconfig-1.0.so
else ! WITH_LINUX
check-symfile:
endif ! WITH_LINUX
check-symsorting:
	$(AM_V_GEN)$(PERL) $(top_srcdir)/build-aux/check-symsorting.pl \
		$(srcdir)/libvirt-gconfig.sym
check-local: check-symfile check-symsorting

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =

if HAVE_INTROSPECTION
LibvirtGConfig-1.0.gir: libvirt-gconfig-1.0.la
LibvirtGConfig_1_0_gir_INCLUDES = GObject-2.0
LibvirtGConfig_1_0_gir_PACKAGES = gobject-2.0
LibvirtGConfig_1_0_gir_EXPORT_PACKAGES = libvirt-gconfig-1.0
LibvirtGConfig_1_0_gir_LIBS = libvirt-gconfig-1.0.la
LibvirtGConfig_1_0_gir_FILES = \
	$(GCONFIG_SOURCE_FILES) \
	$(GCONFIG_HEADER_FILES) \
	$(GCONFIG_GENERATED_FILES) \
	$(NULL)
LibvirtGConfig_1_0_gir_CFLAGS = $(libvirt_gconfig_1_0_la_CFLAGS)
LibvirtGConfig_1_0_gir_SCANNERFLAGS = \
	--identifier-prefix=GVirConfig \
	--symbol-prefix=gvir_config \
	--c-include="libvirt-gconfig/libvirt-gconfig.h" \
	$(NULL)
INTROSPECTION_GIRS += LibvirtGConfig-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = LibvirtGConfig-1.0.gir

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = LibvirtGConfig-1.0.typelib

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif # HAVE_INTROSPECTION
