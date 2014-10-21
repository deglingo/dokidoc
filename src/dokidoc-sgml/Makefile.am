#

bin_PROGRAMS = dokidoc-sgml

dokidoc_sgml_CFLAGS = -DCL_LOG_DOMAIN=\"dokidoc-sgml\" -I$(top_srcdir)/src @CLOG_CFLAGS@ @GMODULE_CFLAGS@ @LIBXML_CFLAGS@

dokidoc_sgml_LDFLAGS = $(top_builddir)/src/libdokidoc/libdokidoc.la @CLOG_LIBS@ @GMODULE_LIBS@ @LIBXML_LIBS@

dokidoc_sgml_SOURCES = \
	sgmlmain.c
