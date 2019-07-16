package = SNESDev
version = 2.0
tarname = $(package)
distdir = $(tarname)-$(version)

prefix      = /usr/local
exec_prefix = $(prefix)
bindir      = $(exec_prefix)/bin
sysconfdir  = /etc
export prefix
export exec_prefix
export bindir
export sysconfdir

all clean check install uninstall SNESDev:
	cd libs && sudo dpkg -i wiringpi-latest.deb
	cd src && $(MAKE) $@

installservice uninstallservice:
	cd scripts && $(MAKE) $@

$(distdir): FORCE
	mkdir -p $(distdir)/src
	cp Makefile $(distdir)
	cp src/Makefile $(distdir)/src
	cp src/*.c $(distdir)/src
	cp src/*.h $(distdir)/src

FORCE:
	-rm $(distdir).tar.gz > /dev/null 2>&1
	-rm -rf $(distdir) > /dev/null 2>&1

.PHONY: FORCE all dist check clean dist distcheck install uninstall installservice uninstallservice
