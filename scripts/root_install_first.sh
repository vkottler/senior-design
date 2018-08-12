#/bin/bash

if [ `whoami` != "root" ]; then
	echo "Must be run as root."
	exit 1
fi

apt update

echo "Installing KiCad . . ."
./install_kicad.sh

echo "Installing toolchain(s) . . ."
./install_toolchains.sh
