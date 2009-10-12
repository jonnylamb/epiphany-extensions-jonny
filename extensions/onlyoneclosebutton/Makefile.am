SUBDIRS=egg

extensiondir = $(EXTENSIONS_DIR)
extension_LTLIBRARIES = libonlyoneclosebuttonextension.la

libonlyoneclosebuttonextension_la_SOURCES = \
	ephy-onlyoneclosebutton-extension.c	\
	ephy-onlyoneclosebutton-extension.h	\
	onlyoneclosebutton.c

libonlyoneclosebuttonextension_la_CPPFLAGS = \
        -I$(top_srcdir)/include				\
	-I$(top_srcdir)/extensions/onlyoneclosebutton/egg \
	-DSHARE_DIR=\"$(pkgdatadir)\"			\
	$(AM_CPPFLAGS)

libonlyoneclosebuttonextension_la_CFLAGS = \
	$(EPIPHANY_DEPENDENCY_CFLAGS)	\
	$(AM_CFLAGS)

libonlyoneclosebuttonextension_la_LDFLAGS = \
	-module -avoid-version \
	-export-symbols $(top_srcdir)/ephy-extension.symbols \
	-$(top_builddir)/extensions/onlyoneclosebutton/egg/libegg.la \
	$(AM_LDFLAGS)

extensioninidir = $(extensiondir)
extensionini_in_files = onlyoneclosebutton.ephy-extension.in
extensionini_DATA = $(extensionini_in_files:.ephy-extension.in=.ephy-extension)

%.ephy-extension: %.ephy-extension.in $(extension_LTLIBRARIES)
	sed -e "s|%LIBRARY%|`. ./$(extension_LTLIBRARIES) && echo $$dlname`|" \
	    -e "s|%EXTENSION_DIR%|$(extensiondir)|" \
	$< > $@


CLEANFILES = $(extensionini_DATA)
DISTCLEANFILES = $(extensionini_DATA)

EXTRA_DIST = $(extensionini_in_files)
