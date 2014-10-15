#

bin_PROGRAMS = dokidoc-filter

dokidoc_filter_CFLAGS = -DCL_LOG_DOMAIN=\"dokidoc-filter\" -I$(top_srcdir)/src @CLOG_CFLAGS@ @LIBXML_CFLAGS@

dokidoc_filter_LDFLAGS = $(top_builddir)/src/libdokidoc/libdokidoc.la @CLOG_LIBS@ @GMODULE_LIBS@ @LIBXML_LIBS@

dokidoc_filter_SOURCES = \
	filtmain.c
