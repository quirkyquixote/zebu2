
include config.mk

.PHONY: all
all:
	$(call descend,src,all)

.PHONY: clean
clean:
	$(call descend,src,clean)

.PHONY: doc
doc:
	$(call descend,doc,all)

.PHONY: clean-doc
clean-doc:
	$(call descend,doc,clean)
