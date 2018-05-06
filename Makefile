
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
examples:
	$(call descend,examples,all)

.PHONY: clean-examples
clean-examples:
	$(call descend,examples,clean)

.PHONY: doc
doc:
	$(call descend,doc,all)

.PHONY: clean-doc
clean-doc:
	$(call descend,doc,clean)

.PHONY: html
html:
	$(call descend,doc,html)

.PHONY: clean-html
clean-html:
	$(call descend,doc,clean-html)

.PHONY: pdf
pdf:
	$(call descend,doc,pdf)

.PHONY: clean-pdf
clean-pdf:
	$(call descend,doc,clean-pdf)
