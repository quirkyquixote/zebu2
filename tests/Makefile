
include ../config.mk

ALL_LDFLAGS += -lreadline

objs += .obj/main.o

deps = $(srcs:.o=.d)

.PHONY: all
all: test

test: $(objs) ../src/libzebu.a
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

../src/libzebu.a:
	$(call descend,../src,libzebu.a)

.PHONY: clean
clean:
	$(RM) test
	$(RM) -r .obj

-include $(deps)
