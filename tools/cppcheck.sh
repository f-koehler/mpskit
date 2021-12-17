#!/usr/bin/bash
set -euf -o pipefail

cppcheck \
    -j $(nproc --all) \
    --std=c++17 \
    --language=c++ \
    --enable=all \
    --suppressions-list=.cppcheck-suppressions \
    --error-exitcode=1 \
    --project=build/compile_commands.json