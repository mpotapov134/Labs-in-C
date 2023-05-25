#ifndef DIJKSTRA_H_INCLUDED
#define DIJKSTRA_H_INCLUDED

#include "graph.h"
#include "vertex.h"

#define MAX_NUMBER 5000

enum DijkstraExitCodes {
    BAD_NUMBER_OF_LINES = 1,
    BAD_NUMBER_OF_VERTICES,
    BAD_NUMBER_OF_EDGES,
    BAD_VERTEX,
    BAD_LENGTH,
};

void FreeAll(Graph_t* graph, Vertices_t* vertices, int* path);

int ReadData(int* numOfVertices, int* start, int* finish, int* numOfEdges);

int ReadEdges(Graph_t* graph, int numOfEdges);

void Dijkstras(const Graph_t* graph, Vertices_t* vertices, int* path, int start, int finish, int* hasOverflow);

void PrintOutput(const Vertices_t* vertices, const int* path, int start, int finish, int hasOverflow);

#endif
