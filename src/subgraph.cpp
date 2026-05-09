#include "graph.hpp"
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <functional>
#include <tuple>

struct UnionFind {
    std::vector<int> padre;
    std::vector<int> rango;

    UnionFind(int n) : padre(n), rango(n, 0) {
        for (int i = 0; i < n; i++) padre[i] = i;
    }

    int find(int x) {
        if (padre[x] != x) padre[x] = find(padre[x]);
        return padre[x];
    }

    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;
        if (rango[px] < rango[py]) std::swap(px, py);
        padre[py] = px;
        if (rango[px] == rango[py]) rango[px]++;
        return true;
    }
};

Graph construirSubgrafo(const Graph& g, const std::vector<int>& nodos) {
    std::set<int> conjNodos(nodos.begin(), nodos.end());
    std::map<int,int> mapaLocal;
    int idx = 0;
    for (int n : conjNodos) mapaLocal[n] = idx++;

    int k = (int)conjNodos.size();
    Graph sub(k);

    for (auto& par : mapaLocal)
        sub.idOriginal[par.second] = g.idOriginal[par.first];

    std::set<std::pair<int,int>> aristasVistas;
    for (int u : conjNodos) {
        for (auto& vecino : g.adj[u]) {
            int v = vecino.first;
            int w = vecino.second;
            if (conjNodos.count(v)) {
                int lu = mapaLocal[u];
                int lv = mapaLocal[v];
                int a = std::min(lu, lv);
                int b = std::max(lu, lv);
                if (!aristasVistas.count({a, b})) {
                    aristasVistas.insert({a, b});
                    sub.addEdge(lu, lv, w);
                }
            }
        }
    }
    return sub;
}

int kruskal(const Graph& sub) {
    std::vector<std::tuple<int,int,int>> aristas;
    for (int u = 0; u < sub.n; u++)
        for (auto& vecino : sub.adj[u])
            if (u < vecino.first)
                aristas.push_back(std::make_tuple(vecino.second, u, vecino.first));

    std::sort(aristas.begin(), aristas.end());

    UnionFind uf(sub.n);
    int pesoMST = 0, aristasEnMST = 0;

    for (auto& arista : aristas) {
        int w = std::get<0>(arista);
        int u = std::get<1>(arista);
        int v = std::get<2>(arista);
        if (uf.unite(u, v)) {
            pesoMST += w;
            aristasEnMST++;
            if (aristasEnMST == sub.n - 1) break;
        }
    }
    return pesoMST;
}

bool tieneCiclo(const Graph& sub) {
    std::vector<int> color(sub.n, 0);
    std::function<bool(int,int)> dfs = [&](int u, int padre) -> bool {
        color[u] = 1;
        for (auto& vecino : sub.adj[u]) {
            int v = vecino.first;
            if (v == padre) continue;
            if (color[v] == 1) return true;
            if (color[v] == 0 && dfs(v, u)) return true;
        }
        color[u] = 2;
        return false;
    };
    for (int i = 0; i < sub.n; i++)
        if (color[i] == 0 && dfs(i, -1)) return true;
    return false;
}

void analizarSubgrafo(const Graph& g,
                       const std::vector<int>& caminoQ01,
                       const std::vector<int>& caminoQ06,
                       const std::string& archivoSub,
                       const std::string& archivoAnalisis) {

    std::vector<int> todosNodos;
    for (int n : caminoQ01) todosNodos.push_back(n);
    for (int n : caminoQ06) todosNodos.push_back(n);

    std::cout << "Construyendo subgrafo inducido..." << std::endl;
    Graph sub = construirSubgrafo(g, todosNodos);
    std::cout << "Subgrafo: " << sub.n << " nodos, " << sub.m << " aristas" << std::endl;

    int pesoMST = kruskal(sub);
    bool ciclo  = tieneCiclo(sub);
    bool esDAG  = !ciclo;

    std::ofstream outSub(archivoSub);
    outSub << "# Subgrafo inducido por caminos Q01 y Q06\n";
    outSub << sub.n << " " << sub.m << "\n";
    for (int u = 0; u < sub.n; u++)
        for (auto& vecino : sub.adj[u])
            if (u < vecino.first)
                outSub << u << " " << vecino.first << " " << vecino.second << "\n";
    outSub.close();

    std::ofstream outA(archivoAnalisis);
    outA << "=== ANALISIS DEL SUBGRAFO INDUCIDO ===\n\n";
    outA << "Nodos Q01: " << caminoQ01.size() << "\n";
    outA << "Nodos Q06: " << caminoQ06.size() << "\n";
    outA << "Nodos subgrafo: " << sub.n << "\n";
    outA << "Aristas subgrafo: " << sub.m << "\n";
    outA << "Peso MST (Kruskal): " << pesoMST << "\n";
    outA << "Tiene ciclos: " << (ciclo ? "SI" : "NO") << "\n";
    outA << "Es DAG: " << (esDAG ? "SI" : "NO") << "\n";
    outA.close();

    std::cout << "Peso MST: " << pesoMST << std::endl;
    std::cout << "Es DAG: " << (esDAG ? "SI" : "NO") << std::endl;
}
