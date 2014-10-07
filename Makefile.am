#

SUBDIRS = src tools

aclocal_DATA = dokidoc.m4

.PHONY: listdirs

listdirs:
	@( \
		fmt='%-16s : %-20s\n'; \
		printf '====  INSTALLATION DIRECTORIES  ====\n'; \
		printf "$$fmt" 'prefix' "$(prefix)"; \
		printf "$$fmt" 'exec_prefix' "$(exec_prefix)"; \
		printf "$$fmt" 'bindir' "$(bindir)"; \
		printf "$$fmt" 'libdir' "$(libdir)"; \
		printf "$$fmt" 'pkglibdir' "$(pkglibdir)"; \
		printf "$$fmt" 'libexecdir' "$(libexecdir)"; \
		printf "$$fmt" 'pkglibexecdir' "$(pkglibexecdir)"; \
		printf "$$fmt" 'scannersdir' "$(scannersdir)"; \
		printf "$$fmt" 'vgsuppdir' "$(vgsuppdir)"; \
		printf "$$fmt" 'datadir' "$(datadir)"; \
		printf "$$fmt" 'pkgdatadir' "$(pkgdatadir)"; \
		printf '====================================\n'; \
	)
