#!/bin/bash

set -euf -o pipefail

iwyu-tool \
    -p build \
    -j $(nproc --all) \
    > .iwyu.out