
include ../../config.mk

ALL_CXXFLAGS += -I$(root_dir)src
ALL_LDFLAGS += -lgc
ALL_LDFLAGS += -lstdc++

tests += int
tests += double
tests += string
tests += pair
tests += list

deps = $(addprefix .obj/,$(addsuffix .d,tests))

.PHONY: all
all: $(addsuffix .run,$(tests))

%.run: % FORCE
	$(QUIET_TEST)./$<

.INTERMEDIATE: $(tests)
int: .obj/int.o
double: .obj/double.o
string: .obj/string.o
pair: .obj/pair.o
list: .obj/list.o

.PHONY: clean
clean:
	$(RM) -r .obj

FORCE:

-include $(deps)
