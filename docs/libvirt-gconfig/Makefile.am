
DOC_MODULE=Libvirt-gconfig

DOC_MAIN_SGML_FILE=$(DOC_MODULE)-docs.xml

# Must not use $(top_srcdir) since gtkdoc-scan runs
# from the srcdir already, not the builddir
DOC_SOURCE_DIR=../../libvirt-gconfig

SCANGOBJ_OPTIONS=

SCAN_OPTIONS=--rebuild-sections --rebuild-types

MKDB_OPTIONS=--sgml-mode --output-format=xml

MKTMPL_OPTIONS=

MKHTML_OPTIONS=

FIXXREF_OPTIONS=

HFILE_GLOB=$(top_srcdir)/libvirt-gconfig/*.h
CFILE_GLOB=$(top_srcdir)/libvirt-gconfig/*.c

IGNORE_HFILES=

HTML_IMAGES=

content_files=

expand_content_files=

GTKDOC_CFLAGS= \
	$(GOBJECT2_CFLAGS) \
	$(LIBXML2_CFLAGS)
GTKDOC_LIBS = \
	$(GOBJECT2_LIBS) \
	$(LIBXML2_LIBS) \
	$(top_builddir)/libvirt-gconfig/libvirt-gconfig-1.0.la

include $(top_srcdir)/gtk-doc.make
