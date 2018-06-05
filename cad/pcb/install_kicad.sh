#!/bin/bash

if [ "$(whoami)" != "root" ]; then
	echo "Script must be run as root"
	exit 1
fi

add-apt-repository --yes ppa:js-reynaud/kicad-4
apt update
apt install kicad -y
