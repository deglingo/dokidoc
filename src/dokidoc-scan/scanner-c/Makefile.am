#

scanners_LTLIBRARIES = dokidoc-scanner-c.la

dokidoc_scanner_c_la_CFLAGS = \
	-DCL_LOG_DOMAIN=\"dokidoc-scanner-c\" \
	-I$(top_srcdir)/src \
	@CLOG_CFLAGS@

dokidoc_scanner_c_la_LDFLAGS = -module

dokidoc_scanner_c_la_SOURCES = \
	scannercmain.c
