#!/usr/bin/env bash

# fail on error
set -e

mkdir -p build
cd build
qmake -config release ..
make
