#!/usr/bin/bash
set -euf -o pipefail

clang-format -i dmrg.cpp $(find dmrg -name "*.cpp" -or -name "*.hpp")