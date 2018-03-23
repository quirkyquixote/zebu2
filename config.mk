
# Where this file resides

root_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# When version changes, increment this number

VERSION := 2.0.0

# Determine where we are going to install things

prefix := /usr/local
suffix := $(shell echo $(VERSION) | grep -o '^[0-9]*')
bindir := $(prefix)/bin
libdir := $(prefix)/lib
datadir := $(prefix)/share/zebu$(suffix)
docdir := $(prefix)/share/doc/zebu$(suffix)
htmldir := $(docdir)
pdfdir := $(docdir)
localstatedir := $(prefix)/var/zebu

# There are so many tools needed...

CC ?= cc
CXX ?= c++
AR ?= ar
INSTALL ?= install
INSTALL_PROGRAM := $(INSTALL)
INSTALL_DATA := $(INSTALL) -m 644

# These flags may be overriden by the user

CPPFLAGS ?=
CFLAGS ?= -g -Werror -Wfatal-errors
CXXFLAGS ?= -g -Werror -Wfatal-errors
LDFLAGS ?=

# These flags are necessary

ALL_CFLAGS := $(CPPFLAGS) $(CFLAGS)
ALL_CXXFLAGS := $(CPPFLAGS) $(CXXFLAGS)
ALL_LDFLAGS := $(LDFLAGS)

ALL_CFLAGS += -std=gnu99
ALL_CFLAGS += -DVERSION=\"$(VERSION)\"
ALL_CFLAGS += -DLIBDIR=\"$(libdir)\"
ALL_CFLAGS += -DDATADIR=\"$(datadir)\"
ALL_CFLAGS += -DLOCALSTATEDIR=\"$(localstatedir)\"
ALL_CFLAGS += -MD
ALL_CFLAGS += -fPIC
ALL_CFLAGS += -I$(root_dir)
ALL_CFLAGS += -I$(root_dir)/src


ALL_CXXFLAGS += -std=c++17
ALL_CXXFLAGS += -DVERSION=\"$(VERSION)\"
ALL_CXXFLAGS += -DLIBDIR=\"$(libdir)\"
ALL_CXXFLAGS += -DDATADIR=\"$(datadir)\"
ALL_CXXFLAGS += -DLOCALSTATEDIR=\"$(localstatedir)\"
ALL_CXXFLAGS += -MD
ALL_CXXFLAGS += -fPIC
ALL_CXXFLAGS += -I$(root_dir)
ALL_CXXFLAGS += -I$(root_dir)/src


ALL_LDFLAGS += -pthread
ALL_LDFLAGS += -lstdc++

# To go down a level $(call descend,directory[,target[,flags]])

descend = make -C $(1) $(2) $(3)

# If the -s option was passed, use these to give a hint of what we are doing

ifneq ($(findstring s,$(filter-out --%,$(MAKEFLAGS))),)
    QUIET_CC = @echo CC $@;
    QUIET_CXX = @echo CXX $@;
    QUIET_LINK = @echo LINK $@;
    QUIET_AR = @echo AR $@;
    QUIET_GEN = @echo GEN $@;
    QUIET_UNZIP = @echo UNZIP $@;
    QUIET_INSTALL = @echo INSTALL $@;
    QUIET_TEST = @echo TEST $@;
    descend = @echo ENTER `pwd`/$(1); make -C $(1) $(2) $(3); echo EXIT `pwd`/$(1)
endif

# Some generic targets that are the same for all Makefiles
.obj/%.o: %.c | .obj
	$(QUIET_CC)$(CC) $(ALL_CFLAGS) -c -o $@ $<

.obj/%.o: %.cc | .obj
	$(QUIET_CXX)$(CXX) $(ALL_CXXFLAGS) -c -o $@ $<

.obj:
	mkdir $@

%.a:
	$(QUIET_AR)$(AR) rc $@ $^

%.so:
	$(QUIET_LINK)$(CC) -shared -Wl,-soname,$@.$(version) -o $@ $^

$(DESTDIR)$(bindir)/%.$(VERSION): %
	@$(INSTALL) -d $(@D)
	$(QUIET_INSTALL)$(INSTALL_PROGRAM) $< $@$(suffix)

$(DESTDIR)$(libdir)/%.$(VERSION): %
	@$(INSTALL) -d $(@D)
	$(QUIET_INSTALL)$(INSTALL_DATA) $< $@

%.$(suffix): %.$(VERSION)
	cd $(@D); ln -f -s $(<F) $(@F)

$(DESTDIR)$(prefix)/include/%: %
	@$(INSTALL) -d $(@D)
	$(QUIET_INSTALL)$(INSTALL_DATA) $< $@

$(DESTDIR)$(datadir)/% $(DESTDIR)$(docdir)/% $(DESTDIR)$(htmldir)/% $(DESTDIR)$(pdfdir)/%: %
	@$(INSTALL) -d $(@D)
	$(QUIET_INSTALL)$(INSTALL_DATA) $< $@

%: %.gz
	@gunzip -k $<
