.PHONY: clean main %-flash %-dump
.SECONDARY:
.DEFAULT_GOAL = main

# toolchain options
include mk/toolchain.mk

# source file discovery
include proc/conf.mk
include app/conf.mk
include lib/conf.mk
include driver/conf.mk

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
	@$(DEV_BIN)/nosetests --nocapture sim

venv:
	@rm -rf venv
	@virtualenv --python=$(PYTHON_BIN) venv
	@$(DEV_BIN)/pip install -r requirements.txt
