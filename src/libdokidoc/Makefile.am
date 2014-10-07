#

lib_LTLIBRARIES = libdokidoc.la

libdokidoc_la_CFLAGS = \
	-DCL_LOG_DOMAIN=\"libdokidoc\" \
	-I$(top_srcdir)/src \
	@CLOG_CFLAGS@ \
	@GMODULE_CFLAGS@

include_HEADERS = \
	libdokidoc.h

libdokidocinclude_HEADERS = \
	dokbase.h \
	dokscanner.h

libdokidoc_la_SOURCES = \
	dokscanner.c
