#!/usr/bin/env bash

sed -i.bak 's/git@github.com:magnus-gross\/qml-material.git/https:\/\/github.com\/magnus-gross\/qml-material.git/' .gitmodules
git submodule init
git submodule update
