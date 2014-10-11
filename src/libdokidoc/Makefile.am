#

lib_LTLIBRARIES = libdokidoc.la

libdokidoc_la_CFLAGS = \
	-DCL_LOG_DOMAIN=\"libdokidoc\" \
	-I$(top_srcdir)/src \
	@CLOG_CFLAGS@ \
	@GMODULE_CFLAGS@ \
	@LIBXML_CFLAGS@

include_HEADERS = \
	libdokidoc.h

libdokidocinclude_HEADERS = \
	dokbase.h \
	dokast.h \
	dokscanner.h \
	doktree.h

libdokidoc_la_SOURCES = \
	dokbase.c \
	dokast.c \
	dokscanner.c \
	doktree.c
