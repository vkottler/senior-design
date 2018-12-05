.PHONY: clean start view

BROWSER ?= firefox

start: node_modules
	node server

view:
	@$(BROWSER) http://localhost:3000 &

node_modules:
	npm install

clean:
	rm -rf node_modules
