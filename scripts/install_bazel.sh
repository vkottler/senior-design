#!/bin/bash
#
# https://docs.bazel.build/versions/master/install-ubuntu.html#install-with-installer-ubuntu
#

if [ `whoami` == "root" ]; then
	echo "Not necessary to run as root."
	exit 1
fi

VERSION="0.16.0"
INSTALLER="bazel-${VERSION}-installer-linux-x86_64.sh"
INSTALLER_LOCATION="https://github.com/bazelbuild/bazel/releases/download/$VERSION/$INSTALLER"

BAZEL_DEST="$HOME/bin/bazel"

if [ ! -f $BAZEL_DEST ]; then
	echo "Installing bazel to '$IMAGE_DEST'."
else
	echo "bazel already installed, skipping."
	exit 0
fi

echo "Installing bazel dependencies . . ."
apt-get install -y pkg-config zip g++ zlib1g-dev unzip python

wget $INSTALLER_LOCATION
chmod +x $INSTALLER
./$INSTALLER --user
rm ./$INSTALLER
