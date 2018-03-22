
include ../config.mk

objs += .obj/zebu.o

deps = $(srcs:.o=.d)

.PHONY: all
all: libzebu.so libzebu.a

libzebu.so: $(objs)
libzebu.a: $(objs)


.PHONY: clean
clean:
	$(RM) libzebu.so
	$(RM) libzebu.a
	$(RM) -r .obj

-include $(deps)
