#include <stdlib.h>
#include "vertex.h"

void FreeVertices(Vertices_t* vertices) {
    free(vertices->distance);
    free(vertices->state);
    vertices->distance = NULL;
    vertices->state = NULL;
}

Vertices_t* InitializeVertices(Vertices_t* vertices, int numOfVertices) {
    vertices->bufferSize = numOfVertices;
    vertices->queueSize = numOfVertices;
    vertices->distance = malloc(numOfVertices * sizeof(*vertices->distance));
    vertices->state = malloc(numOfVertices * sizeof(*vertices->state));
    if (!vertices->distance || !vertices->state) {
        FreeVertices(vertices);
        return NULL;
    }
    for (int i = 0; i < numOfVertices; ++i) {
        vertices->distance[i] = (unsigned) INFINITY;
        vertices->state[i] = NOT_VISITED;
    }
    return vertices;
}

int ExtractMin(Vertices_t* vertices) {
    unsigned long long minDistance = INFINITY;
    int minVertex = 0;
    for (int i = 0; i < vertices->bufferSize; ++i) {
        if (vertices->state[i] == NOT_VISITED && vertices->distance[i] < minDistance) {
            minDistance = vertices->distance[i];
            minVertex = i;
        }
    }
    if (minDistance == (unsigned) INFINITY) {
        return NO_MIN;
    }
    return minVertex;
}

int QueueIsEmpty(Vertices_t* vertices) {
    return vertices->queueSize == 0 ? 1 : 0;
}
