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
BROWSER       ?= firefox

test-sim: venv
	@$(DEV_BIN)/nosetests py

sim: venv
	@$(DEV_BIN)/$(PYTHON_BIN) sim.py

#@$(DEV_BIN)/$(PYTHON_BIN) sim.py --server
serve: venv
	@$(DEV_BIN)/$(PYTHON_BIN) telemetry.py

serve-web:
	@$(PYTHON_BIN) -m http.server

view-web:
	@$(BROWSER) http://localhost:8000/web/websocket_client.html

venv:
	@virtualenv --python=$(PYTHON_BIN) venv
	@$(DEV_BIN)/pip install -r requirements.txt

clean-venv:
	@rm -rf venv
