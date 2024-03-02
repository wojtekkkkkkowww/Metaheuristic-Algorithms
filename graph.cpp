#include <iostream>
#include <vector>
#include <limits>
#include "graph.hpp"



int minKey(const std::vector<int>& key, const std::vector<bool>& inMST, int n) {
    int arg = -1;
    int min = std::numeric_limits<int>::max();

    for (int i = 0; i < n; i++) {
        if (min > key[i] && !inMST[i]) {
            min = key[i];
            arg = i;
        }
    }

    return arg;
}


MST::MST(const std::vector<int>& weights, int n, int r) {
    std::vector<int> key(n, std::numeric_limits<int>::max());
    std::vector<int> parent(n, -1);
    std::vector<bool> inMST(n, false);
    wt = 0;
    key[r] = 0;

    for (int i = 0; i < n - 1; i++) {
        int u = minKey(key, inMST, n);
        inMST[u] = true;

        for (int v = 0; v < n; ++v) {
            if (v != u && !inMST[v] && weights[u*n + v] < key[v]) {
                parent[v] = u;
                key[v] = weights[u*n + v];
            }
        }
    }
    this -> V = n;
    this -> adjList = std::vector<std::vector<int>>(n);

    
    for (int v = 1; v < n; ++v) {
        this -> addEdge(parent[v], v);
        this -> wt += weights[v*n + parent[v]];
    }

}


void MST:: addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

void MST:: DFSvisit(int u, std::vector<bool>& visited, std::vector<int>& result) {
        visited[u] = true;
        result.push_back(u);

        for (int v : adjList[u]) {
            if (!visited[v]) {
                DFSvisit(v, visited, result);
            }
        }
    }

std::vector<int> MST:: DFS(int startVertex) {
        std::vector<bool> visited(V, false);
        std::vector<int> result;

        DFSvisit(startVertex, visited, result);

        return result;
    }

void MST:: deleteMST() {
        adjList.clear();
        V = 0;
}


int MST:: getWeight() {
        return this -> wt;
    }
