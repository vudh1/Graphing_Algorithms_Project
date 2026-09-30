#include "graph.h"

#include <algorithm>
#include <queue>
#include <stdexcept>

Graph make_graph(int num_nodes, std::vector<int> u, std::vector<int> v) {
    if (num_nodes < 0) {
        throw std::invalid_argument("num_nodes must be non-negative");
    }
    if (u.size() != v.size()) {
        throw std::invalid_argument("edge endpoint arrays must have equal length");
    }

    Graph graph;
    graph.set_num_nodes(num_nodes);

    const auto nodes = graph.get_id_to_node_map();
    std::set<std::pair<int, int>> unique_edges;

    for (std::size_t i = 0; i < u.size(); ++i) {
        if (u[i] < 0 || v[i] < 0 || u[i] >= num_nodes || v[i] >= num_nodes || u[i] == v[i]) {
            continue;
        }

        const int a = std::min(u[i], v[i]);
        const int b = std::max(u[i], v[i]);
        if (!unique_edges.insert({a, b}).second) {
            continue;
        }

        graph.add_neighbor(nodes.at(a), nodes.at(b));
        graph.add_neighbor(nodes.at(b), nodes.at(a));
    }

    graph.set_num_edges(static_cast<int>(unique_edges.size()));
    return graph;
}

Graph::Graph() : num_nodes(0), num_edges(0) {}

int Graph::get_num_nodes() {
    return num_nodes;
}

void Graph::set_num_nodes(int count) {
    num_nodes = count;
    nodes.clear();
    for (int i = 0; i < count; ++i) {
        nodes.emplace(i, Node(i));
    }
}

int Graph::get_num_edges() {
    return num_edges;
}

void Graph::set_num_edges(int count) {
    num_edges = count;
}

std::map<int, Node> Graph::get_id_to_node_map() {
    return nodes;
}

std::vector<std::pair<Node, Node>> Graph::get_edges() {
    std::vector<std::pair<Node, Node>> edges;
    for (const auto& [id, node] : nodes) {
        for (const Node& neighbor : node.neighbors) {
            if (id < neighbor.id) {
                edges.push_back({node, nodes.at(neighbor.id)});
            }
        }
    }
    return edges;
}

bool Graph::is_neighbor(Node u, Node v) {
    const auto u_it = nodes.find(u.id);
    const auto v_it = nodes.find(v.id);
    if (u_it == nodes.end() || v_it == nodes.end()) {
        return false;
    }

    const auto& neighbors = u_it->second.neighbors;
    return std::find(neighbors.begin(), neighbors.end(), v_it->second) != neighbors.end();
}

std::vector<Node> Graph::get_neighbors(Node u) {
    const auto it = nodes.find(u.id);
    if (it == nodes.end()) {
        return {};
    }
    return it->second.neighbors;
}

void Graph::add_neighbor(Node u, Node v) {
    nodes.at(u.id).add_neighbor(v);
}

std::map<Node, int> Graph::get_all_degrees() {
    std::map<Node, int> degrees;
    for (const auto& [id, node] : nodes) {
        (void)id;
        degrees[node] = node.getDegree();
    }
    return degrees;
}

int Graph::get_distance(Node u, Node v) {
    if (nodes.find(u.id) == nodes.end() || nodes.find(v.id) == nodes.end()) {
        return -1;
    }
    if (u.id == v.id) {
        return 0;
    }

    std::vector<int> distance(nodes.size(), -1);
    std::queue<int> queue;
    distance[u.id] = 0;
    queue.push(u.id);

    while (!queue.empty()) {
        const int current = queue.front();
        queue.pop();

        for (const Node& neighbor : nodes.at(current).neighbors) {
            if (distance[neighbor.id] != -1) {
                continue;
            }

            distance[neighbor.id] = distance[current] + 1;
            if (neighbor.id == v.id) {
                return distance[neighbor.id];
            }
            queue.push(neighbor.id);
        }
    }

    return -1;
}

std::vector<Node> Graph::get_bfs(Node start) {
    std::vector<Node> order;
    if (nodes.find(start.id) == nodes.end()) {
        return order;
    }

    std::vector<bool> visited(nodes.size(), false);
    std::queue<int> queue;
    visited[start.id] = true;
    queue.push(start.id);

    while (!queue.empty()) {
        const int current = queue.front();
        queue.pop();
        order.push_back(nodes.at(current));

        for (const Node& neighbor : nodes.at(current).neighbors) {
            if (!visited[neighbor.id]) {
                visited[neighbor.id] = true;
                queue.push(neighbor.id);
            }
        }
    }

    return order;
}

std::pair<int, Node> Graph::bfs_for_diameter(Node start) {
    if (nodes.find(start.id) == nodes.end()) {
        return {-1, Node()};
    }

    std::vector<int> distance(nodes.size(), -1);
    std::queue<int> queue;
    distance[start.id] = 0;
    queue.push(start.id);

    int max_distance = 0;
    Node farthest = nodes.at(start.id);

    while (!queue.empty()) {
        const int current = queue.front();
        queue.pop();

        for (const Node& neighbor : nodes.at(current).neighbors) {
            if (distance[neighbor.id] != -1) {
                continue;
            }

            distance[neighbor.id] = distance[current] + 1;
            queue.push(neighbor.id);

            if (distance[neighbor.id] > max_distance) {
                max_distance = distance[neighbor.id];
                farthest = nodes.at(neighbor.id);
            }
        }
    }

    return {max_distance, farthest};
}

long long Graph::get_num_two_paths() {
    long long count = 0;
    for (const auto& [id, node] : nodes) {
        (void)id;
        const long long degree = node.getDegree();
        count += degree * (degree - 1) / 2;
    }
    return count;
}

std::pair<std::list<Node>, std::map<Node, std::vector<Node>>> Graph::get_degeneracy() {
    std::list<Node> ordering;
    std::map<Node, std::vector<Node>> forward_neighbors;

    auto degrees = get_all_degrees();
    std::set<Node> remaining;
    for (const auto& [node, degree] : degrees) {
        (void)degree;
        remaining.insert(node);
    }

    while (!remaining.empty()) {
        auto min_it = std::min_element(
            remaining.begin(),
            remaining.end(),
            [&degrees](const Node& a, const Node& b) {
                if (degrees[a] != degrees[b]) {
                    return degrees[a] < degrees[b];
                }
                return a.id < b.id;
            }
        );

        Node v = *min_it;
        remaining.erase(min_it);
        ordering.push_back(v);

        for (const Node& neighbor : get_neighbors(v)) {
            if (remaining.count(neighbor)) {
                forward_neighbors[v].push_back(neighbor);
                --degrees[neighbor];
            }
        }
    }

    return {ordering, forward_neighbors};
}

long long Graph::get_triangles() {
    long long triangles = 0;

    // Count each triangle exactly once using increasing node IDs: u < v < w.
    for (const auto& [u_id, u] : nodes) {
        for (const Node& v : u.neighbors) {
            if (v.id <= u_id) {
                continue;
            }

            for (const Node& w : nodes.at(v.id).neighbors) {
                if (w.id <= v.id) {
                    continue;
                }

                if (is_neighbor(u, w)) {
                    ++triangles;
                }
            }
        }
    }

    return triangles;
}
