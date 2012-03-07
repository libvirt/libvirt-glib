
DOC_MODULE=Libvirt-gobject

DOC_MAIN_SGML_FILE=$(DOC_MODULE)-docs.xml

DOC_SOURCE_DIR=$(top_srcdir)/libvirt-gobject

SCANGOBJ_OPTIONS=

SCAN_OPTIONS=--rebuild-sections --rebuild-types

MKDB_OPTIONS=--sgml-mode --output-format=xml

MKTMPL_OPTIONS=

MKHTML_OPTIONS=

FIXXREF_OPTIONS=

HFILE_GLOB=$(top_srcdir)/libvirt-gobject/*.h
CFILE_GLOB=$(top_srcdir)/libvirt-gobject/*.c

IGNORE_HFILES=

HTML_IMAGES=

content_files=

expand_content_files=

GTKDOC_CFLAGS= \
	$(LIBVIRT_CFLAGS) \
	$(GOBJECT2_CFLAGS) \
	$(GIO2_CFLAGS)
GTKDOC_LIBS = \
	$(LIBVIRT_LIBS) \
	$(GOBJECT2_LIBS) \
	$(GIO2_LIBS) \
	$(top_builddir)/libvirt-gobject/libvirt-gobject-1.0.la

include $(top_srcdir)/gtk-doc.make
