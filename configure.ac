m4_define([e_e_version_major],[0])
m4_define([e_e_version_minor],[0])
m4_define([e_e_version_micro],[1])
m4_define([e_e_version],[e_e_version_major.e_e_version_minor.e_e_version_micro])

AC_INIT([epiphany-extensions-jonny],[e_e_version],[jonny@debian.org],[epiphany-extensions-jonny])

EPIPHANY_API_VERSION=2.28

EPIPHANY_EXTENSIONS_MAJOR=e_e_version_major.e_e_version_minor
AC_SUBST(EPIPHANY_EXTENSIONS_MAJOR)

AC_PREREQ([2.59])

AC_REVISION([$Revision: 1.235 $])

AC_CONFIG_SRCDIR([extensions])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.9 foreign dist-bzip2 no-dist-gzip tar-ustar])

# Use AM_SILENT_RULES if present
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

if test -z "$enable_maintainer_mode"; then
  enable_maintainer_mode=yes
fi
AM_MAINTAINER_MODE([enable])

AM_DISABLE_STATIC
AC_ENABLE_SHARED(yes)
AC_ENABLE_STATIC(no)

AM_PROG_LIBTOOL

AC_ISC_POSIX
AC_PROG_CC

AC_PROG_CXX
AC_HEADER_STDC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

AC_PATH_PROG([GLIB_GENMARSHAL], [glib-genmarshal])
AC_PATH_PROG(GCONFTOOL, [gconftool-2])
AC_PATH_PROG([GLIB_GENMARSHAL], [glib-genmarshal])
AC_PATH_PROG([GLIB_MKENUMS],[glib-mkenums])

AM_GCONF_SOURCE_2

AC_SUBST([AM_CXXFLAGS])

GNOME_COMMON_INIT
GNOME_DEBUG_CHECK
GNOME_COMPILE_WARNINGS([maximum])
GNOME_CXX_WARNINGS
GNOME_MAINTAINER_MODE_DEFINES

MORE_WARN_FLAGS=
DEPRECATION_FLAGS=

if test "x$enable_maintainer_mode" = "xyes"; then
	AC_DEFINE([MAINTAINER_MODE],[1],[Define to enable 'maintainer-only' behaviour])
	enable_debug=yes
        DEPRECATION_FLAGS="-DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED -DGCONF_DISABLE_DEPRECATED -DLIBGLADE_DISABLE_DEPRECATED -DPANGO_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED -DGNOME_DISABLE_DEPRECATED -DG_DISABLE_SINGLE_INCLUDES -DGTK_DISABLE_SINGLE_INCLUDES"
fi

GETTEXT_PACKAGE=epiphany-extensions-jonny
AC_SUBST([GETTEXT_PACKAGE])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE],["$GETTEXT_PACKAGE"],[Gettext package])

# ********************************
# pkg config checks
# ********************************

EPIPHANY_REQUIRED=2.27.4
LIBGLIB_REQUIRED=2.15.5
LIBGTK_REQUIRED=2.11.6

AC_SUBST([EPIPHANY_REQUIRED])
AC_SUBST([LIBGLIB_REQUIRED])
AC_SUBST([LIBGTK_REQUIRED])


EPIPHANY_EXTENSION_INIT([$EPIPHANY_API_VERSION], [$EPIPHANY_REQUIRED])

PKG_CHECK_MODULES([EPIPHANY_DEPENDENCY], [\
		  glib-2.0 >= $LIBGLIB_REQUIRED \
		  gio-2.0 >= $LIBGLIB_REQUIRED \
		  gmodule-2.0 \
		  gtk+-2.0 >= $LIBGTK_REQUIRED \
		  epiphany-$EPIPHANY_API_VERSION >= $EPIPHANY_REQUIRED])
AC_SUBST([EPIPHANY_DEPENDENCY_CFLAGS])
AC_SUBST([EPIPHANY_DEPENDENCY_LIBS])

PKG_CHECK_MODULES([WEBKIT], [webkit-1.0])
AC_SUBST([WEBKIT_CFLAGS])
AC_SUBST([WEBKIT_LIBS])

# TODO remove the EXTENSIONS_DIR var
EXTENSIONS_DIR=$EPIPHANY_EXTENSIONS_DIR
AC_SUBST([EXTENSIONS_DIR])

# *****************
# Add warning flags
# *****************

AM_CPPFLAGS="$AM_CPPFLAGS $DEPRECATION_FLAGS"
AM_CFLAGS="$AM_CFLAGS $WARN_CFLAGS"
AM_CXXFLAGS="$AM_CXXFLAGS $WARN_CXXFLAGS"
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_CXXFLAGS])

# *******************************
# share/data dir
# *******************************

if test "x${datadir}" = 'x${prefix}/share'; then
  if test "x${prefix}" = "xNONE"; then
    AC_DEFINE_UNQUOTED([EXTENSIONS_DATA_DIR], ["${ac_default_prefix}/share/${PACKAGE}-${EPIPHANY_EXTENSIONS_MAJOR}"], [Directory of data files])
  else
    AC_DEFINE_UNQUOTED([EXTENSIONS_DATA_DIR], ["${prefix}/share/${PACKAGE}-${EPIPHANY_EXTENSIONS_MAJOR}"], [Directory of data files])
  fi
else
  AC_DEFINE_UNQUOTED([EXTENSIONS_DATA_DIR], ["${datadir}/${PACKAGE}-${EPIPHANY_EXTENSIONS_MAJOR}"], [Directory of data files])
fi

# *******************************
# *******************************

AC_CONFIG_FILES([
Makefile
extensions/Makefile
extensions/confirmwindowclose/Makefile
extensions/onlyoneclosebutton/Makefile
extensions/onlyoneclosebutton/egg/Makefile
extensions/tabkeynavigate/Makefile
extensions/middleclickclose/Makefile
])

AC_OUTPUT
