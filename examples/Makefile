
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

.INTERMEDIATE: $(addsuffix .c,$(bins))
calc: .obj/calc.o .obj/test.o ../src/libzebu2.a
math: .obj/math.o .obj/test.o ../src/libzebu2.a
lisp: .obj/lisp.o .obj/test.o ../src/libzebu2.a
shon: .obj/shon.o .obj/test.o ../src/libzebu2.a

../src/libzebu2.a:
	$(call descend,../src,libzebu2.a)

.PHONY: clean
clean:
	$(RM) test
	$(RM) -r .obj

-include $(deps)
