
include ../config.mk

objs += .obj/zebu2.o

deps = $(srcs:.o=.d)

inst += $(DESTDIR)$(libdir)/libzebu2.so
inst += $(DESTDIR)$(prefix)/include/zebu2.h

.PHONY: all
all: libzebu2.so libzebu2.a

libzebu2.so: $(objs)
libzebu2.a: $(objs)

.PHONY: clean
clean:
	$(RM) libzebu2.so
	$(RM) libzebu2.a
	$(RM) -r .obj

.PHONY: install
install: $(inst)

.PHONY: uninstall
uninstall:
	$(RM) $(inst)

-include $(deps)
