#include "graph.hpp"
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>

const int INF = std::numeric_limits<int>::max();

std::vector<int> dijkstra(const Graph& g,
                           int origen,
                           std::vector<int>& prev,
                           int& nodosExplorados) {

    std::vector<int> dist(g.n, INF);
    prev.assign(g.n, -1);
    nodosExplorados = 0;

    std::priority_queue
        std::pair<int,int>,
        std::vector<std::pair<int,int>>,
        std::greater<std::pair<int,int>>
    > heap;

    dist[origen] = 0;
    heap.push({0, origen});

    while (!heap.empty()) {
        auto top = heap.top();
        heap.pop();
        int d = top.first;
        int u = top.second;

        if (d > dist[u]) continue;

        nodosExplorados++;

        for (auto& vecino : g.adj[u]) {
            int v = vecino.first;
            int w = vecino.second;
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                heap.push({dist[v], v});
            }
        }
    }

    return dist;
}

std::vector<int> reconstruirCamino(const std::vector<int>& prev,
                                    int origen,
                                    int destino) {
    std::vector<int> camino;
    if (prev[destino] == -1 && destino != origen) {
        return camino;
    }
    for (int v = destino; v != -1; v = prev[v]) {
        camino.push_back(v);
    }
    std::reverse(camino.begin(), camino.end());
    return camino;
}