
include ../config.mk

ALL_CFLAGS += -I$(root_dir)src
ALL_LDFLAGS += -lreadline -lgc

.PHONY: all
all: calc math lisp shon

calc: .obj/calc.o ../src/libzebu.a
math: .obj/math.o ../src/libzebu.a
lisp: .obj/lisp.o ../src/libzebu.a
shon: .obj/shon.o ../src/libzebu.a

../src/libzebu.a:
	$(call descend,../src,libzebu.a)

.PHONY: clean
clean:
	$(RM) test
	$(RM) -r .obj
