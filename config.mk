
# Where this file resides

root_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# When version changes, increment this number

VERSION := 1.0.0

# Determine where we are going to install things

prefix := /usr/local
suffix := $(shell echo $(VERSION) | grep -o '^[0-9]*')
bindir := $(prefix)/bin
libdir := $(prefix)/lib
datadir := $(prefix)/share/zebu2
docdir := $(prefix)/share/doc/zebu2
htmldir := $(docdir)
pdfdir := $(docdir)
localstatedir := $(prefix)/var/zebu

# There are so many tools needed...

CC ?= cc
AR ?= ar
YACC ?= yacc
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

ALL_CFLAGS += -std=gnu11
ALL_CFLAGS += -MD
ALL_CFLAGS += -fPIC

ALL_CXXFLAGS += -std=c++14
ALL_CXXFLAGS += -MD
ALL_CXXFLAGS += -fPIC

# To go down a level $(call descend,directory[,target[,flags]])

descend = make -C $(1) $(2) $(3)

# If the -s option was passed, use these to give a hint of what we are doing

ifneq ($(findstring s,$(filter-out --%,$(MAKEFLAGS))),)
    QUIET_CC = @echo CC $@;
    QUIET_LINK = @echo LINK $@;
    QUIET_AR = @echo AR $@;
    QUIET_YACC = @echo YACC $@;
    QUIET_GEN = @echo GEN $@;
    QUIET_UNZIP = @echo UNZIP $@;
    QUIET_INSTALL = @echo INSTALL $@;
    QUIET_TEST = @echo TEST $@;
    descend = @echo ENTER `pwd`/$(1); make -C $(1) $(2) $(3); echo EXIT `pwd`/$(1)
endif

# Some generic targets that are the same for all Makefiles
%.c: %.y
	$(QUIET_YACC)$(YACC) $< -o $@

.obj/%.o: %.c | .obj
	$(QUIET_CC)$(CC) $(ALL_CFLAGS) -c -o $@ $<

.obj/%.o: %.cc | .obj
	$(QUIET_CXX)$(CXX) $(ALL_CXXFLAGS) -c -o $@ $<

.obj:
	mkdir $@

%: .obj/%.o
	$(QUIET_CC)$(CC) $(ALL_LDFLAGS) $^ -o $@

%: %.c
%: %.cc

%.a:
	$(QUIET_AR)$(AR) rc $@ $^

%.so:
	$(QUIET_LINK)$(CC) -shared -Wl,-soname,$@.$(VERSION) -o $@ $^

$(DESTDIR)$(bindir)/%: %
	@$(INSTALL) -d $(@D)
	$(QUIET_INSTALL)$(INSTALL_PROGRAM) $< $@.$(VERSION)
	$(QUIET_GEN)cd $(@D); ln -f -s $(@F).$(VERSION) $(@F)

$(DESTDIR)$(libdir)/%: %
	@$(INSTALL) -d $(@D)
	$(QUIET_INSTALL)$(INSTALL_DATA) $< $@.$(VERSION)
	$(QUIET_GEN)cd $(@D); ln -f -s $(@F).$(VERSION) $(@F)

$(DESTDIR)$(prefix)/include/%: %
	@$(INSTALL) -d $(@D)
	$(QUIET_INSTALL)$(INSTALL_DATA) $< $@

$(DESTDIR)$(datadir)/% $(DESTDIR)$(docdir)/% $(DESTDIR)$(htmldir)/% $(DESTDIR)$(pdfdir)/%: %
	@$(INSTALL) -d $(@D)
	$(QUIET_INSTALL)$(INSTALL_DATA) $< $@

%: %.gz
	@gunzip -k $<
