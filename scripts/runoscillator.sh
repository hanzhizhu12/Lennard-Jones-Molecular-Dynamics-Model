#!/bin/bash
# Compile, run both integrators, then plot.   Usage: bash scripts/runoscillator.sh [--show]
set -e
source .venv/bin/activate
mkdir -p build
g++ -std=c++17 -Wall -Wextra -O2 src/oscillator.cpp -o build/oscillator
./build/oscillator
python3 scripts/plotoscillator.py "$@"