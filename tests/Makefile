
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
tests += zz_cmp
tests += zz_copy
tests += zz_index
tests += zz_list
tests += zz_append
tests += zz_merge

deps = $(addprefix .obj/,$(addsuffix .d,tests))

.PHONY: all
all: $(addsuffix .log,$(tests))

%.log: % FORCE
	$(QUIET_GEN)./$< &> $@

FORCE:

zz_int: .obj/zz_int.o $(root_dir)src/libzebu2.a
zz_ptr: .obj/zz_ptr.o $(root_dir)src/libzebu2.a
zz_str: .obj/zz_str.o $(root_dir)src/libzebu2.a
zz_pair: .obj/zz_pair.o $(root_dir)src/libzebu2.a
zz_cast: .obj/zz_cast.o $(root_dir)src/libzebu2.a
zz_head: .obj/zz_head.o $(root_dir)src/libzebu2.a
zz_tail: .obj/zz_tail.o $(root_dir)src/libzebu2.a
zz_foreach: .obj/zz_foreach.o $(root_dir)src/libzebu2.a
zz_cmp: .obj/zz_cmp.o $(root_dir)src/libzebu2.a
zz_copy: .obj/zz_copy.o $(root_dir)src/libzebu2.a
zz_index: .obj/zz_index.o $(root_dir)src/libzebu2.a
zz_list: .obj/zz_list.o $(root_dir)src/libzebu2.a
zz_append: .obj/zz_append.o $(root_dir)src/libzebu2.a
zz_merge: .obj/zz_merge.o $(root_dir)src/libzebu2.a

../src/libzebu2.a:
	(call descend,../src,libzebu2.a)

.PHONY: clean
clean:
	$(RM) $(tests)
	$(RM) -r .obj

-include $(deps)
