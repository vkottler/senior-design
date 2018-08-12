#!/bin/bash
#
# https://www.freecadweb.org/wiki/Download
#

if [ `whoami` == "root" ]; then
	echo "Not necessary to run as root."
	exit 1
fi

IMAGE_NAME="FreeCAD-13528.glibc2.17-x86_64.AppImage"
SHORT_IMAGE="FreeCAD.AppImage"
IMAGE_DEST="$HOME/bin/$SHORT_IMAGE"

if [ ! -f $IMAGE_DEST ]; then
	echo "Installing FreeCAD to '$IMAGE_DEST'."
else
	echo "FreeCAD already installed, skipping."
	exit 0
fi

IMAGE_LOCATION="https://github.com/FreeCAD/FreeCAD/releases/download/0.17/$IMAGE_NAME"

wget $IMAGE_LOCATION

mv ./$IMAGE_NAME ./$SHORT_IMAGE
chmod a+x ./$SHORT_IMAGE

mkdir -p $HOME/bin
mv ./$SHORT_IMAGE $IMAGE_DEST
