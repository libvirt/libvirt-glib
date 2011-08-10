
EXTRA_DIST = libvirt-gobject.sym

lib_LTLIBRARIES = libvirt-gobject-1.0.la

GOBJECT_HEADER_FILES = \
			libvirt-gobject.h \
			libvirt-gobject-main.h \
			libvirt-gobject-domain-snapshot.h \
			libvirt-gobject-domain.h \
			libvirt-gobject-interface.h \
			libvirt-gobject-network.h \
			libvirt-gobject-network-filter.h \
			libvirt-gobject-node-device.h \
			libvirt-gobject-secret.h \
			libvirt-gobject-storage-vol.h \
			libvirt-gobject-storage-pool.h \
			libvirt-gobject-stream.h \
			libvirt-gobject-connection.h \
			libvirt-gobject-manager.h
GOBJECT_SOURCE_FILES = \
			libvirt-gobject-main.c \
			libvirt-gobject-domain-snapshot.c \
			libvirt-gobject-domain.c \
			libvirt-gobject-interface.c \
			libvirt-gobject-network.c \
			libvirt-gobject-network-filter.c \
			libvirt-gobject-node-device.c \
			libvirt-gobject-secret.c \
			libvirt-gobject-storage-vol.c \
			libvirt-gobject-storage-pool.c \
			libvirt-gobject-stream.c \
			libvirt-gobject-connection.c \
			libvirt-gobject-manager.c

libvirt_gobject_1_0_ladir = $(includedir)/libvirt-gobject-1.0/libvirt-gobject
libvirt_gobject_1_0_la_HEADERS = \
			libvirt-gobject-enums.h \
			$(GOBJECT_HEADER_FILES)
libvirt_gobject_1_0_la_SOURCES = \
			$(libvirt_gobject_1_0_la_HEADERS) \
			libvirt-gobject-enums.c \
			$(GOBJECT_SOURCE_FILES)
libvirt_gobject_1_0_la_CFLAGS = \
			-DDATADIR="\"$(datadir)\"" \
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

libvirt-gobject-enums.c: libvirt-gobject-domain.h libvirt-gobject-connection.h
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

libvirt-gobject-enums.h: libvirt-gobject-domain.h libvirt-gobject-connection.h
	$(AM_V_GEN)glib-mkenums    --fhead "#ifndef __LIBVIRT_GOBJECT_ENUMS_H__\n" \
                        --fhead "#define __LIBVIRT_GOBJECT_ENUMS_H__\n\n" \
                        --fhead "G_BEGIN_DECLS\n\n" \
                        --ftail "G_END_DECLS\n\n" \
                        --ftail "#endif /* __LIBVIRT_GOBJECT_ENUMS_H__ */\n" \
                        --eprod "#define GVIR_TYPE_@ENUMSHORT@ @enum_name@_get_type()\n" \
                        --eprod "GType @enum_name@_get_type (void);\n" \
                $^ | sed -e 's/g_vir/gvir/g' > $@

BUILT_SOURCES = libvirt-gobject-enums.c libvirt-gobject-enums.h

CLEANFILES = $(BUILT_SOURCES)

if WITH_GOBJECT_INTROSPECTION

LibvirtGObject-1.0.gir: libvirt-gobject-1.0.la $(G_IR_SCANNER) Makefile.am
	$(AM_V_GEN)$(G_IR_SCANNER) \
                --quiet \
                --warn-all \
                --namespace LibvirtGObject \
                --nsversion 1.0 \
                --include GObject-2.0 \
                --include Gio-2.0 \
                --include LibvirtGLib-1.0 \
                --include LibvirtGConfig-1.0 \
                --identifier-prefix=GVir \
                --symbol-prefix=gvir \
                --library=$(builddir)/libvirt-gobject-1.0.la \
                --add-include-path=$(top_builddir)/libvirt-glib \
                --add-include-path=$(top_builddir)/libvirt-gconfig \
                --output $@ \
                -I$(top_srcdir) \
                -I$(top_builddir) \
                --verbose \
                --pkg=gobject-2.0 \
                --pkg=gthread-2.0 \
                $(srcdir)/libvirt-gobject.h \
                $(GOBJECT_SOURCE_FILES:%=$(srcdir)/%) \
                $(GOBJECT_HEADER_FILES:%=$(srcdir)/%) \
                $(builddir)/libvirt-gobject-enums.c

girdir = $(datadir)/gir-1.0
gir_DATA = LibvirtGObject-1.0.gir

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = LibvirtGObject-1.0.typelib

%.typelib: %.gir
	$(AM_V_GEN)$(G_IR_COMPILER) \
                --includedir=$(top_builddir)/libvirt-glib \
                --includedir=$(top_builddir)/libvirt-gconfig \
                --includedir=$(builddir) \
                --includedir=$(girdir) \
                -o $@ $<

CLEANFILES += $(gir_DATA) $(typelibs_DATA)

endif # WITH_GOBJECT_INTROSPECTION
