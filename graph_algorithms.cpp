#include "project3.h"

#include <algorithm>

int get_naive_diameter(Graph graph) {
    int max_diameter = 0;
    const auto nodes = graph.get_id_to_node_map();
    const int size = static_cast<int>(nodes.size());

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            const int distance = graph.get_distance(nodes.at(i), nodes.at(j));
            if (distance > max_diameter) {
                max_diameter = distance;
            }
        }
    }

    return max_diameter;
}

int get_diameter(Graph graph) {
    const auto nodes = graph.get_id_to_node_map();
    const int size = static_cast<int>(nodes.size());

    if (size <= 1) {
        return 0;
    }
    if (size <= 100) {
        return get_naive_diameter(graph);
    }

    // Approximate large-graph diameter with several double-sweep BFS runs.
    std::mt19937 generator(42);
    std::uniform_int_distribution<int> distribution(0, size - 1);
    int best = 0;

    const int sweeps = std::min(8, size);
    for (int i = 0; i < sweeps; ++i) {
        Node start = nodes.at(distribution(generator));
        auto first = graph.bfs_for_diameter(start);
        if (first.first < 0) {
            continue;
        }
        auto second = graph.bfs_for_diameter(first.second);
        best = std::max(best, second.first);
    }

    return best;
}

double get_clustering_coefficient(Graph graph) {
    const long long two_paths = graph.get_num_two_paths();
    if (two_paths == 0) {
        return 0.0;
    }

    const long long triangles = graph.get_triangles();
    return 3.0 * static_cast<double>(triangles) / static_cast<double>(two_paths);
}

std::map<int, int> get_degree_distribution(Graph graph) {
    std::map<int, int> histogram;

    for (const auto& [node, degree] : graph.get_all_degrees()) {
        (void)node;
        ++histogram[degree];
    }

    return histogram;
}
