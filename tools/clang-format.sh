#!/usr/bin/bash
set -euf -o pipefail

clang-format -i mpskit.cpp $(find mpskit -name "*.cpp" -or -name "*.hpp")