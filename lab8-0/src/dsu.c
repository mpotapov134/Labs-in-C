#include <stdlib.h>
#include "dsu.h"

void FreeDSU(DSU_t* dsu) {
    free(dsu->parent);
    free(dsu->weight);
    dsu->parent = NULL;
    dsu->weight = NULL;
}

DSU_t* InitializeDSU(DSU_t* dsu, int number) {
    dsu->parent = calloc(number, sizeof(*dsu->parent));
    dsu->weight = calloc(number, sizeof(*dsu->weight));
    if (!dsu->parent || !dsu->weight) {
        FreeDSU(dsu);
        return NULL;
    }
    for (int i = 0; i < number; ++i) {
        dsu->parent[i] = i; // Изначально все вершины лежат в различных множетсвах (компонентах связности)
        dsu->weight[i] = 1; // Поэтому вес каждой вершины равен 1
    }
    return dsu;
}

/* Представитель множества, которому принадлежит vertex */
int Leader(DSU_t* dsu, int vertex) {
    int currentParent = dsu->parent[vertex];
    if (currentParent == vertex) {
        return vertex; // Элемент является родителем сам для себя => он же является представителем своего множества
    }
    /* Обновляем информацию о родителе, чтобы поиск был быстрее в дальнейшем */
    dsu->parent[vertex] = Leader(dsu, currentParent);
    return dsu->parent[vertex]; // В конце родитель элемента будет совпадать с его лидером
}

/* Объединение двух непересекающихся множеств */
void Unite(DSU_t* dsu, int a, int b) { // Подвешиваем к большему
    a = Leader(dsu, a); // Представляет множество, которому принадлежит a
    b = Leader(dsu, b); // Представляет множество, которому принадлежит b
    if (dsu->weight[a] > dsu->weight[b]) { // Меняем местами, чтобы множество b было больше
        int tmp = a;
        a = b;
        b = tmp;
    }
    dsu->weight[b] += dsu->weight[a]; // Вес объединения равен сумме весов
    dsu->parent[a] = b; // "Подвесили" a к b
}
