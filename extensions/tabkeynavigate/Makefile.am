extensiondir = $(EXTENSIONS_DIR)
extension_LTLIBRARIES = libtabkeynavigateextension.la

libtabkeynavigateextension_la_SOURCES = \
	ephy-tabkeynavigate-extension.c	\
	ephy-tabkeynavigate-extension.h	\
	tabkeynavigate.c

libtabkeynavigateextension_la_CPPFLAGS = \
        -I$(top_srcdir)/include				\
	-DSHARE_DIR=\"$(pkgdatadir)\"			\
	$(AM_CPPFLAGS)

libtabkeynavigateextension_la_CFLAGS = \
	$(EPIPHANY_DEPENDENCY_CFLAGS)	\
	$(AM_CFLAGS)

libtabkeynavigateextension_la_LDFLAGS = \
	-module -avoid-version \
	-export-symbols $(top_srcdir)/ephy-extension.symbols \
	$(AM_LDFLAGS)

extensioninidir = $(extensiondir)
extensionini_in_files = tabkeynavigate.ephy-extension.in
extensionini_DATA = $(extensionini_in_files:.ephy-extension.in=.ephy-extension)

%.ephy-extension: %.ephy-extension.in $(extension_LTLIBRARIES)
	sed -e "s|%LIBRARY%|`. ./$(extension_LTLIBRARIES) && echo $$dlname`|" \
	    -e "s|%EXTENSION_DIR%|$(extensiondir)|" \
	$< > $@


CLEANFILES = $(extensionini_DATA)
DISTCLEANFILES = $(extensionini_DATA)

EXTRA_DIST = $(extensionini_in_files)
