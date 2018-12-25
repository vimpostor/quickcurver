#!/usr/bin/env bash

mkdir build
cd build
qmake -config release ..
make
