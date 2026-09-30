# Graphing Algorithms Project

An experimental C++ project for studying structural properties of synthetic networks.

It generates **Erdős–Rényi** and **Barabási–Albert** graphs, then measures:

- graph diameter;
- global clustering coefficient;
- degree distribution.

## Build and run

```bash
./run.sh
```

The default run uses small/medium graph sizes so the project finishes in a practical amount of time and writes CSV files to `data_All/`.

To reproduce the original large experiment:

```bash
./run.sh --full
```

You can choose another output folder:

```bash
./run.sh --output results
```

## Test

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic \
  -o graph_tests \
  tests.cpp graph.cpp graph_algorithms.cpp erdos_renyi.cpp barabasi_albert.cpp
./graph_tests
```

The tests use known small graphs to verify diameter, triangle counting, clustering coefficient, degree distribution, unreachable distances, and duplicate-edge handling.

## Implementation notes

- Erdős–Rényi generation uses an edge-skipping algorithm rather than checking every possible pair.
- Barabási–Albert generation uses degree-proportional sampling with distinct targets for each new vertex.
- Large-graph diameter uses several double-sweep BFS passes as an approximation.
- Triangle counting counts each triangle exactly once using increasing node IDs.

The original experimental report is preserved in `Graphing_report.pdf`.
