#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class MST {
private:
    int V;
    int wt;
    std::vector<std::vector<int>> adjList;

public:
    MST(const std::vector<int>& weights, int n, int r);
    void addEdge(int u, int v);
    void DFSvisit(int u, std::vector<bool>& visited, std::vector<int>& result);
    std::vector<int> DFS(int startVertex);
    void deleteMST();
    int getWeight();
};

int minKey(const std::vector<int>& key, const std::vector<bool>& inMST);

#endif
