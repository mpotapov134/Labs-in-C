#include <stdlib.h>
#include "queue.h"

void FreeQueue(Queue_t* queue) {
    free(queue->distance);
    free(queue->parent);
    queue->distance = NULL;
    queue->parent = NULL;
}

Queue_t* InitializeQueue(Queue_t* queue, int numOfVertices) {
    queue->queueSize = numOfVertices;
    queue->bufferSize = numOfVertices;
    queue->distance = malloc(numOfVertices * sizeof(*queue->distance));
    queue->parent = calloc(numOfVertices, sizeof(*queue->parent));
    if (!queue->distance || !queue->parent) {
        FreeQueue(queue);
        return NULL;
    }
    for (int i = 0; i < numOfVertices; ++i) {
        queue->distance[i] = INFINITY;
    }
    return queue;
}

int ExtractMin(Queue_t* queue) {
    unsigned int minDistance = INFINITY;
    int indexOfMin;
    for (int i = 0; i < queue->bufferSize; ++i) {
        unsigned int distance = queue->distance[i];
        if (distance == (unsigned) NOT_IN_QUEUE) {
            continue;
        }
        if (distance < minDistance) {
            minDistance = distance;
            indexOfMin = i;
        }
    }
    if (minDistance == (unsigned) INFINITY) {
        return NO_MINIMAL;
    }
    queue->distance[indexOfMin] = NOT_IN_QUEUE;
    queue->queueSize--;
    return indexOfMin;
}
