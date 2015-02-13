CC=gcc
SRCDIR=.
BUILDDIR=../build
CFLAGS=-Wall -O3
LIBBCM2835_DIR=../libs/bcm2835-1.39
LIBBCM2835_INCDIR=$(LIBBCM2835_DIR)/src
LIBBCM2835_LIBDIR=$(LIBBCM2835_DIR)/src
LIBCONFUSE_DIR=/usr/local
LIBCONFUSE_INCDIR=$(LIBCONFUSE_DIR)/include
LIBCONFUSE_LIBDIR=$(LIBCONFUSE_DIR)/lib
LIBS=-lbcm2835 -L$(LIBBCM2835_LIBDIR) -I$(LIBBCM2835_INCDIR) -lrt -lconfuse -L$(LIBCONFUSE_LIBDIR) -I$(LIBCONFUSE_INCDIR)

TARGET := SNESDev
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPS := $(OBJECTS:.o=.deps)

all $(TARGET): $(LIBBCM2835_LIBDIR)/libbcm2835.a $(LIBCONFUSE_LIBDIR)/libconfuse.a $(OBJECTS)
	@echo " Linking..."; $(CC) $^ $(LIBS) -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -I$(LIBBCM2835_INCDIR) -I$(LIBCONFUSE_INCDIR) -MD -MF $(@:.o=.deps) -c -o $@ $<

$(LIBBCM2835_LIBDIR)/libbcm2835.a:
	cd ../libs && tar zxvf bcm2835-1.39.tar.gz
	cd ../libs/bcm2835-1.39/ && ./configure
	cd ../libs/bcm2835-1.39/ && $(MAKE)

$(LIBCONFUSE_LIBDIR)/libconfuse.a:
	cd ../libs/confuse-2.7/ && ./configure
	cd ../libs/confuse-2.7/ && $(MAKE)
	cd ../libs/confuse-2.7/ && $(MAKE) install

check:
	# TO BE DONE! JUST A DUMMY FOR NOW
	@echo "*** NO TESTS IMPLEMENTED ***"

clean:
	@echo " Cleaning..."; $(RM) -r $(BUILDDIR) $(TARGET)

-include $(DEPS)

install:
	install -d $(DESTDIR)$(bindir)
	install -m 0755 $(TARGET) $(DESTDIR)$(bindir)
	install -m 0755 ../supplementary/snesdev.cfg $(DESTDIR)/$(sysconfdir)

uninstall:
	-rm $(DESTDIR)$(bindir)/$(TARGET)
	-rm $(DESTDIR)/$(sysconfdir)/snesdev.cfg

.PHONY: clean install uninstall
