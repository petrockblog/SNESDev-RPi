# Generic Makefile for compiling a simple executable.
LIBBCM2835_DIR=/usr/local
LIBBCM2835_INCDIR=$(LIBBCM2835_DIR)/include
LIBBCM2835_LIBDIR=$(LIBBCM2835_DIR)/lib

LIBCONFUSE_DIR=/usr/local
LIBCONFUSE_INCDIR=$(LIBCONFUSE_DIR)/include
LIBCONFUSE_LIBDIR=$(LIBCONFUSE_DIR)/lib

CC=gcc
SRCDIR=src
BUILDDIR=build
CFLAGS=-c -Wall -O3
LIBS=-lbcm2835 -L$(LIBBCM2835_LIBDIR) -I$(LIBBCM2835_INCDIR) -lrt -lconfuse -L$(LIBCONFUSE_LIBDIR) -I$(LIBCONFUSE_INCDIR)
TARGET := SNESDev
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPS := $(OBJECTS:.o=.deps)

$(TARGET): $(OBJECTS)
	@echo " Linking..."; $(CC) $^ $(LIBS) -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -o $@ $<

clean:
	@echo " Cleaning..."; $(RM) -r $(BUILDDIR) $(TARGET)

-include $(DEPS)

.PHONY: clean
