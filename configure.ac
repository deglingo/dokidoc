AC_INIT([dokidoc], [0.0.0])
AM_INIT_AUTOMAKE

: ${CFLAGS:=-g -O0 -Wall}

AC_SUBST([aclocaldir], ['${datadir}/aclocal'])

AC_PROG_CC
AM_PROG_CC_C_O
LT_INIT


# [fixme] check for python3
AC_PATH_PROG([PYTHON], [python3])
test x"$PYTHON" != x || AC_ERROR([python3 not found])

PKG_CHECK_MODULES([CLOG], [clog])

AC_CONFIG_FILES([ \
  Makefile \
  src/Makefile \
  src/dokidoc/Makefile \
  src/dokidoc-scan/Makefile \
  tools/Makefile \
])
AC_CONFIG_FILES([tools/dokidocize], [chmod +x tools/dokidocize])
AC_CONFIG_FILES([src/dokidoc/dokidoc], [chmod +x src/dokidoc/dokidoc])
AC_OUTPUT
