#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "prim.h"

void FreeAll(Graph_t* graph, Queue_t* queue, MST_t* minSpanningTree) {
    FreeGraph(graph);
    FreeQueue(queue);
    FreeMST(minSpanningTree);
}

int ReadSize(int* numOfVertices, int* numOfEdges) {
    if (scanf("%i %i", numOfVertices, numOfEdges) != 2) {
        printf("bad number of lines\n");
        return BAD_NUMBER_OF_LINES;
    }
    if (*numOfVertices < 0 || *numOfVertices > MAX_NUMBER) {
        printf("bad number of vertices\n");
        return BAD_NUMBER_OF_VERTICES;
    }
    if (*numOfEdges < 0 || *numOfEdges > *numOfVertices * (*numOfVertices - 1) / 2) {
        printf("bad number of edges\n");
        return BAD_NUMBER_OF_EDGES;
    }
    return EXIT_SUCCESS;
}

int ReadEdges(Graph_t* graph, int numOfEdges) {
    for (int i = 0; i < numOfEdges; ++i) {
        int start, end;
        long long length;
        if (scanf("%i %i %lli", &start, &end, &length) != 3) {
            printf("bad number of lines\n");
            return BAD_NUMBER_OF_LINES;
        }

        if (start < 1 || end < 1 || start > graph->numOfVertices || end > graph->numOfVertices) {
            printf("bad vertex\n");
            return BAD_VERTEX;
        }

        if (length < 0 || length > INT_MAX) {
            printf("bad length\n");
            return BAD_LENGTH;
        }
        /* Граф неориентированный, матрица должна быть симметричной */
        graph->adjacencyMatrix[(start - 1) * graph->numOfVertices + (end - 1)] = length;
        graph->adjacencyMatrix[(end - 1) * graph->numOfVertices + (start - 1)] = length;
    }
    return EXIT_SUCCESS;
}

int Prims(const Graph_t* graph, Queue_t* queue, MST_t* minSpanningTree) {
    queue->distance[0] = 0;
    int closestVertex = ExtractMin(queue);
    while (queue->queueSize) {
        for (int dest = 0; dest < graph->numOfVertices; ++dest) {
            if (IsAdjacent(graph, closestVertex, dest) && BelongsToQueue(queue, dest)) {
                unsigned edgeLen = graph->adjacencyMatrix[closestVertex * graph->numOfVertices + dest];
                if (edgeLen < queue->distance[dest]) {
                    queue->distance[dest] = edgeLen;
                    queue->parent[dest] = closestVertex;
                }
            }
        }
        closestVertex = ExtractMin(queue);
        if (closestVertex == NO_MINIMAL) {
            return NO_SPANNING_TREE;
        }
        AddToMST(minSpanningTree, queue->parent[closestVertex], closestVertex);
    }
    return EXIT_SUCCESS;
}
