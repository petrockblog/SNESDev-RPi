ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src doc
CLEANFILES = *~
EXTRA_DIST = examples/blink/blink.c \
examples/input/input.c \
examples/event/event.c \
examples/spi/spi.c \
examples/spin/spin.c 

upload:
	rsync -avz @PACKAGE_TARNAME@-@VERSION@.tar.gz doc/html/ server2:/var/www/html/mikem/@PACKAGE_NAME@


