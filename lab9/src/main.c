#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

enum ExitCodes {
    OUT_OF_MEMORY = -1,
};

int main(void) {
    int numOfVertices, start, finish, numOfEdges;
    if (ReadData(&numOfVertices, &start, &finish, &numOfEdges) != EXIT_SUCCESS) {
        return EXIT_SUCCESS;
    }

    Graph_t graph;
    Vertices_t vertices;
    int* path = malloc(numOfVertices * sizeof(*path));

    if (!path || !InitializeGraph(&graph, numOfVertices) || !InitializeVertices(&vertices, numOfVertices)) {
        FreeAll(&graph, &vertices, path);
        return OUT_OF_MEMORY;
    }

    if (ReadEdges(&graph, numOfEdges) != EXIT_SUCCESS) {
        FreeAll(&graph, &vertices, path);
        return EXIT_SUCCESS;
    }

    int hasOverflow = 0;
    Dijkstras(&graph, &vertices, path, start, finish, &hasOverflow);
    PrintOutput(&vertices, path, start, finish, hasOverflow);

    FreeAll(&graph, &vertices, path);
}
