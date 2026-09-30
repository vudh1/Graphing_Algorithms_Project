#ifndef PROJECT3_H_INCLUDED
#define PROJECT3_H_INCLUDED

#include "graph.h"
#include "node.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <vector>

Graph make_graph(int num_nodes, std::vector<int> u, std::vector<int> v);

int get_naive_diameter(Graph graph);
int get_diameter(Graph graph);
double get_clustering_coefficient(Graph graph);
std::map<int, int> get_degree_distribution(Graph graph);

Graph create_erdos_renyi_graph(int n, double p);
Graph create_barabasi_albert_graph(int n, int d);

#endif
