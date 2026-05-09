#include "graph.hpp"
#include <queue>
#include <vector>
#include <iostream>

std::vector<int> bfs(const Graph& g, int origen, int& nodosExplorados) {
    std::vector<int> dist(g.n, -1);
    std::queue<int> cola;

    dist[origen] = 0;
    cola.push(origen);
    nodosExplorados = 0;

    while (!cola.empty()) {
        int u = cola.front();
        cola.pop();
        nodosExplorados++;

        for (auto& par : g.adj[u]) {
            int v = par.first;
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                cola.push(v);
            }
        }
    }

    return dist;
}

int contarComponentes(const Graph& g, int& nodosEnComponentePrincipal) {
    std::vector<bool> visitado(g.n, false);
    int componentes = 0;
    nodosEnComponentePrincipal = 0;

    for (int i = 0; i < g.n; i++) {
        if (!visitado[i]) {
            componentes++;
            std::queue<int> cola;
            cola.push(i);
            visitado[i] = true;
            int tamano = 0;

            while (!cola.empty()) {
                int u = cola.front();
                cola.pop();
                tamano++;
                for (auto& par : g.adj[u]) {
                    int v = par.first;
                    if (!visitado[v]) {
                        visitado[v] = true;
                        cola.push(v);
                    }
                }
            }

            if (tamano > nodosEnComponentePrincipal) {
                nodosEnComponentePrincipal = tamano;
            }
        }
    }

    return componentes;
}