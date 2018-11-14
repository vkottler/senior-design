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

OBJECTS = $(SRCS:.c=.o)

# up-to-date dependency trees
DEPS += $(SRCS:.c=.d)
-include $(DEPS)

# useful target declarations
include mk/rules.mk

###############################################################################

DEV_BIN       = venv/bin
PYTHON_BIN    ?= python3

telem_deps:
	gcc telemetry/app/main.c telemetry/src/*.c -I telemetry/include/ -o telemetry/main.elf
	gcc telemetry/app/send_main.c telemetry/src/*.c -I telemetry/include/ -o telemetry/send_main.elf
	gcc telemetry/app/client.c telemetry/src/*.c -I telemetry/include/ -o telemetry/client.elf

telem: telem_deps
	./telemetry/main.elf

send_telem: telem_deps
	./telemetry/send_main.elf

telem_demo: telem_deps
	./telemetry/client.elf

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
