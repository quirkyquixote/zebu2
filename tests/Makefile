
include ../config.mk

ALL_CFLAGS += -I$(root_dir)src
ALL_LDFLAGS += -lgc

tests += zz_atom
tests += zz_is_atom
tests += zz_to_atom
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

zz_atom: .obj/zz_atom.o $(root_dir)src/libzebu.a
zz_is_atom: .obj/zz_is_atom.o $(root_dir)src/libzebu.a
zz_to_atom: .obj/zz_to_atom.o $(root_dir)src/libzebu.a
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
