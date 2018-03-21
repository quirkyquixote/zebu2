
include config.mk

.PHONY: all
all:
	$(call descend,src,all)

.PHONY: clean
clean:
	$(call descend,src,clean)
