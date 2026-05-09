#include "graph.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <set>
#include <vector>

Graph loadGraph(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo: " << filename << std::endl;
        exit(1);
    }

    std::cout << "Leyendo archivo..." << std::endl;

    std::set<int> nodeSet;
    std::vector<std::pair<int,int>> edges;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        int u, v;
        if (iss >> u >> v) {
            nodeSet.insert(u);
            nodeSet.insert(v);
            edges.push_back({u, v});
        }
    }
    file.close();

    std::cout << "Nodos unicos encontrados: " << nodeSet.size() << std::endl;
    std::cout << "Lineas de aristas leidas: " << edges.size() << std::endl;

    int n = nodeSet.size();
    Graph g(n);

    int idx = 0;
    for (int originalId : nodeSet) {
        g.idInterno[originalId] = idx;
        g.idOriginal[idx] = originalId;
        idx++;
    }

    srand(42);

    std::set<std::pair<int,int>> seen;
    for (auto& edge : edges) {
        int u = g.idInterno[edge.first];
        int v = g.idInterno[edge.second];
        int w = (rand() % 10) + 1;
        int a = std::min(u, v);
        int b = std::max(u, v);
        if (a != b && seen.find({a, b}) == seen.end()) {
            seen.insert({a, b});
            g.addEdge(u, v, w);
        }
    }

    std::cout << "Grafo construido: " << g.n << " nodos, "
              << g.m << " aristas" << std::endl;

    return g;
}