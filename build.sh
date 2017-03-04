#!/usr/bin/env bash

if ! [[ $(type qmake) ]]; then
	echo 'Cannot find qmake. Do you have Qt installed?'; exit 1
fi
echo 'Checking Qt version...'
QTVERSION=$(qmake -v| tail -1| sed 's/Using Qt version //'| sed -r 's/ .*//')
echo 'You are using Qt' $QTVERSION
if [[ $(echo $QTVERSION| sed -r 's/\..*//') -lt 5 ]]; then # major version number
	echo 'Your Qt version is too old, you need at least Qt 5.7'; exit 1
elif [[ $(echo $QTVERSION| grep -Eo '\..*'| grep -Eo '[[:digit:]].*'| sed -r 's/\..*//') -lt 7 ]]; then # minor version number
	echo 'Your Qt version is too old, you need at least Qt 5.7'; exit 2
fi
if [[ -d build ]]; then
	echo 'Found build directory' $(pwd)'/build'
	cd build || exit 1
else
	echo $(pwd)'/build does not exist yet, creating build directory...'
	mkdir build || exit 2
	cd build || exit 3
fi
echo 'Generating Makefile...'
qmake .. || exit 4
echo 'Invoking make...'
make || exit 5
echo -e '\e[0;32mDone! Your executable can be found in' $(pwd)
