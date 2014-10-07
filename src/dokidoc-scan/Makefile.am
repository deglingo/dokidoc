#

SUBDIRS = scanner-c

bin_PROGRAMS = dokidoc-scan

dokidoc_scan_CFLAGS = \
	-DCL_LOG_DOMAIN=\"dokidoc-scan\" \
	-I$(top_srcdir)/src \
	-I$(top_builddir)/src \
	@CLOG_CFLAGS@ \
	@GMODULE_LIBS@

dokidoc_scan_LDFLAGS = @CLOG_LIBS@ @GMODULE_LIBS@

dokidoc_scan_SOURCES = \
	scanmain.c
