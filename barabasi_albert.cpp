#include "project3.h"

#include <algorithm>
#include <unordered_set>

namespace {
std::mt19937_64& generator() {
    static std::mt19937_64 value(std::random_device{}());
    return value;
}
}

Graph create_barabasi_albert_graph(int n, int d) {
    if (n <= 0) {
        return make_graph(0, {}, {});
    }
    if (n == 1) {
        return make_graph(1, {}, {});
    }

    d = std::max(1, std::min(d, n - 1));
    const int seed_size = std::min(n, d + 1);

    std::vector<int> first;
    std::vector<int> second;
    std::vector<int> repeated_nodes;

    // Start with a complete seed graph so every seed vertex has non-zero degree.
    for (int u = 0; u < seed_size; ++u) {
        for (int v = u + 1; v < seed_size; ++v) {
            first.push_back(u);
            second.push_back(v);
            repeated_nodes.push_back(u);
            repeated_nodes.push_back(v);
        }
    }

    for (int v = seed_size; v < n; ++v) {
        std::unordered_set<int> targets;
        std::uniform_int_distribution<std::size_t> distribution(0, repeated_nodes.size() - 1);

        while (static_cast<int>(targets.size()) < d) {
            targets.insert(repeated_nodes[distribution(generator())]);
        }

        for (int target : targets) {
            first.push_back(v);
            second.push_back(target);

            // Each endpoint gains one degree, so append both to the sampling pool.
            repeated_nodes.push_back(v);
            repeated_nodes.push_back(target);
        }
    }

    return make_graph(n, first, second);
}
