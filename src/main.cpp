#include "graph.hpp"
#include "loader.cpp"
#include "dijkstra.cpp"
#include "structural.cpp"
#include "subgraph.cpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Uso: ./eda_pf [A|B|C] [ruta_archivo]" << std::endl;
        return 1;
    }

    std::string modulo  = argv[1];
    std::string archivo = (argc >= 3) ? argv[2] : "data/roadNet-PA.txt";

    std::cout << "Cargando grafo desde: " << archivo << std::endl;
    Graph g = loadGraph(archivo);
    std::cout << "Carga completada." << std::endl << std::endl;

    if (modulo == "A") {
        std::cout << "=== MODULO A: Analisis Estructural ===" << std::endl;
        guardarAnalisisEstructural(g, "results/analisis_estructural.txt");
    }

    else if (modulo == "B") {
        std::cout << "=== MODULO B: Consultas P2P ===" << std::endl;

        struct Consulta { int origenOriginal; int destinoOriginal; };
        std::vector<Consulta> consultas = {
            {1, 500000}, {100, 1000000}, {50000, 750000},
            {200000, 800000}, {300000, 100000}, {1, 1087562},
            {500000, 1}, {250000, 600000}, {10000, 900000},
            {400000, 150000}
        };

        std::ofstream csv("results/consultas_p2p.csv");
        csv << "consulta,origen,destino,dist_dijkstra,saltos_bfs,"
            << "nodos_dijkstra,nodos_bfs,t_dijkstra_ms,t_bfs_ms" << std::endl;

        std::vector<int> caminoQ01, caminoQ06;

        for (int i = 0; i < (int)consultas.size(); i++) {
            int origId = consultas[i].origenOriginal;
            int destId = consultas[i].destinoOriginal;

            std::cout << "Procesando Q" << (i+1)
                      << " (" << origId << " -> " << destId << ")..." << std::endl;

            if (g.idInterno.find(origId) == g.idInterno.end() ||
                g.idInterno.find(destId) == g.idInterno.end()) {
                std::cout << "  -> Nodo no encontrado, saltando." << std::endl;
                csv << "Q" << (i+1) << "," << origId << "," << destId
                    << ",INF,-1,-1,-1,-1,-1" << std::endl;
                continue;
            }

            int origen  = g.idInterno[origId];
            int destino = g.idInterno[destId];

            std::vector<int> prev;
            int nodosD = 0;
            auto t1 = std::chrono::high_resolution_clock::now();
            std::vector<int> distD = dijkstra(g, origen, prev, nodosD);
            auto t2 = std::chrono::high_resolution_clock::now();
            double tiempoD = std::chrono::duration<double,std::milli>(t2-t1).count();

            int nodosB = 0;
            auto t3 = std::chrono::high_resolution_clock::now();
            std::vector<int> distB = bfs(g, origen, nodosB);
            auto t4 = std::chrono::high_resolution_clock::now();
            double tiempoB = std::chrono::duration<double,std::milli>(t4-t3).count();

            std::string distDijkstra = (distD[destino] == INF)
                                        ? "INF"
                                        : std::to_string(distD[destino]);
            int saltosBFS = distB[destino];

            std::cout << "  Dijkstra: " << distDijkstra
                      << " | BFS: " << saltosBFS
                      << " | t_D: " << tiempoD << "ms"
                      << " | t_B: " << tiempoB << "ms" << std::endl;

            csv << "Q" << (i+1) << "," << origId << "," << destId << ","
                << distDijkstra << "," << saltosBFS << ","
                << nodosD << "," << nodosB << ","
                << tiempoD << "," << tiempoB << std::endl;

            if (i == 0 && distDijkstra != "INF")
                caminoQ01 = reconstruirCamino(prev, origen, destino);
            if (i == 5 && distDijkstra != "INF")
                caminoQ06 = reconstruirCamino(prev, origen, destino);
        }
        csv.close();

        std::ofstream camFile("results/caminos_q01_q06.txt");
        camFile << "Q01 " << caminoQ01.size() << "\n";
        for (int n : caminoQ01) camFile << n << " ";
        camFile << "\n";
        camFile << "Q06 " << caminoQ06.size() << "\n";
        for (int n : caminoQ06) camFile << n << " ";
        camFile << "\n";
        camFile.close();

        std::cout << "Guardado en results/consultas_p2p.csv" << std::endl;
    }

    else if (modulo == "C") {
        std::cout << "=== MODULO C: Subgrafo, MST y DAG ===" << std::endl;

        std::ifstream camFile("results/caminos_q01_q06.txt");
        if (!camFile.is_open()) {
            std::cerr << "Error: primero ejecuta el Modulo B." << std::endl;
            return 1;
        }

        std::vector<int> caminoQ01, caminoQ06;
        std::string etiqueta;
        int tamano;

        camFile >> etiqueta >> tamano;
        camFile.ignore();
        std::string lineaQ01;
        std::getline(camFile, lineaQ01);
        std::istringstream ss1(lineaQ01);
        int nodo;
        while (ss1 >> nodo) caminoQ01.push_back(nodo);

        camFile >> etiqueta >> tamano;
        camFile.ignore();
        std::string lineaQ06;
        std::getline(camFile, lineaQ06);
        std::istringstream ss2(lineaQ06);
        while (ss2 >> nodo) caminoQ06.push_back(nodo);
        camFile.close();

        std::cout << "Q01: " << caminoQ01.size() << " nodos" << std::endl;
        std::cout << "Q06: " << caminoQ06.size() << " nodos" << std::endl;

        analizarSubgrafo(g, caminoQ01, caminoQ06,
                         "results/subgrafo_caminos.txt",
                         "results/analisis_subgrafo.txt");
    }

    else {
        std::cerr << "Modulo desconocido. Usa A, B o C." << std::endl;
        return 1;
    }

    return 0;
}