#!/bin/bash
# Compile, run both integrators, then plot.   Usage: bash run.sh [--show]
set -e
g++ -std=c++17 -Wall -Wextra -O2 refactor.cpp  -o refactor
./refactor
python3 plot.py "$@"
