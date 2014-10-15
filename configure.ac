AC_INIT([dokidoc], [0.0.0])
AM_INIT_AUTOMAKE

: ${CFLAGS:=-g -O0 -Wall}

AC_SUBST([aclocaldir], ['${datadir}/aclocal'])
AC_SUBST([libdokidocincludedir], ['${includedir}/libdokidoc'])
AC_SUBST([scannersdir], ['${pkglibexecdir}/scannersdir'])

AC_PROG_CC
AM_PROG_CC_C_O
LT_INIT
AC_PROG_YACC


# [fixme] check for python3
AC_PATH_PROG([PYTHON], [python3])
test x"$PYTHON" != x || AC_ERROR([python3 not found])

PKG_CHECK_MODULES([GMODULE], [gmodule-2.0])
PKG_CHECK_MODULES([CLOG], [clog])

# libxml
AC_SUBST([LIBXML_CFLAGS])
AC_SUBST([LIBXML_LIBS])
AC_PATH_PROG([XML_CONFIG], [xml2-config])
LIBXML_CFLAGS=`$XML_CONFIG --cflags`
LIBXML_LIBS=`$XML_CONFIG --libs`

AC_CONFIG_FILES([ \
  Makefile \
  src/Makefile \
  src/dokidoc/Makefile \
  src/libdokidoc/Makefile \
  src/dokidoc-base/Makefile \
  src/dokidoc-scan/Makefile \
  src/dokidoc-scan/scanner-c/Makefile \
  src/dokidoc-filter/Makefile \
  tools/Makefile \
  test/Makefile \
])
AC_CONFIG_FILES([tools/dokidocize], [chmod +x tools/dokidocize])
AC_CONFIG_FILES([src/dokidoc/dokidoc], [chmod +x src/dokidoc/dokidoc])
AC_CONFIG_FILES([test/dokidoc-test-scan], [chmod +x test/dokidoc-test-scan])
AC_CONFIG_FILES([test/dokidoc-test-scan.conf])
AC_OUTPUT
