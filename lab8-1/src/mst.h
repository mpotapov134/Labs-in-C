#ifndef MST_H_INCLUDED
#define MST_H_INCLUDED

typedef struct Vertex {
    int parent;
    int vertex;
} Vertex_t;

typedef struct MinimalSpanningTree {
    Vertex_t* vertices;
    int mstSize;
} MST_t;

void FreeMST(MST_t* minSpanningTree);

MST_t* InitializeMST(MST_t* minSpanningTree, int numOfVertices);

void AddToMST(MST_t* minSpanningTree, int parent, int vertex);

void PrintMST(const MST_t* minSpanningTree);

#endif
