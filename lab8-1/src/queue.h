#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <limits.h>

#define INFINITY UINT_MAX
#define NOT_IN_QUEUE (UINT_MAX - 1)
#define NO_MINIMAL -1

typedef struct Queue {
    unsigned int* distance;
    int* parent;
    int queueSize;
    int bufferSize;
} Queue_t;

void FreeQueue(Queue_t* queue);

Queue_t* InitializeQueue(Queue_t* queue, int numOfVertices);

int ExtractMin(Queue_t* queue);

int BelongsToQueue(const Queue_t* queue, int vertex);

#endif
