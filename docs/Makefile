.PHONY: help Makefile view serve

SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
SPHINXPROJ    = SeniorDesignQuadcopter
SOURCEDIR     = src
BUILDDIR      = rel
DEV_BIN       = venv/bin

BROWSER       ?= firefox
PYTHON_BIN    ?= python3

# Windows-specific variable modifications
ifeq ($(OS),Windows_NT)
DEV_BIN       = venv/Scripts
endif

help: venv
	@$(DEV_BIN)/$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile venv
	@$(DEV_BIN)/$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

view:
	@$(BROWSER) http://localhost:8000/$(BUILDDIR)/html/index.html

serve: venv
	@$(PYTHON_BIN) -m http.server

venv:
	@rm -rf venv
	@virtualenv --python=$(PYTHON_BIN) venv
	@$(DEV_BIN)/pip install -r requirements.txt
