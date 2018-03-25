
include ../config.mk

ALL_CFLAGS += -I$(root_dir)src
ALL_LDFLAGS += -lgc

tests += zz_int
tests += zz_is_int
tests += zz_to_int
tests += zz_ptr
tests += zz_is_ptr
tests += zz_to_ptr
tests += zz_str
tests += zz_is_str
tests += zz_to_str
tests += zz_pair
tests += zz_is_pair
tests += zz_to_pair
tests += zz_head
tests += zz_tail
tests += zz_foreach

.PHONY: all
all: $(addsuffix .log,$(tests))

%.log: %
	$(QUIET_GEN)./$<

zz_int: .obj/zz_int.o $(root_dir)src/libzebu.a
zz_is_int: .obj/zz_is_int.o $(root_dir)src/libzebu.a
zz_to_int: .obj/zz_to_int.o $(root_dir)src/libzebu.a
zz_ptr: .obj/zz_ptr.o $(root_dir)src/libzebu.a
zz_is_ptr: .obj/zz_is_ptr.o $(root_dir)src/libzebu.a
zz_to_ptr: .obj/zz_to_ptr.o $(root_dir)src/libzebu.a
zz_str: .obj/zz_str.o $(root_dir)src/libzebu.a
zz_is_str: .obj/zz_is_str.o $(root_dir)src/libzebu.a
zz_to_str: .obj/zz_to_str.o $(root_dir)src/libzebu.a
zz_pair: .obj/zz_pair.o $(root_dir)src/libzebu.a
zz_is_pair: .obj/zz_is_pair.o $(root_dir)src/libzebu.a
zz_to_pair: .obj/zz_to_pair.o $(root_dir)src/libzebu.a
zz_head: .obj/zz_head.o $(root_dir)src/libzebu.a
zz_tail: .obj/zz_tail.o $(root_dir)src/libzebu.a
zz_foreach: .obj/zz_foreach.o $(root_dir)src/libzebu.a

../src/libzebu.a:
	$(call descend,../src,libzebu.a)

.PHONY: clean
clean:
	$(RM) $(tests)
	$(RM) -r .obj
