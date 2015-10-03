#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([reflektor], [0.1a], [kbeckman@users.sf.net])
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE([no-define dist-bzip2])

AC_CONFIG_SRCDIR([src/mirror.c])
AC_CONFIG_HEADER([config.h])
AC_COPYRIGHT([Copyright 2009 Keith Beckman])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h stdlib.h string.h strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([strerror])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
