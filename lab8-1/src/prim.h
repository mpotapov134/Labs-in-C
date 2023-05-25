#ifndef PRIM_H_INCLUDED
#define PRIM_H_INCLUDED

#include "graph.h"
#include "queue.h"
#include "mst.h"

#define MAX_NUMBER 5000

enum PrimExitCodes {
    BAD_NUMBER_OF_LINES = 1,
    BAD_NUMBER_OF_VERTICES,
    BAD_NUMBER_OF_EDGES,
    BAD_VERTEX,
    BAD_LENGTH,
    NO_SPANNING_TREE,
};

void FreeAll(Graph_t* graph, Queue_t* queue, MST_t* minSpanningTree);

int ReadSize(int* numOfVertices, int* numOfEdges);

int ReadEdges(Graph_t* graph, int numOfEdges);

int Prims(const Graph_t* graph, Queue_t* queue, MST_t* minSpanningTree);

#endif
