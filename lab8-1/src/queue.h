#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#define INFINITY -1
#define NOT_IN_QUEUE -2
#define NO_MINIMAL -3

typedef struct Queue {
    unsigned int* distance;
    int* parent;
    int queueSize;
    int bufferSize;
} Queue_t;

void FreeQueue(Queue_t* queue);

Queue_t* InitializeQueue(Queue_t* queue, int numOfVertices);

int ExtractMin(Queue_t* queue);

#endif
