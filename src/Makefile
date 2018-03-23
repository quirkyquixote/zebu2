
include ../config.mk

objs += .obj/zebu2.o

deps = $(srcs:.o=.d)

inst += $(DESTDIR)$(libdir)/libzebu.so.$(suffix)
inst += $(DESTDIR)$(libdir)/libzebu.so.$(VERSION)
inst += $(DESTDIR)$(prefix)/include/zebu2.h

.PHONY: all
all: libzebu.so libzebu.a

libzebu.so: $(objs)
libzebu.a: $(objs)

.PHONY: clean
clean:
	$(RM) libzebu.so
	$(RM) libzebu.a
	$(RM) -r .obj

.PHONY: install
install: $(inst)

.PHONY: uninstall
uninstall:
	$(RM) $(inst)

-include $(deps)
