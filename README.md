# Práctica Final - Grafos

Estructuras de Datos y Algoritmos 1 - EAFIT 2026

Cristóbal Machado Sánchez

---

## Qué hace el proyecto

Trabaja con el dataset de carreteras de Pennsylvania (roadNet-PA) que tiene como 1 millon de nodos. 
Se implementaron tres modulos:

- Modulo A: carga el grafo y saca estadisticas basicas
- Modulo B: encuentra caminos mas cortos con Dijkstra y BFS entre 10 pares de nodos
- Modulo C: construye un subgrafo con los caminos encontrados y le aplica Kruskal para el MST

---

## Como compilar

```bash
g++ -std=c++17 -O2 -o eda_pf src/main.cpp
```

## Como correr

```bash
./eda_pf A data/roadNet-PA.txt
./eda_pf B data/roadNet-PA.txt
./eda_pf C data/roadNet-PA.txt
```

hay que correr el B antes del C porque el C usa los caminos que genera el B

---

## Dataset

Se descarga asi:

```bash
cd data
curl -L -o roadNet-PA.txt.gz https://snap.stanford.edu/data/roadNet-PA.txt.gz
gunzip roadNet-PA.txt.gz
```

fuente: http://snap.stanford.edu/data

---

## Herramientas

Use Claude de Anthropic para ayudarme con partes del codigo que no entiendia bien,
especialmente la parte del heap en Dijkstra y el Union-Find de Kruskal.