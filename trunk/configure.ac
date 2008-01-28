#                                               -*- Autoconf -*-

AC_PREREQ(2.61)
AC_INIT(The Carom Library, 0.2, tavianator@gmail.com, libcarom)
AM_INIT_AUTOMAKE(libcarom, 0.2)

# Programs
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Libraries.
AC_CHECK_LIB([gmp], [__gmpz_init], , [AC_MSG_ERROR([GMP not found])])
AC_CHECK_LIB([mpfr], [mpfr_init], , [AC_MSG_ERROR([MPFR not found])])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 doc/Makefile])
AC_OUTPUT
