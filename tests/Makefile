
include ../config.mk

ALL_CFLAGS += -I$(root_dir)src
ALL_LDFLAGS += -lgc

tests += int
tests += ptr
tests += str
tests += pair
tests += cast
tests += head
tests += tail
tests += foreach
tests += cmp
tests += copy
tests += index
tests += unpack
tests += list
tests += append
tests += prepend
tests += merge
tests += find
tests += find_if
tests += remove
tests += remove_if
tests += sort

deps = $(addprefix .obj/,$(addsuffix .d,tests))

.PHONY: all
all: $(addsuffix .run,$(tests))

%.run: % FORCE
	$(QUIET_TEST)./$<

.INTERMEDIATE: $(tests)
int: .obj/int.o $(root_dir)src/libzebu2.a
ptr: .obj/ptr.o $(root_dir)src/libzebu2.a
str: .obj/str.o $(root_dir)src/libzebu2.a
pair: .obj/pair.o $(root_dir)src/libzebu2.a
cast: .obj/cast.o $(root_dir)src/libzebu2.a
head: .obj/head.o $(root_dir)src/libzebu2.a
tail: .obj/tail.o $(root_dir)src/libzebu2.a
foreach: .obj/foreach.o $(root_dir)src/libzebu2.a
cmp: .obj/cmp.o $(root_dir)src/libzebu2.a
copy: .obj/copy.o $(root_dir)src/libzebu2.a
index: .obj/index.o $(root_dir)src/libzebu2.a
unpack: .obj/unpack.o $(root_dir)src/libzebu2.a
list: .obj/list.o $(root_dir)src/libzebu2.a
append: .obj/append.o $(root_dir)src/libzebu2.a
prepend: .obj/prepend.o $(root_dir)src/libzebu2.a
merge: .obj/merge.o $(root_dir)src/libzebu2.a
find: .obj/find.o $(root_dir)src/libzebu2.a
find_if: .obj/find_if.o $(root_dir)src/libzebu2.a
remove: .obj/remove.o $(root_dir)src/libzebu2.a
remove_if: .obj/remove_if.o $(root_dir)src/libzebu2.a
sort: .obj/sort.o $(root_dir)src/libzebu2.a

$(root_dir)src/libzebu2.a:
	(call descend,$(root_dir)src,libzebu2.a)

.PHONY: clean
clean:
	$(RM) -r .obj

FORCE:

-include $(deps)
