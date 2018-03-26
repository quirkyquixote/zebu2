
include ../config.mk

ALL_CFLAGS += -I$(root_dir)src
ALL_LDFLAGS += -lreadline -lgc

bins += calc
bins += math
bins += lisp
bins += shon

deps = $(addprefix .obj/,$(addsuffix .d,$(bins)))

.PHONY: all
all: $(bins)

calc: .obj/calc.o .obj/test.o ../src/libzebu.a
math: .obj/math.o .obj/test.o ../src/libzebu.a
lisp: .obj/lisp.o .obj/test.o ../src/libzebu.a
shon: .obj/shon.o .obj/test.o ../src/libzebu.a

../src/libzebu.a:
	$(call descend,../src,libzebu.a)

.PHONY: clean
clean:
	$(RM) test
	$(RM) -r .obj

-include $(deps)
