#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <limits.h>
#include <stdint.h>

#define INFINITY UINT_MAX
#define NO_MIN -1

enum VertexState {
    VISITED = 1,
    NOT_VISITED,
};

typedef struct VerticesQueue {
    uint64_t* distance; // Минимальное расстояние от этой вершины до старта
    char* state; // Состояние вершины: уже посещена или еще нет
    int bufferSize;
    int queueSize;
} Vertices_t;

void FreeVertices(Vertices_t* vertices);

Vertices_t* InitializeVertices(Vertices_t* vertices, int numOfVertices);

int ExtractMin(Vertices_t* vertices);

int QueueIsEmpty(Vertices_t* vertices);

#endif
