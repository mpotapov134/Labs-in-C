#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "dsu.h"
#include "mst.h"

#define MAX_NUMBER 5000

enum ExitCodes {
    OUT_OF_MEMORY = 1,
    BAD_NUMBER_OF_LINES,
    BAD_NUMBER_OF_VERTICES,
    BAD_NUMBER_OF_EDGES,
    BAD_VERTEX,
    BAD_LENGTH,
};

/* Параметр сортировки */
static int Compare(const void* a, const void* b) {
    Edge_t edge1 = *(Edge_t*) a;
    Edge_t edge2 = *(Edge_t*) b;
    if (edge1.length < edge2.length) {
        return -1;
    }
    if (edge1.length == edge2.length) {
        return 0;
    }
    return 1;
}

static void FreeAll(Edge_t* edges, DSU_t* dsu, MST_t* minSpanningTree) {
    free(edges);
    FreeDSU(dsu);
    FreeMST(minSpanningTree);
}

static int AddsLoop(DSU_t* dsu, int start, int end) {
    // Если начало и конец ребра лежат в одной компоненте связности, образуется цикл
    return (Leader(dsu, start) == Leader(dsu, end)) ? 1 : 0;
}

static int ReadSize(int* numOfVertices, int* numOfEdges) {
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

static int ReadEdges(Edge_t* edges, int numOfVertices, int numOfEdges) {
    for (int i = 0; i < numOfEdges; ++i) {
        long long int length;
        if (scanf("%i %i %lli", &edges[i].start, &edges[i].end, &length) != 3) {
            printf("bad number of lines\n");
            return BAD_NUMBER_OF_LINES;
        }

        if (edges[i].start < 1 || edges[i].start > numOfVertices || edges[i].end < 1 || edges[i].end > numOfVertices) {
            printf("bad vertex\n");
            return BAD_VERTEX;
        }
        if (length < 0 || length > INT_MAX) {
            printf("bad length\n");
            return BAD_LENGTH;
        }
        edges[i].length = length;
    }
    return EXIT_SUCCESS;
}

void FindMST(const Edge_t* edges, DSU_t* dsu, MST_t* minSpanningTree, int numOfVertices, int numOfEdges) {
    int edgeIndex = 0;
    for (int i = 0; i < numOfVertices - 1; ++i) {
        /* Если при добавлении ребра возникает цикл, такое добавление недопустимо */
        while (edgeIndex < numOfEdges && AddsLoop(dsu, edges[edgeIndex].start - 1, edges[edgeIndex].end - 1)) {
            edgeIndex++;
        }
        if (edgeIndex >= numOfEdges) { // Уже перебрали все ребра
            return;
        }
        AddToMST(minSpanningTree, edges[edgeIndex]);
        Unite(dsu, edges[edgeIndex].start - 1, edges[edgeIndex].end - 1);
        edgeIndex++;
    }
}

static void PrintMST(const MST_t* minSpanningTree) {
    for (int i = 0; i < minSpanningTree->bufferSize; ++i) {
        printf("%i %i\n", minSpanningTree->edges[i].start, minSpanningTree->edges[i].end);
    }
}

int main(void) {
    int numOfVertices, numOfEdges;
    if (ReadSize(&numOfVertices, &numOfEdges) != EXIT_SUCCESS) {
        return EXIT_SUCCESS;
    }
    if (numOfVertices == 0) {
        printf("no spanning tree\n");
        return EXIT_SUCCESS;
    }

    Edge_t* edges = calloc(numOfEdges, sizeof(Edge_t));
    DSU_t verticesDSU;
    MST_t minSpanningTree;

    if (!edges || !InitializeDSU(&verticesDSU, numOfVertices) ||
            !InitializeMST(&minSpanningTree, numOfVertices)) {
        FreeAll(edges, &verticesDSU, &minSpanningTree);
        return OUT_OF_MEMORY;
    }

    if (ReadEdges(edges, numOfVertices, numOfEdges) != EXIT_SUCCESS) {
        FreeAll(edges, &verticesDSU, &minSpanningTree);
        return EXIT_SUCCESS;
    }
    qsort(edges, numOfEdges, sizeof(Edge_t), Compare);

    FindMST(edges, &verticesDSU, &minSpanningTree, numOfVertices, numOfEdges);

    if (minSpanningTree.bufferSize < numOfVertices - 1) { // Получили меньше ребер, чем необходимо => каркаса нет
        printf("no spanning tree\n");
        FreeAll(edges, &verticesDSU, &minSpanningTree);
        return EXIT_SUCCESS;
    }

    PrintMST(&minSpanningTree);
    FreeAll(edges, &verticesDSU, &minSpanningTree);
}
