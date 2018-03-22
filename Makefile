
include config.mk

.PHONY: all
all:
	$(call descend,src,all)

.PHONY: clean
clean:
	$(call descend,src,clean)

.PHONY: test
test:
	$(call descend,tests,all)

.PHONY: clean-test
clean-test:
	$(call descend,tests,clean)

.PHONY: doc
doc:
	$(call descend,doc,all)

.PHONY: clean-doc
clean-doc:
	$(call descend,doc,clean)
