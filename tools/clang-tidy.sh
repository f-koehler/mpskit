#!/bin/bash
set -euf -o pipefail

run-clang-tidy \
    -p build \
    -j $(nproc --all) \
    -fix