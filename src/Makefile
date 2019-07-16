CC=gcc
SRCDIR=.
BUILDDIR=../build
CFLAGS=-Wall -O3
LIBCONFUSE_DIR=/usr/local
LIBCONFUSE_INCDIR=$(LIBCONFUSE_DIR)/include
LIBCONFUSE_LIBDIR=$(LIBCONFUSE_DIR)/lib
LIBS=-lwiringPi -lpthread -lrt -lconfuse -L$(LIBCONFUSE_LIBDIR) -I$(LIBCONFUSE_INCDIR)

TARGET := SNESDev
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPS := $(OBJECTS:.o=.deps)

all $(TARGET):$(OBJECTS)
	@echo " Linking..."; $(CC) $^ $(LIBS) -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -I$(LIBCONFUSE_INCDIR) -MD -MF $(@:.o=.deps) -c -o $@ $<

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
