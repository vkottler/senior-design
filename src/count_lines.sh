#!/bin/bash

# get raw line count for source and header files
C_LINES=(`find -iname '*.c' | xargs wc -l | tail -n 1`)
H_LINES=(`find -iname '*.h' | xargs wc -l | tail -n 1`)

UNWANTED_FILES1="proc/stm32f303xe.h proc/stm32f3xx.h proc/system_stm32f3xx.h"
UNWANTED_FILES2="include/spi.h include/i2c.h include/accel.h include/adc.h"
UNWANTED_FILES="$UNWANTED_FILES1 $UNWANTED_FILES2"
UNWANTED_LINES=(`wc -l $UNWANTED_FILES | tail -n 1`)
CMSIS_UNWANTED=(`cd include/cmsis && ls | xargs wc -l | tail -n 1`)
ST_UNWANTED=(`cd include/st && ls | xargs wc -l | tail -n 1`)

# accumulated unwanted lines
UNWANTED_LINES=`expr $UNWANTED_LINES + $CMSIS_UNWANTED + $ST_UNWANTED`

H_LINES=`expr $H_LINES - $UNWANTED_LINES`

UNWANTED_FILES1="driver/adc.c driver/i2c.c driver/spi.c proc/system_stm32f3xx.c"
UNWANTED_LINES=(`wc -l $UNWANTED_FILES1 | tail -n 1`)
ST_UNWANTED_C=(`cd driver/st && ls | xargs wc -l | tail -n 1`)

UNWANTED_LINES=`expr $UNWANTED_LINES + $ST_UNWANTED_C`

C_LINES=`expr $C_LINES - $UNWANTED_LINES`

PYTHON_LINES=(`find -iname '*.py' | xargs wc -l | tail -n 1`)

MAKEFILE_LINES=(`find -iname '*.mk' | xargs wc -l | tail -n 1`)
TOP_LEVEL_MK_LINES=(`wc -l Makefile | tail -n 1`)
TOP_LEVEL_MK_LINES2=(`wc -l lib/telemetry/Makefile | tail -n 1`)
MAKEFILE_LINES=`expr $MAKEFILE_LINES + $TOP_LEVEL_MK_LINES + $TOP_LEVEL_MK_LINES2`

HTML_LINES=(`find -iname '*.html' | xargs wc -l | tail -n 1`)
CSS_LINES=(`find -iname '*.css' | xargs wc -l | tail -n 1`)
JS_LINES=(`find -iname '*.js' | xargs wc -l | tail -n 1`)

echo "Originally Authored"
echo "Source Code Breakdown"
echo "======================="
echo "C Source   : $C_LINES lines"
echo "C Header   : $H_LINES lines"
echo "Python     : $PYTHON_LINES lines"
echo "GNU make   : $MAKEFILE_LINES  lines"
echo "HTML       : $HTML_LINES  lines"
echo "CSS        : $CSS_LINES  lines"
echo "JavaScript : $JS_LINES  lines"
echo "======================="

TOTAL_LINES=`expr $C_LINES + $H_LINES + $PYTHON_LINES + $MAKEFILE_LINES`
TOTAL_LINES=`expr $TOTAL_LINES + $HTML_LINES + $CSS_LINES + $JS_LINES`

echo "Total      : $TOTAL_LINES lines"
