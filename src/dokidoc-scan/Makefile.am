#

SUBDIRS = scanner-c

bin_PROGRAMS = dokidoc-scan

dokidoc_scan_CFLAGS = \
	-DCL_LOG_DOMAIN=\"dokidoc-scan\" \
	-I$(top_srcdir)/src \
	-I$(top_builddir)/src \
	@CLOG_CFLAGS@

dokidoc_scan_LDFLAGS = \
	$(top_builddir)/src/libdokidoc/libdokidoc.la \
	@CLOG_LIBS@ \
	@GMODULE_LIBS@

EXTRA_dokidoc_scan_DEPENDENCIES = $(top_builddir)/src/libdokidoc/libdokidoc.la

dokidoc_scan_SOURCES = \
	scanmain.c
