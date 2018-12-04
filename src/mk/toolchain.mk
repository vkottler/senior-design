OBJ_DIR   = bin
TOOLCHAIN = arm-none-eabi-
INCLUDES += -I include -I proc -I include/cmsis -I include/st
INCLUDES += -I lib/telemetry/include
CFLAGS   += $(INCLUDES) -Wall -Werror -pedantic -std=c99
CFLAGS   += -D_POSIX_C_SOURCE=200809L -Wno-strict-aliasing
LFLAGS   += -Wl,--gc-sections
