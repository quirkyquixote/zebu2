
include ../config.mk

ALL_CFLAGS += -I$(root_dir)src
ALL_LDFLAGS += -lgc

tests += zz_int
tests += zz_ptr
tests += zz_str
tests += zz_pair
tests += zz_cast
tests += zz_head
tests += zz_tail
tests += zz_foreach

deps = $(addprefix .obj/,$(addsuffix .d,tests))

.PHONY: all
all: $(addsuffix .log,$(tests))

%.log: % FORCE
	$(QUIET_GEN)./$< &> $@

FORCE:

zz_int: .obj/zz_int.o $(root_dir)src/libzebu.a
zz_ptr: .obj/zz_ptr.o $(root_dir)src/libzebu.a
zz_str: .obj/zz_str.o $(root_dir)src/libzebu.a
zz_pair: .obj/zz_pair.o $(root_dir)src/libzebu.a
zz_cast: .obj/zz_cast.o $(root_dir)src/libzebu.a
zz_head: .obj/zz_head.o $(root_dir)src/libzebu.a
zz_tail: .obj/zz_tail.o $(root_dir)src/libzebu.a
zz_foreach: .obj/zz_foreach.o $(root_dir)src/libzebu.a

../src/libzebu.a:
	$(call descend,../src,libzebu.a)

.PHONY: clean
clean:
	$(RM) $(tests)
	$(RM) -r .obj

-include $(deps)
