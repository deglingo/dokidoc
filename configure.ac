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

#
AC_SUBST([DOKIDOCIZE], ['${top_builddir}/tools/dokidocize'])
AC_SUBST([DOKIDOC_SCAN], ['${top_builddir}/src/dokidoc-scan/dokidoc-scan'])
AC_SUBST([DOKIDOC_UPDATE], ['${top_builddir}/src/dokidoc-update/dokidoc-update'])
AC_SUBST([DOKIDOC_SGML], ['${top_builddir}/src/dokidoc-sgml/dokidoc-sgml'])

AC_CONFIG_FILES([ \
  Makefile \
  src/Makefile \
  src/dokidoc/Makefile \
  src/libdokidoc/Makefile \
  src/dokidoc-base/Makefile \
  src/dokidoc-scan/Makefile \
  src/dokidoc-scan/scanner-c/Makefile \
  src/dokidoc-update/Makefile \
  src/dokidoc-filter/Makefile \
  src/dokidoc-sgml/Makefile \
  tools/Makefile \
  test/Makefile \
  test/src/Makefile \
  test/docs/Makefile \
])
AC_CONFIG_FILES([tools/dokidocize], [chmod +x tools/dokidocize])
AC_CONFIG_FILES([src/dokidoc/dokidoc], [chmod +x src/dokidoc/dokidoc])
AC_CONFIG_FILES([test/dokidoc-test], [chmod +x test/dokidoc-test])
AC_CONFIG_FILES([test/docs/dokidoc.conf])
AC_OUTPUT
