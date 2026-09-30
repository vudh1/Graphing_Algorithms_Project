#include "project3.h"

#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    {
        Graph triangle = make_graph(3, {0, 1, 0}, {1, 2, 2});
        assert(triangle.get_num_nodes() == 3);
        assert(triangle.get_num_edges() == 3);
        assert(triangle.get_triangles() == 1);
        assert(get_diameter(triangle) == 1);
        assert(std::abs(get_clustering_coefficient(triangle) - 1.0) < 1e-9);
        auto degrees = get_degree_distribution(triangle);
        assert(degrees[2] == 3);
    }

    {
        Graph path = make_graph(4, {0, 1, 2}, {1, 2, 3});
        auto nodes = path.get_id_to_node_map();
        assert(path.get_distance(nodes[0], nodes[3]) == 3);
        assert(get_diameter(path) == 3);
        assert(path.get_triangles() == 0);
        assert(get_clustering_coefficient(path) == 0.0);
    }

    {
        Graph disconnected = make_graph(3, {0}, {1});
        auto nodes = disconnected.get_id_to_node_map();
        assert(disconnected.get_distance(nodes[0], nodes[2]) == -1);
    }

    {
        Graph deduplicated = make_graph(3, {0, 1, 0, 0}, {1, 0, 1, 0});
        assert(deduplicated.get_num_edges() == 1);
    }

    {
        Graph er = create_erdos_renyi_graph(25, 0.25);
        assert(er.get_num_nodes() == 25);
        assert(er.get_num_edges() >= 0);
    }

    {
        Graph ba = create_barabasi_albert_graph(50, 5);
        assert(ba.get_num_nodes() == 50);
        assert(ba.get_num_edges() > 0);
    }

    std::cout << "Graph tests passed.\n";
    return 0;
}
