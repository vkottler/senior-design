.PHONY: clean proposal presentation
.DEFAULT_GOAL := proposal

# platform-specific programs
PDFVIEWER := evince
DVIVIEWER := xdvi
PNGVIEWER := eog

%.dvi: %.tex
	latex $<

%.png: %.dvi
	dvipng $< -o $@

%.pdf: %.tex
	pdflatex $<

proposal: proposal.pdf
	@$(PDFVIEWER) $< &

presentation: presentation.pdf
	@$(PDFVIEWER) $< &

#dvi: $(PROJECT).dvi
#	@$(DVIVIEWER) $< &

#png: $(PROJECT).png
#	@$(PNGVIEWER) $< &

clean:
	@rm -f *.log *.aux *.dvi *.out *.pdf
