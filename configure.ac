
AC_INIT(libvirt-glib, 0.0.1)
AC_CONFIG_SRCDIR(src/events.c)
AC_CONFIG_AUX_DIR([build-aux])
AM_CONFIG_HEADER(config.h)
dnl Make automake keep quiet about wildcards & other GNUmake-isms
AM_INIT_AUTOMAKE([-Wno-portability])
AC_CANONICAL_HOST

LIBVIRT_REQUIRED=0.4.6
GLIB2_REQUIRED=2.10.0


LIBVIRT_GLIB_MAJOR_VERSION=`echo $VERSION | awk -F. '{print $1}'`
LIBVIRT_GLIB_MINOR_VERSION=`echo $VERSION | awk -F. '{print $2}'`
LIBVIRT_GLIB_MICRO_VERSION=`echo $VERSION | awk -F. '{print $3}'`
LIBVIRT_GLIB_VERSION=$LIBVIRT_GLIB_MAJOR_VERSION.$LIBVIRT_GLIB_MINOR_VERSION.$LIBVIRT_GLIB_MICRO_VERSION$LIBVIRT_GLIB_MICRO_VERSION_SUFFIX
LIBVIRT_GLIB_VERSION_INFO=`expr $LIBVIRT_GLIB_MAJOR_VERSION + $LIBVIRT_GLIB_MINOR_VERSION`:$LIBVIRT_GLIB_MICRO_VERSION:$LIBVIRT_GLIB_MINOR_VERSION
LIBVIRT_GLIB_VERSION_NUMBER=`expr $LIBVIRT_GLIB_MAJOR_VERSION \* 1000000 + $LIBVIRT_GLIB_MINOR_VERSION \* 1000 + $LIBVIRT_GLIB_MICRO_VERSION`

AC_SUBST([LIBVIRT_GLIB_MAJOR_VERSION])
AC_SUBST([LIBVIRT_GLIB_MINOR_VERSION])
AC_SUBST([LIBVIRT_GLIB_MICRO_VERSION])
AC_SUBST([LIBVIRT_GLIB_VERSION])
AC_SUBST([LIBVIRT_GLIB_VERSION_INFO])
AC_SUBST([LIBVIRT_GLIB_VERSION_NUMBER])

AC_PROG_CC
AM_PROG_CC_C_O

AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

AC_CONFIG_LIBOBJ_DIR([src])

AC_DEFINE([_GNU_SOURCE], [], [Enable GNU extensions])

LIBVIRT_GLIB_COMPILE_WARNINGS([maximum])

PKG_CHECK_MODULES(LIBVIRT, libvirt >= $LIBVIRT_REQUIRED)
PKG_CHECK_MODULES(GLIB2, glib-2.0 >= $GLIB2_REQUIRED)

AC_ARG_ENABLE([test-coverage],
[  --enable-test-coverage  turn on code coverage instrumentation],
[case "${enableval}" in
   yes|no) ;;
   *)      AC_MSG_ERROR([bad value ${enableval} for test-coverage option]) ;;
 esac],
              [enableval=no])
enable_coverage=$enableval

if test "${enable_coverage}" = yes; then
  gl_COMPILER_FLAGS(-fprofile-arcs)
  gl_COMPILER_FLAGS(-ftest-coverage)
  AC_SUBST([COVERAGE_CFLAGS], [$COMPILER_FLAGS])
  AC_SUBST([COVERAGE_LDFLAGS], [$COMPILER_FLAGS])
  COMPILER_FLAGS=
fi


AC_OUTPUT(Makefile
          include/Makefile
          include/libvirt-glib/Makefile
          src/Makefile
          examples/Makefile
          libvirt-glib.pc
          libvirt-glib.spec)
