# Makefile for Sphinx documentation
#
include ../config.mk

# You can set these variables from the command line.
SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
PAPER         =
BUILDDIR      = _build

# Internal variables.
PAPEROPT_a4     = -D latex_paper_size=a4
PAPEROPT_letter = -D latex_paper_size=letter
ALLSPHINXOPTS   = -d $(BUILDDIR)/doctrees $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) .
# the i18n builder cannot share the environment and doctrees with the others
I18NSPHINXOPTS  = $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) .

.PHONY: all
all: html pdf

.PHONY: install
install: install-html install-pdf

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/*

.PHONY: html
html:
	$(SPHINXBUILD) -b html $(ALLSPHINXOPTS) $(BUILDDIR)/html
	@echo
	@echo "Build finished. The HTML pages are in $(BUILDDIR)/html."

.PHONY: clean-html
clean-html:
	rm -rf $(BUILDDIR)/html

.PHONY: install-html
install-html:
	$(INSTALL) -d $(DESTDIR)$(htmldir)/apidoc
	$(INSTALL) -d $(DESTDIR)$(htmldir)/apidoc/_static
	$(INSTALL_DATA) $(BUILDDIR)/html/*.html $(DESTDIR)$(htmldir)/apidoc
	$(INSTALL_DATA) $(BUILDDIR)/html/*.js $(DESTDIR)$(htmldir)/apidoc
	$(INSTALL_DATA) $(BUILDDIR)/html/_static/* $(DESTDIR)$(htmldir)/apidoc/_static

.PHONY: pdf
pdf:
	$(SPHINXBUILD) -b latex $(ALLSPHINXOPTS) $(BUILDDIR)/latex
	@echo "Running LaTeX files through pdflatex..."
	$(MAKE) -C $(BUILDDIR)/latex all-pdf
	@echo "pdflatex finished; the PDF files are in $(BUILDDIR)/latex."

.PHONY: clean-pdf
clean-pdf:
	rm -rf $(BUILDDIR)/pdf

.PHONY: install-pdf
install-pdf:
	$(INSTALL) -d $(DESTDIR)$(pdfdir)
	$(INSTALL_DATA) $(BUILDDIR)/latex/TNT.pdf $(DESTDIR)$(pdfdir)/apidoc.pdf

