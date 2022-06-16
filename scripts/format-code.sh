#!/usr/bin/env sh

git ls-files| grep -E '.*\.[ch]pp'| xargs clang-format -style=file -i
