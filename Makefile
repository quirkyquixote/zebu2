
include config.mk

.PHONY: all
all:
	$(call descend,src,all)

.PHONY: clean
clean:
	$(call descend,src,clean)

.PHONY: install
install:
	$(call descend,src,install)

.PHONY: uninstall
uninstall:
	$(call descend,src,uninstall)

.PHONY: test
test:
	$(call descend,tests,all)

.PHONY: clean-test
clean-test:
	$(call descend,tests,clean)

.PHONY: examples
example:
	$(call descend,examples,all)

.PHONY: clean-examples
clean-example:
	$(call descend,examples,clean)

.PHONY: doc
doc:
	$(call descend,doc,all)

.PHONY: clean-doc
clean-doc:
	$(call descend,doc,clean)
