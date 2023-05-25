#include <stdlib.h>
#include "graph.h"

void FreeGraph(Graph_t* graph) {
    free(graph->adjacencyMatrix);
    graph->adjacencyMatrix = NULL;
}

Graph_t* InitializeGraph(Graph_t* graph, int numOfVertices) {
    graph->numOfVertices = numOfVertices;
    graph->adjacencyMatrix = malloc(numOfVertices * numOfVertices * sizeof(*graph->adjacencyMatrix));
    if (!graph->adjacencyMatrix) {
        return NULL;
    }
    for (int i = 0; i < numOfVertices; ++i) {
        for (int j = 0; j < numOfVertices; ++j) {
            graph->adjacencyMatrix[i * numOfVertices + j] = NO_EDGE;
        }
    }
    return graph;
}

int IsAdjacent(const Graph_t* graph, int edgeStart, int edgeEnd) {
    int edgeLen = graph->adjacencyMatrix[edgeStart * graph->numOfVertices + edgeEnd];
    return edgeLen == NO_EDGE ? 0 : 1;
}
