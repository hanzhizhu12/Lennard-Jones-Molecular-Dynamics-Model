set -e
source .venv/bin/activate
g++ -std=c++17 -Wall -Wextra -O2 ljN.cpp  -o ljN
./ljN
python3 plotljN.py "$@"