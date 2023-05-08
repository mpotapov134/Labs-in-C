#include <stdlib.h>
#include "mst.h"

void FreeMST(MST_t* minSpanningTree) {
    free(minSpanningTree->vertices);
    minSpanningTree->vertices = NULL;
}

MST_t* InitializeMST(MST_t* minSpanningTree, int numOfVertices) {
    minSpanningTree->mstSize = 0;
    minSpanningTree->vertices = malloc(numOfVertices * sizeof(*minSpanningTree->vertices));
    if (!minSpanningTree->vertices) {
        return NULL;
    }
    return minSpanningTree;
}

void AddToMST(MST_t* minSpanningTree, int parent, int vertex) {
    minSpanningTree->vertices[minSpanningTree->mstSize].parent = parent + 1;
    minSpanningTree->vertices[minSpanningTree->mstSize].vertex = vertex + 1;
    minSpanningTree->mstSize++;
}
