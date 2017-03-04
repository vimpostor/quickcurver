#!/usr/bin/env bash

sed -i.bak 's/git@github.com:Magnus2552\/qml-material.git/https:\/\/github.com\/Magnus2552\/qml-material.git/' .gitmodules
git submodule init
git submodule update
