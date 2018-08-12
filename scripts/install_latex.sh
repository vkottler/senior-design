#/bin/bash

if [ `whoami` != "root" ]; then
	echo "Must be run as root."
	exit 1
fi

if which latex >/dev/null; then
	echo "LaTeX found, skipping."
	exit 0
fi

apt-get install -y texlive-full
