
EXTRA_DIST = libvirt-gobject.sym

lib_LTLIBRARIES = libvirt-gobject-1.0.la

libvirt_gobject_1_0_ladir = $(includedir)/libvirt-gobject
libvirt_gobject_1_0_la_HEADERS = \
			libvirt-gobject.h \
			libvirt-gobject-main.h \
			libvirt-gobject-connection.h \
			libvirt-gobject-domain.h \
			libvirt-gobject-enums.h
libvirt_gobject_1_0_la_SOURCES = \
			$(libvirt_gobject_1_0_la_HEADERS) \
			libvirt-gobject-main.c \
			libvirt-gobject-connection.c \
			libvirt-gobject-domain.c \
			libvirt-gobject-enums.c
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

libvirt-gobject-enums.c: libvirt-gobject-domain.h libvirt-gobject-connection.h
	$(AM_V_GEN)glib-mkenums    --fhead "#include <glib-object.h>\n" \
                        --fhead "#include \"libvirt-gobject/libvirt-gobject-enums.h\"\n\n" \
                        --fhead "#include \"libvirt-gobject/libvirt-gobject-domain.h\"\n\n" \
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
	$(AM_V_GEN)$(G_IR_SCANNER) -v \
                --namespace LibvirtGObject \
                --nsversion 1.0 \
                --include GObject-2.0 \
                --include Gio-2.0 \
                --strip-prefix=gvir \
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

CLEANFILES += $(gir_DATA) $(typelibs_DATA)

endif # WITH_GOBJECT_INTROSPECTION
