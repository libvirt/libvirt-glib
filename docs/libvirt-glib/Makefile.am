
DOC_MODULE=Libvirt-glib

DOC_MAIN_SGML_FILE=$(DOC_MODULE)-docs.xml

# Must not use $(top_srcdir) since gtkdoc-scan runs
# from the srcdir already, not the builddir
DOC_SOURCE_DIR=../../libvirt-glib

SCANGOBJ_OPTIONS=

SCAN_OPTIONS=--rebuild-sections --rebuild-types

MKDB_OPTIONS=--sgml-mode --output-format=xml

MKTMPL_OPTIONS=

MKHTML_OPTIONS=

FIXXREF_OPTIONS=

HFILE_GLOB=$(top_srcdir)/libvirt-glib/*.h
CFILE_GLOB=$(top_srcdir)/libvirt-glib/*.c

IGNORE_HFILES=

HTML_IMAGES=

content_files=

expand_content_files=

GTKDOC_CFLAGS= \
	$(LIBVIRT_CFLAGS) \
	$(GLIB2_CFLAGS) \
	$(GTHREAD2_CFLAGS)
GTKDOC_LIBS = \
	$(LIBVIRT_LIBS) \
	$(GLIB2_LIBS) \
	$(GTHREAD2_LIBS) \
	$(top_builddir)/libvirt-glib/libvirt-glib-1.0.la

include $(top_srcdir)/gtk-doc.make

