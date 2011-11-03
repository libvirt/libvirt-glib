
SUBDIRS = libvirt-glib libvirt-gconfig libvirt-gobject python vapi examples docs

ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libvirt-glib-1.0.pc libvirt-gconfig-1.0.pc libvirt-gobject-1.0.pc

EXTRA_DIST = @PACKAGE@.spec $(pkgconfig_DATA:%.pc=%.pc.in)

DISTCLEAN_FILES = @PACKAGE@.spec $(pkgconfig_DATA)

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc=yes --enable-introspection=yes
