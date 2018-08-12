#!/bin/bash
#
# http://kicad-pcb.org/download/ubuntu/
#

if [ `whoami` != "root" ]; then
	echo "Must be run as root."
	exit 1
fi

if which kicad >/dev/null; then
	echo "KiCad found, skipping."
	exit 0
fi

add-apt-repository --yes ppa:js-reynaud/kicad-5
apt update
apt install kicad -y
