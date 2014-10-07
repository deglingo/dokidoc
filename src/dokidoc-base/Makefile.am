#

all-local: dokidoc-sysconf.h

BUILT_SOURCES = dokidoc-sysconf.h

dokidoc-sysconf.h: $(top_builddir)/config.status
	(	echo "#ifndef _DOKIDOC_SYSCONF_H_"; \
		echo "#define _DOKIDOC_SYSCONF_H_"; \
		echo "#define DOKIDOC_SCANNERSDIR \"$(scannersdir)\""; \
		echo "#endif"; \
	) >$@.tmp
	mv -f $@.tmp $@