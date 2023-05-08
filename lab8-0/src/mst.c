#include <stdlib.h>
#include "mst.h"

void FreeMST(MST_t* mst) {
    free(mst->edges);
    mst->edges = NULL;
}

MST_t* InitializeMST(MST_t* mst, int numOfVertices) {
    mst->bufferSize = 0;
    mst->edges = calloc(numOfVertices - 1, sizeof(*mst->edges)); // Ребер в каркасе на 1 меньше, чем вершин
    if (!mst->edges) {
        return NULL;
    }
    return mst;
}

void AddToMST(MST_t* minSpanningTree, Edge_t edge) {
    minSpanningTree->edges[minSpanningTree->bufferSize] = edge;
    minSpanningTree->bufferSize++;
}
