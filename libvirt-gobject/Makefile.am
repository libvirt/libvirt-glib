
EXTRA_DIST = libvirt-gobject.sym

lib_LTLIBRARIES = libvirt-gobject-1.0.la

GOBJECT_HEADER_FILES = \
			libvirt-gobject.h \
			libvirt-gobject-main.h \
			libvirt-gobject-domain-snapshot.h \
			libvirt-gobject-domain-device.h \
			libvirt-gobject-domain-disk.h \
			libvirt-gobject-domain-interface.h \
			libvirt-gobject-domain.h \
			libvirt-gobject-interface.h \
			libvirt-gobject-network.h \
			libvirt-gobject-network-dhcp-lease.h \
			libvirt-gobject-network-filter.h \
			libvirt-gobject-node-device.h \
			libvirt-gobject-secret.h \
			libvirt-gobject-storage-vol.h \
			libvirt-gobject-storage-pool.h \
			libvirt-gobject-stream.h \
			libvirt-gobject-connection.h \
			libvirt-gobject-manager.h
noinst_HEADERS = \
			libvirt-gobject-storage-pool-private.h \
			libvirt-gobject-network-dhcp-lease-private.h
GOBJECT_SOURCE_FILES = \
			libvirt-gobject-main.c \
			libvirt-gobject-domain-snapshot.c \
			libvirt-gobject-domain-device.c \
			libvirt-gobject-domain-disk.c \
			libvirt-gobject-domain-interface.c \
			libvirt-gobject-domain.c \
			libvirt-gobject-interface.c \
			libvirt-gobject-network.c \
			libvirt-gobject-network-dhcp-lease.c \
			libvirt-gobject-network-filter.c \
			libvirt-gobject-node-device.c \
			libvirt-gobject-secret.c \
			libvirt-gobject-storage-vol.c \
			libvirt-gobject-storage-pool.c \
			libvirt-gobject-stream.c \
			libvirt-gobject-connection.c \
			libvirt-gobject-manager.c

GOBJECT_GENERATED_FILES = \
			libvirt-gobject-enums.c \
			libvirt-gobject-enums.h

libvirt_gobject_1_0_ladir = $(includedir)/libvirt-gobject-1.0/libvirt-gobject
libvirt_gobject_1_0_la_HEADERS = \
			$(GOBJECT_HEADER_FILES)
nodist_libvirt_gobject_1_0_la_HEADERS = \
			libvirt-gobject-enums.h
libvirt_gobject_1_0_la_SOURCES = \
			$(libvirt_gobject_1_0_la_HEADERS) \
			$(GOBJECT_SOURCE_FILES) \
			libvirt-gobject-domain-device-private.h \
			libvirt-gobject-compat.h \
			libvirt-gobject-compat.c \
			libvirt-gobject-input-stream.h \
			libvirt-gobject-input-stream.c \
			libvirt-gobject-output-stream.h \
			libvirt-gobject-output-stream.c
nodist_libvirt_gobject_1_0_la_SOURCES = \
			$(GOBJECT_GENERATED_FILES)
libvirt_gobject_1_0_la_CFLAGS = \
			-DGETTEXT_PACKAGE="\"libvirt-glib\"" \
			-DG_LOG_DOMAIN="\"Libvirt.GObject\"" \
			-DDATADIR="\"$(datadir)\"" \
			-DLIBVIRT_GOBJECT_BUILD \
			-DGLIB_VERSION_MIN_REQUIRED=$(GLIB2_VERSION_SYMBOL) \
			-DGLIB_VERSION_MAX_ALLOWED=$(GLIB2_VERSION_SYMBOL) \
			$(COVERAGE_CFLAGS) \
			-I$(top_srcdir) \
			-I$(top_builddir) \
			$(GIO2_CFLAGS) \
			$(GOBJECT2_CFLAGS) \
			$(GTHREAD2_CFLAGS) \
			$(LIBVIRT_CFLAGS) \
			$(LIBXML2_CFLAGS) \
			$(WARN_CFLAGS)
libvirt_gobject_1_0_la_LIBADD = \
			$(GIO2_LIBS) \
			$(GOBJECT2_LIBS) \
			$(GTHREAD2_LIBS) \
			$(LIBVIRT_LIBS) \
			../libvirt-glib/libvirt-glib-1.0.la \
			../libvirt-gconfig/libvirt-gconfig-1.0.la \
			$(CYGWIN_EXTRA_LIBADD)
libvirt_gobject_1_0_la_DEPENDENCIES = \
                        libvirt-gobject.sym \
			../libvirt-glib/libvirt-glib-1.0.la \
			../libvirt-gconfig/libvirt-gconfig-1.0.la
