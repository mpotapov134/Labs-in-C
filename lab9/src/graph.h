#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#define NO_EDGE -1

typedef struct Graph {
    int numOfVertices;
    int* adjacencyMatrix;
} Graph_t;

void FreeGraph(Graph_t* graph);

Graph_t* InitializeGraph(Graph_t* graph, int numOfVertices);

int IsAdjacent(const Graph_t* graph, int edgeStart, int edgeEnd);

#endif
