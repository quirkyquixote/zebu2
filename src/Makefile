
include ../config.mk

ALL_LDFLAGS += -lreadline

objs += .obj/main.o
objs += .obj/zebu.o

deps = $(srcs:.o=.d)

.PHONY: all
all: test

test: $(objs)
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

.PHONY: clean
clean:
	$(RM) test
	$(RM) -r .obj

-include $(deps)
