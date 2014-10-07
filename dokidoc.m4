#
AC_DEFUN([DOKIDOC_CHECK_PATH],
[
    AC_PATH_PROG([DOKIDOCIZE], [dokidocize], [no])
    test x"$DOKIDOCIZE" != x"no" || AC_ERROR([dokidocize not found])
    AC_PATH_PROG([DOKIDOC], [dokidoc], [no])
    test x"$DOKIDOC" != x"no" || AC_ERROR([dokidoc not found])
])
#
AC_DEFUN([DOKIDOC_INIT],
[
    AC_REQUIRE([DOKIDOC_CHECK_PATH])
    AC_CONFIG_FILES([$1/Makefile])
    AC_CONFIG_FILES([$1/dokidoc.conf])
])
