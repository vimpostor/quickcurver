#!/usr/bin/env bash

REQUIREDQTMAJORVERSION=5
REQUIREDQTMINORVERSION=7

if [[ $1 = '-f' ]]; then
	echo 'Skipping Qt version check...'
else
	if ! [[ $(type qmake) ]]; then
		echo 'Cannot find qmake. Do you have Qt installed?'; exit 1
	fi
	echo 'Checking Qt version...'
	QTVERSION=$(qmake -v| tail -1| sed 's/Using Qt version //'| sed -r 's/ .*//')
	QTMAJORVERSION=$(echo $QTVERSION| sed -r 's/\..*//')
	QTMINORVERSION=$(echo $QTVERSION| grep -Eo '\..*'| grep -Eo '[[:digit:]].*'| sed -r 's/\..*//')
	echo 'You are using Qt' $QTVERSION
	if ( [ $QTMAJORVERSION -lt $REQUIREDQTMAJORVERSION ] || ( [ $QTMAJORVERSION -eq $REQUIREDQTMAJORVERSION ]  && [ $QTMINORVERSION -lt $REQUIREDQTMINORVERSION ] ) ); then
		echo 'Your Qt version is too old, you need at least Qt' $REQUIREDQTMAJORVERSION.$REQUIREDQTMINORVERSION; exit 1
	fi
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
