#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <iostream>
#include <vector>

class Node {
public:
    int id;
    std::vector<Node> neighbors;

    Node() : id(-1) {}
    explicit Node(int node_id) : id(node_id) {}

    void add_neighbor(const Node& node) { neighbors.push_back(node); }
    int getDegree() const { return static_cast<int>(neighbors.size()); }

    bool operator==(const Node& other) const { return id == other.id; }
    bool operator!=(const Node& other) const { return id != other.id; }
    bool operator>(const Node& other) const { return id > other.id; }
    bool operator>=(const Node& other) const { return id >= other.id; }
    bool operator<(const Node& other) const { return id < other.id; }
    bool operator<=(const Node& other) const { return id <= other.id; }

    friend std::ostream& operator<<(std::ostream& os, const Node& node) {
        os << node.id;
        return os;
    }
};

#endif
