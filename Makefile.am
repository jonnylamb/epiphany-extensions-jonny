SUBDIRS = extensions

ACLOCAL_AMFLAGS = -I m4

NULL =

EXTRA_DIST =			\
	autogen.sh		\
	COPYING.README		\
	README 			\
	ephy-extension.symbols	\
	$(NULL)

DISTCHECK_CONFIGURE_FLAGS = \
	--with-extensions=all	\
	--disable-scrollkeeper		\
	--disable-schemas-install
