#include "graph.hpp"
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>

const int INF = std::numeric_limits<int>::max();

typedef std::pair<int,int> pii;

std::vector<int> dijkstra(const Graph& g,
                           int origen,
                           std::vector<int>& prev,
                           int& nodosExplorados) {

    std::vector<int> dist(g.n, INF);
    prev.assign(g.n, -1);
    nodosExplorados = 0;

    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> heap;

    dist[origen] = 0;
    heap.push(pii(0, origen));

    while (!heap.empty()) {
        pii top = heap.top();
        heap.pop();
        int d = top.first;
        int u = top.second;

        if (d > dist[u]) continue;

        nodosExplorados++;

        for (int i = 0; i < (int)g.adj[u].size(); i++) {
            int v = g.adj[u][i].first;
            int w = g.adj[u][i].second;
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                heap.push(pii(dist[v], v));
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