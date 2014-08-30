package = SNESDev
version = 1.0
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
	cd src && $(MAKE) $@

installservice uninstallservice:
	cd scripts && $(MAKE) $@

dist: $(distdir).tar.gz

distcheck: $(distdir).tar.gz
	gzip -cd $(distdir).tar.gz | tar xvf -
	cd $(distdir) && $(MAKE) all
	cd $(distdir) && $(MAKE) check
	cd $(distdir) && $(MAKE) install DESTDIR=$${PWD}/_inst install
	cd $(distdir) && $(MAKE) install DESTDIR=$${PWD}/_inst uninstall
	@remaining="`find $${PWD}/$(distdir)/_inst/ -type f | wc -l`"; \
	if test "$${remaining}" -ne 0; then \
		echo "$${remaining} file(s) in the stage directory!"; \
		exit 1; \
	fi
	cd $(distdir) && $(MAKE) clean
	rm -rf $(distdir)
	@echo "*** Package $(distdir).tar.gz is ready for distribution"

$(distdir).tar.gz: $(distdir)
	tar chof - $(distdir) | gzip -9 -c > $@
	rm -rf $(distdir)

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
