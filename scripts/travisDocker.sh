#!/usr/bin/env bash

docker run -t --rm=true base/archlinux /bin/sh -c "pacman -Syu --noconfirm make gcc git qt5-base qt5-declarative qt5-svg qt5-quickcontrols2 qt5-graphicaleffects && git clone --recursive https://github.com/magnus-gross/quickcurver.git && cd quickcurver && git checkout develop && scripts/build.sh"
