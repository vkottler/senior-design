.PHONY: test-sim sim clean-venv

DEV_BIN       = venv/bin
PYTHON_BIN    ?= python3

test-sim: venv
	@$(DEV_BIN)/nosetests py

sim:
	@./sim.py

venv:
	@virtualenv --python=$(PYTHON_BIN) venv
	@$(DEV_BIN)/pip install -r requirements.txt

clean-venv:
	@rm -rf venv
