#ifndef MST_H_INCLUDED
#define MST_H_INCLUDED

typedef struct Edge { // Структура ребра графа
    int start;
    int end;
    int length;
} Edge_t;

typedef struct MinimalSpanningTree { // Минимальное остовное дерево
    Edge_t* edges; // Список ребер, вошедших в каркас
    int bufferSize; // Количество записанных ребер
} MST_t;

void FreeMST(MST_t* mst);

MST_t* InitializeMST(MST_t* mst, int numOfVertices);

void AddToMST(MST_t* minSpanningTree, Edge_t edge);

#endif
