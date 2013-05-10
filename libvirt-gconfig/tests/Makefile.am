noinst_PROGRAMS = test-domain-create test-domain-parse test-capabilities-parse

AM_CFLAGS = \
		$(GOBJECT2_CFLAGS) \
		$(LIBXML2_CFLAGS) \
		$(WARN_CFLAGS)
AM_CPPFLAGS = -I$(top_srcdir)
LDADD = \
		$(top_builddir)/libvirt-gconfig/libvirt-gconfig-1.0.la \
		$(GOBJECT2_LIBS) \
		$(LIBXML2_LIBS) \
		$(CYGWIN_EXTRA_LIBADD)

test_domain_create_SOURCES = test-domain-create.c

test_domain_parse_SOURCES = test-domain-parse.c

test_capabilities_parse_SOURCES = test-capabilities-parse.c
