#

scanners_LTLIBRARIES = dokidoc-scanner-c.la

dokidoc_scanner_c_la_CFLAGS = \
	-DCL_LOG_DOMAIN=\"dokidoc-scanner-c\" \
	-I$(top_srcdir)/src \
	@CLOG_CFLAGS@ \
	@LIBXML_CFLAGS@

dokidoc_scanner_c_la_LDFLAGS = -module

dokidoc_scanner_c_la_SOURCES = \
	scannercgram.y \
	lexer.c \
	cpp.c \
	scannercmain.c

AM_YFLAGS = -d

BUILT_SOURCES = scannercgram.h
