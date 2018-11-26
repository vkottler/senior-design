.PHONY: clean main %-flash %-dump sim test-sim clean-venv serve
.SECONDARY:
.DEFAULT_GOAL = main

# toolchain options
include mk/toolchain.mk

# source file discovery
include proc/conf.mk
include app/conf.mk
include lib/conf.mk
include driver/conf.mk
include common/conf.mk
include control/conf.mk

OBJECTS = $(SRCS:.c=.o)

# up-to-date dependency trees
DEPS += $(SRCS:.c=.d)
-include $(DEPS)

# useful target declarations
include mk/rules.mk

###############################################################################

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
