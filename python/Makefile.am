# Makefile for libvirt python library

INCLUDES = \
        $(WARN_CFLAGS) \
        -I$(PYTHON_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include \
	-I$(top_builddir)/$(subdir) \
	$(GLIB2_CFLAGS)

EXTRA_DIST = libvirtglib.py

if WITH_PYTHON
python_LTLIBRARIES = libvirtglibmod.la

libvirtglibmod_la_SOURCES = libvirt-glib.c
# Python header files contain a redundant decl, hence:
libvirtglibmod_la_CFLAGS = -Wno-redundant-decls

libvirtglibmod_la_LDFLAGS = -module -avoid-version \
	$(CYGWIN_EXTRA_LDFLAGS)
libvirtglibmod_la_LIBADD = ../src/libvirt-glib.la
	$(CYGWIN_EXTRA_LIBADD) $(CYGWIN_EXTRA_PYTHON_LIBADD)


install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(pythondir)
	@INSTALL@ -m 0644 $(srcdir)/libvirtglib.py $(DESTDIR)$(pythondir)

uninstall-local:
	rm -f $(DESTDIR)$(pythondir)/libvirtglib.py


else
all:
endif

