set -e
g++ -std=c++17 -Wall -Wextra -O2 lj2.cpp  -o lj2
./lj2
python3 plotlj2.py "$@"