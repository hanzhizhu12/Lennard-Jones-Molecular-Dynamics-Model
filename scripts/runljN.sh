set -e
source .venv/bin/activate
mkdir -p build
g++ -std=c++17 -Wall -Wextra -O2 src/ljN.cpp -o build/ljN
./build/ljN
python3 scripts/plotljN.py "$@"