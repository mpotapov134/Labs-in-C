#include <stdlib.h>
#include <stdio.h>
#include "prim.h"

enum ExitCodes {
    OUT_OF_MEMORY = 1,
};

int main() {
    int numOfVertices, numOfEdges;
    if (ReadSize(&numOfVertices, &numOfEdges) != EXIT_SUCCESS) {
        return EXIT_SUCCESS;
    }
    if (numOfVertices == 0) {
        printf("no spanning tree\n");
        return EXIT_SUCCESS;
    }

    Graph_t graph;
    Queue_t queue;
    MST_t minSpanningTree;

    if (!InitializeGraph(&graph, numOfVertices) ||
            !InitializeQueue(&queue, numOfVertices) ||
            !InitializeMST(&minSpanningTree, numOfVertices)) {
        FreeAll(&graph, &queue, &minSpanningTree);
        return OUT_OF_MEMORY;
    }

    if (ReadEdges(&graph, numOfEdges) != EXIT_SUCCESS) {
        FreeAll(&graph, &queue, &minSpanningTree);
        return EXIT_SUCCESS;
    }

    if (Prims(&graph, &queue, &minSpanningTree) == NO_SPANNING_TREE) {
        printf("no spanning tree\n");
        FreeAll(&graph, &queue, &minSpanningTree);
        return EXIT_SUCCESS;
    }

    PrintMST(&minSpanningTree);
    FreeAll(&graph, &queue, &minSpanningTree);
}
