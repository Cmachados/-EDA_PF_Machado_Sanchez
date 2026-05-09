#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <utility>
#include <map>

struct Graph {
    int n;
    int m;
    std::vector<std::vector<std::pair<int,int>>> adj;
    std::vector<int> idOriginal;
    std::map<int,int> idInterno;

    Graph() : n(0), m(0) {}

    Graph(int n) : n(n), m(0), adj(n), idOriginal(n, 0) {}

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        m++;
    }
};

#endif