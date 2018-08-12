#/bin/bash

if [ `whoami` != "root" ]; then
	echo "Must be run as root."
	exit 1
fi

if which arm-none-eabi-gcc >/dev/null; then
	echo "Found toolchain(s), skipping."
	exit 0
fi

apt-get install -y gcc-arm-none-eabi gdb-arm-none-eabi make screen
