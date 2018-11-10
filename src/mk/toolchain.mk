OBJ_DIR   = bin
TOOLCHAIN = arm-none-eabi-
INCLUDES += -I include -I proc -I include/cmsis -I include/st
CFLAGS   += $(INCLUDES) -Wall -Werror -pedantic -std=c99
LFLAGS   += -Wl,--gc-sections
