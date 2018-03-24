
include ../config.mk

ALL_CFLAGS += -I../src
ALL_LDFLAGS += -lreadline -lgc

objs += .obj/calc.tab.o
objs += .obj/math.tab.o
objs += .obj/lisp.tab.o
objs += .obj/shon.tab.o

deps = $(objs=.o:.d)

.PHONY: all
all: calc math lisp shon

calc: .obj/calc.tab.o ../src/libzebu.a
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

calc.tab.c: calc.y
	bison $< -o $@

math: .obj/math.tab.o ../src/libzebu.a
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

math.tab.c: math.y
	bison $< -o $@

lisp: .obj/lisp.tab.o ../src/libzebu.a
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

lisp.tab.c: lisp.y
	bison $< -o $@

shon: .obj/shon.tab.o ../src/libzebu.a
	$(QUIET_LINK)$(CC) $(ALL_CFLAGS) $(ALL_LDFLAGS) -o $@ $^

shon.tab.c: shon.y
	bison $< -o $@

../src/libzebu.a:
	$(call descend,../src,libzebu.a)

.PHONY: clean
clean:
	$(RM) test
	$(RM) -r .obj
	$(RM) *.tab.c

-include $(deps)
