.PHONY: help Makefile

SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
SPHINXPROJ    = SeniorDesignQuadcopter
SOURCEDIR     = src
BUILDDIR      = rel
DEV_BIN       = venv/bin

help:
	@$(DEV_BIN)/$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile venv
	@$(DEV_BIN)/$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

venv:
	@rm -rf venv
	@virtualenv --python=python3 venv
	@$(DEV_BIN)/pip install -r dev_requirements.txt
