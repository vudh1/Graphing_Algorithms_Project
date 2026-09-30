#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

#include <list>
#include <map>
#include <set>
#include <utility>
#include <vector>

class AbstractGraph {
public:
    virtual ~AbstractGraph() = default;
    virtual int get_num_nodes() = 0;
    virtual int get_num_edges() = 0;
    virtual bool is_neighbor(Node u, Node v) = 0;
    virtual std::vector<Node> get_neighbors(Node u) = 0;
    virtual std::map<int, Node> get_id_to_node_map() = 0;
};

class Graph : public AbstractGraph {
private:
    int num_nodes;
    int num_edges;
    std::map<int, Node> nodes;

public:
    Graph();

    int get_num_nodes() override;
    void set_num_nodes(int count);

    int get_num_edges() override;
    void set_num_edges(int count);

    std::map<int, Node> get_id_to_node_map() override;
    std::vector<std::pair<Node, Node>> get_edges();

    bool is_neighbor(Node u, Node v) override;
    std::vector<Node> get_neighbors(Node u) override;
    void add_neighbor(Node u, Node v);

    std::map<Node, int> get_all_degrees();
    int get_distance(Node u, Node v);
    std::vector<Node> get_bfs(Node start);
    std::pair<int, Node> bfs_for_diameter(Node start);

    long long get_num_two_paths();
    std::pair<std::list<Node>, std::map<Node, std::vector<Node>>> get_degeneracy();
    long long get_triangles();
};

#endif