libvirt_gobject_1_0_la_LDFLAGS = \
			$(COVERAGE_CFLAGS:-f%=-Wc,f%) \
			$(CYGWIN_EXTRA_LDFLAGS) $(MINGW_EXTRA_LDFLAGS) \
			-Wl,--version-script=$(srcdir)/libvirt-gobject.sym \
			-version-info $(LIBVIRT_GLIB_VERSION_INFO)

libvirt-gobject-enums.c: $(libvirt_gobject_1_0_la_HEADERS)
	$(AM_V_GEN)glib-mkenums \
                        --fhead "#include \"libvirt-gobject/libvirt-gobject.h\"\n\n" \
                        --vhead "static const G@Type@Value _@enum_name@_values[] = {" \
                        --vprod "  { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
                        --vtail "  { 0, NULL, NULL }\n};\n\n" \
                        --vtail "GType\n@enum_name@_get_type (void)\n{\n" \
                        --vtail "  static GType type = 0;\n\n" \
                        --vtail "  if (!type)\n" \
                        --vtail "    type = g_@type@_register_static (\"@EnumName@\", _@enum_name@_values);\n\n" \
                        --vtail "  return type;\n}\n\n" \
                $^ | sed -e 's/g_vir/gvir/g' > $@

libvirt-gobject-enums.h: $(libvirt_gobject_1_0_la_HEADERS)
	$(AM_V_GEN)glib-mkenums    --fhead "#ifndef __LIBVIRT_GOBJECT_ENUMS_H__\n" \
                        --fhead "#define __LIBVIRT_GOBJECT_ENUMS_H__\n\n" \
                        --fhead "G_BEGIN_DECLS\n\n" \
                        --ftail "G_END_DECLS\n\n" \
                        --ftail "#endif /* __LIBVIRT_GOBJECT_ENUMS_H__ */\n" \
                        --eprod "#define GVIR_TYPE_@ENUMSHORT@ @enum_name@_get_type()\n" \
                        --eprod "GType @enum_name@_get_type (void);\n" \
                $^ | sed -e 's/g_vir/gvir/g' | sed -e 's/_VIR_/_/g' > $@

BUILT_SOURCES = $(GOBJECT_GENERATED_FILES)

CLEANFILES = $(BUILT_SOURCES)

# .libs/libvirt-gobject.so is built by libtool as a side-effect of the Makefile
# rule for libosvirt-gobject.la.  However, checking symbols relies on Linux ELF layout
if WITH_LINUX
check-symfile: libvirt-gobject.sym libvirt-gobject-1.0.la
	$(AM_V_GEN)$(PERL) $(top_srcdir)/build-aux/check-symfile.pl $(srcdir)/libvirt-gobject.sym \
		.libs/libvirt-gobject-1.0.so
else ! WITH_LINUX
check-symfile:
endif ! WITH_LINUX
check-symsorting:
	$(AM_V_GEN)$(PERL) $(top_srcdir)/build-aux/check-symsorting.pl \
		$(srcdir)/libvirt-gobject.sym
check-local: check-symfile check-symsorting

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =

if HAVE_INTROSPECTION
LibvirtGObject-1.0.gir: libvirt-gobject-1.0.la
LibvirtGObject_1_0_gir_INCLUDES = GObject-2.0 Gio-2.0
LibvirtGObject_1_0_gir_PACKAGES = gobject-2.0
LibvirtGObject_1_0_gir_EXPORT_PACKAGES = libvirt-gobject-1.0
LibvirtGObject_1_0_gir_LIBS = \
	$(top_builddir)/libvirt-glib/libvirt-glib-1.0.la \
	$(top_builddir)/libvirt-gconfig/libvirt-gconfig-1.0.la \
	libvirt-gobject-1.0.la
LibvirtGObject_1_0_gir_FILES = \
	$(GOBJECT_SOURCE_FILES) \
	$(GOBJECT_HEADER_FILES) \
	$(GOBJECT_GENERATED_FILES) \
	$(NULL)
LibvirtGObject_1_0_gir_CFLAGS = $(libvirt_gobject_1_0_la_CFLAGS)
LibvirtGObject_1_0_gir_SCANNERFLAGS = \
	--identifier-prefix=GVir --symbol-prefix=gvir \
	--c-include="libvirt-gobject/libvirt-gobject.h" \
	--include-uninstalled $(top_builddir)/libvirt-glib/LibvirtGLib-1.0.gir \
	--include-uninstalled $(top_builddir)/libvirt-gconfig/LibvirtGConfig-1.0.gir \
	--warn-all \
	$(NULL)
INTROSPECTION_GIRS += LibvirtGObject-1.0.gir
INTROSPECTION_COMPILER_ARGS = \
	--includedir=$(top_builddir)/libvirt-glib \
	--includedir=$(top_builddir)/libvirt-gconfig

girdir = $(datadir)/gir-1.0
gir_DATA = LibvirtGObject-1.0.gir

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = LibvirtGObject-1.0.typelib

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif # HAVE_INTROSPECTION
