#

bin_PROGRAMS = dokidoc-update

dokidoc_update_CFLAGS = -DCL_LOG_DOMAIN=\"dokidoc-update\" -I$(top_srcdir)/src @CLOG_CFLAGS@ @LIBXML_CFLAGS@

dokidoc_update_LDFLAGS = $(top_builddir)/src/libdokidoc/libdokidoc.la @CLOG_LIBS@ @GMODULE_LIBS@ @LIBXML_LIBS@

dokidoc_update_SOURCES = \
	updatemain.c
