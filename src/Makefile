.PHONY: test-sim sim serve clean-venv

DEV_BIN       = venv/bin
PYTHON_BIN    ?= python3

test-sim: venv
	@$(DEV_BIN)/nosetests py

sim:
	@$(DEV_BIN)/$(PYTHON_BIN) sim.py

serve: 
	@$(DEV_BIN)/$(PYTHON_BIN) sim.py --server

venv:
	@virtualenv --python=$(PYTHON_BIN) venv
	@$(DEV_BIN)/pip install -r requirements.txt

clean-venv:
	@rm -rf venv
