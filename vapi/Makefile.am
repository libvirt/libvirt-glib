NULL =

if WITH_VALA
vapidir = $(datadir)/vala/vapi
vapi_DATA =					\
	libvirt-gconfig-1.0.vapi		\
	libvirt-glib-1.0.vapi			\
	libvirt-gobject-1.0.deps		\
	libvirt-gobject-1.0.vapi		\
	$(NULL)

libvirt-glib-1.0.vapi: $(top_builddir)/libvirt-glib/LibvirtGLib-1.0.gir
	$(AM_V_GEN)$(VAPIGEN)			\
		--vapidir=$(builddir)		\
		--pkg gobject-2.0		\
		--pkg gio-2.0			\
		--library libvirt-glib-1.0	\
		$<

libvirt-gobject-1.0.vapi: $(top_builddir)/libvirt-gobject/LibvirtGObject-1.0.gir libvirt-glib-1.0.vapi
	$(AM_V_GEN)$(VAPIGEN)							 \
		--vapidir=$(builddir)						 \
		--pkg gobject-2.0						 \
		--pkg gio-2.0							 \
		--pkg libvirt-glib-1.0						 \
		--library libvirt-gobject-1.0					 \
		--metadatadir=$(srcdir)						 \
		--girdir=$(top_builddir)/libvirt-glib				 \
		--girdir=$(top_builddir)/libvirt-gconfig			 \
		$<

libvirt-gconfig-1.0.vapi: $(top_builddir)/libvirt-gconfig/LibvirtGConfig-1.0.gir
	$(AM_V_GEN)$(VAPIGEN)			\
		--vapidir=$(builddir)		\
		--pkg gobject-2.0		\
		--library libvirt-gconfig-1.0	\
		$<

endif

CLEANFILES =					\
	libvirt-gconfig-1.0.vapi		\
	libvirt-glib-1.0.vapi			\
	libvirt-gobject-1.0.vapi		\
	$(NULL)

EXTRA_DIST = libvirt-gobject-1.0.deps
