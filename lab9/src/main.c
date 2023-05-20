#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "graph.h"
#include "vertex.h"

#define MAX_NUMBER 5000

enum ExitCodes {
    OUT_OF_MEMORY = 1,
    BAD_NUMBER_OF_LINES,
    BAD_NUMBER_OF_VERTICES,
    BAD_NUMBER_OF_EDGES,
    BAD_VERTEX,
    BAD_LENGTH,
};

static void FreeAll(Graph_t* graph, Vertices_t* vertices, int* path) {
    FreeGraph(graph);
    FreeVertices(vertices);
    free(path);
}

static int ReadData(int* numOfVertices, int* start, int* finish, int* numOfEdges) {
    if (scanf("%i %i %i %i", numOfVertices, start, finish, numOfEdges) != 4) {
        printf("bad number of lines\n");
        return BAD_NUMBER_OF_LINES;
    }
    if (*numOfVertices < 0 || *numOfVertices > MAX_NUMBER) {
        printf("bad number of vertices\n");
        return BAD_NUMBER_OF_VERTICES;
    }
    if (*numOfEdges < 0 || *numOfEdges > *numOfVertices * (*numOfVertices + 1) / 2) {
        printf("bad number of edges\n");
        return BAD_NUMBER_OF_EDGES;
    }
    if (*start < 1 || *finish < 1 || *start > *numOfVertices || *finish > *numOfVertices) {
        printf("bad vertex\n");
        return BAD_VERTEX;
    }
    return EXIT_SUCCESS;
}

static int ReadEdges(Graph_t* graph, int numOfEdges) {
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
        graph->adjacencyMatrix[(start - 1) * graph->numOfVertices + (end - 1)] = length;
        graph->adjacencyMatrix[(end - 1) * graph->numOfVertices + (start - 1)] = length;
    }
    return EXIT_SUCCESS;
}

static int IsAdjacent(const Graph_t* graph, int edgeStart, int edgeEnd) {
    int edgeLen = graph->adjacencyMatrix[edgeStart * graph->numOfVertices + edgeEnd];
    return edgeLen == NO_EDGE ? 0 : 1;
}

void Dijkstras(const Graph_t* graph, Vertices_t* vertices, int* path, int start, int finish, int* hasOverflow) {
    vertices->distance[start - 1] = 0;
    path[start - 1] = start - 1;
    int minVertex = ExtractMin(vertices);
    while (!QueueIsEmpty(vertices)) {
        for (int edgeEnd = 0; edgeEnd < graph->numOfVertices; ++edgeEnd) {
            if (!IsAdjacent(graph, minVertex, edgeEnd)) {
                continue;
            }

            int edgeLen = graph->adjacencyMatrix[minVertex * graph->numOfVertices + edgeEnd];
            unsigned long long newDistance = vertices->distance[minVertex] + edgeLen;
            if (edgeEnd == finish - 1 && vertices->distance[edgeEnd] != (unsigned) INFINITY &&
                    vertices->distance[edgeEnd] > INT_MAX && newDistance > INT_MAX) {
                *hasOverflow = 1;
            }

            if (newDistance < vertices->distance[edgeEnd]) {
                vertices->distance[edgeEnd] = newDistance;
                path[edgeEnd] = minVertex;
            }
        }
        vertices->state[minVertex] = VISITED;
        vertices->queueSize--;
        minVertex = ExtractMin(vertices);
        if (minVertex == NO_MIN) {
            return;
        }
    }
}

static void PrintOutput(const Vertices_t* vertices, const int* path, int start, int finish, int hasOverflow) {
    for (int i = 0; i < vertices->bufferSize; ++i) {
        unsigned long long distance = vertices->distance[i];
        if (distance == (unsigned) INFINITY) {
            printf("oo ");
        }
        else if (distance > INT_MAX) {
            printf("INT_MAX+ ");
        }
        else {
            printf("%llu ", distance);
        }
    }
    printf("\n");

    unsigned long long distanceToFinish = vertices->distance[finish - 1];
    if (distanceToFinish == (unsigned) INFINITY) {
        printf("no path\n");
    }
    else if (distanceToFinish > INT_MAX && hasOverflow) {
        printf("overflow\n");
    }
    else {
        printf("%i ", finish);
        int previous = path[finish - 1];
        while (previous != start - 1) {
            printf("%i ", previous + 1);
            previous = path[previous];
        }
        printf("%i\n", start);
    }
}

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
