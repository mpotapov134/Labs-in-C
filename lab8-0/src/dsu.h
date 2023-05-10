#ifndef DSU_H_INCLUDED
#define DSU_H_INCLUDED

typedef struct DisjointSetUnion { // Объединение непересекающихся множеств
    int* parent; // Родитель i-ой вершины, т.е. вершина, к которой "подвешена" данная
    int* weight; // Вес i-ой вершины, т.е. количество вершин, для которой i — родитель (в т.ч. i — родитель для себя)
} DSU_t;

void FreeDSU(DSU_t* dsu);

DSU_t* InitializeDSU(DSU_t* dsu, int number);

int Leader(DSU_t* dsu, int vertex);

void Unite(DSU_t* dsu, int a, int b);

#endif
