#!/bin/bash

# check usage
if [ $# -ne 1 ]; then
	echo "usage: (flight|ground)/(project_name)" && echo ""
	echo "options" && echo "" 
	echo "flight/" && ls flight && echo ""
	echo "ground/" && ls ground && echo ""
	exit 1
fi

# check if the input can be parsed
if ! echo $1 | grep '/' >/dev/null; then
	echo "Malformed input (no '/')."
	exit 1
fi

ROOT=$(echo $1 | sed -r 's/\/(.*)//')
PROJECT=$(echo $1 | sed -r 's/^(flight|ground)\///')
PROJECT_FILE="$ROOT/$PROJECT/$PROJECT.pro"

if [ ! -f $PROJECT_FILE ]; then
	echo "Can't find project '$PROJECT_FILE'."
	exit 1
fi

kicad $PROJECT_FILE &
