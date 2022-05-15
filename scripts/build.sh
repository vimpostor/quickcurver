#!/usr/bin/env bash

set -e

cmake -B build -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build
