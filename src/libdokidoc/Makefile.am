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
	doktype.h \
	dokast.h \
	dokvisitor.h \
	dokastvisitor.h \
	dokastdumper.h \
	dokastprocessor.h \
	dokscanner.h \
	doktree.h

libdokidoc_la_SOURCES = \
	dokbase.c \
	doktype.c \
	dokast.c \
	dokvisitor.c \
	dokastvisitor.c \
	dokastdumper.c \
	dokastprocessor.c \
	dokscanner.c \
	doktree.c
