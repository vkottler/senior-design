.PHONY: help Makefile

SPHINXOPTS    =
SPHINXBUILD   = sphinx-build
SPHINXPROJ    = SeniorDesignQuadcopter
SOURCEDIR     = src
BUILDDIR      = rel

help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
