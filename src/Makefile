.PHONY: clean

DEV_BIN       = venv/bin
PYTHON_BIN    ?= python3

test-sim: venv
	@$(DEV_BIN)/nosetests --nocapture --nologcapture sim

venv:
	@rm -rf venv
	@virtualenv --python=$(PYTHON_BIN) venv
	@$(DEV_BIN)/pip install -r requirements.txt
