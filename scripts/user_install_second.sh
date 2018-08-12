#/bin/bash

if [ `whoami` == "root" ]; then
	echo "Not necessary to run as root."
	exit 1
fi

./get_freecad.sh
./install_bazel.sh
