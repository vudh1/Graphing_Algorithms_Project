#!/usr/bin/env bash
set -euo pipefail

g++ -std=c++17 -O2 -Wall -Wextra -pedantic \
  -o Project3.out \
  main.cpp graph.cpp graph_algorithms.cpp erdos_renyi.cpp barabasi_albert.cpp

./Project3.out "$@"
