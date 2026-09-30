#include "project3.h"

#include <algorithm>
#include <limits>

namespace {
std::mt19937_64& generator() {
    static std::mt19937_64 value(std::random_device{}());
    return value;
}
}

Graph create_erdos_renyi_graph(int n, double p) {
    if (n <= 0) {
        return make_graph(0, {}, {});
    }

    p = std::clamp(p, 0.0, 1.0);
    if (p == 0.0 || n == 1) {
        return make_graph(n, {}, {});
    }

    std::vector<int> first;
    std::vector<int> second;

    if (p == 1.0) {
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                first.push_back(u);
                second.push_back(v);
            }
        }
        return make_graph(n, first, second);
    }

    std::uniform_real_distribution<double> uniform(
        std::nextafter(0.0, 1.0),
        std::nextafter(1.0, 0.0)
    );

    // Batagelj-Brandes edge-skipping generator for G(n, p).
    int v = 1;
    int w = -1;
    const double log_one_minus_p = std::log1p(-p);

    while (v < n) {
        const double r = uniform(generator());
        w += 1 + static_cast<int>(std::floor(std::log1p(-r) / log_one_minus_p));

        while (w >= v && v < n) {
            w -= v;
            ++v;
        }

        if (v < n) {
            first.push_back(v);
            second.push_back(w);
        }
    }

    return make_graph(n, first, second);
}
