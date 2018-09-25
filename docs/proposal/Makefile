.PHONY: clean all
.DEFAULT_GOAL := all

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

%: %.pdf
	@$(PDFVIEWER) $< &

all: presentation

clean:
	@rm -f *.log *.aux *.dvi *.out *.pdf
