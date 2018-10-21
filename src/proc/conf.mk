CPU        = -mcpu=cortex-m4
FPU        = -mfpu=fpv4-sp-d16 -mfloat-abi=hard
MCU        = $(CPU) -mthumb $(FPU)
CFLAGS    += $(MCU) -Os -fdata-sections -ffunction-sections -fno-builtin
LFLAGS    += --specs=nosys.specs -Tproc/link.ld
# -lc -lm -lnosys

SRCS += proc/vectors.c
SRCS += proc/system_stm32f4xx.c
