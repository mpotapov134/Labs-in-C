#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define MAX_NUMBER 5000

enum ExitCodes {
    OUT_OF_MEMORY = 101,
};

typedef struct DisjointSetUnion { // Объединение непересекающихся множеств
    int parent[MAX_NUMBER]; // Родитель i-ой вершины, т.е. представитель множества, которому принадлежит i
    int size[MAX_NUMBER]; // Вес i-ой вершины, т.е. количество вершин, для которой i — родитель
} DSU_t;

typedef struct Edge {
    int start;
    int end;
    int length;
} Edge_t;

typedef struct MinimalSpanningTree { // Минимальное остовное дерево
    Edge_t edges[MAX_NUMBER - 1]; // Ребер на 1 меньше, чем вершин
    int shift; // Сдвиг относительно начала массива
} MST_t;


void InitializeDSU(DSU_t* dsu, int number) {
    for (int i = 0; i < number; ++i) {
        dsu->parent[i] = i; // Изначально все вершины лежат в различных множетсвах (компонентах связности)
        dsu->size[i] = 1;
    }
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
    if (dsu->size[a] > dsu->size[b]) { // Меняем местами, чтобы множество b было больше
        int tmp = a;
        a = b;
        b = tmp;
    }
    // printf("%i %i\n", a, b);
    dsu->size[b] += dsu->size[a]; // Вес объединения равен сумме весов
    dsu->parent[a] = b; // "Подвесили" a к b
}


/* Параметр сортировки */
int Compare(const void* a, const void* b) {
    Edge_t arg1 = *(Edge_t*) a;
    Edge_t arg2 = *(Edge_t*) b;
    if (arg1.length < arg2.length) {
        return -1;
    }
    if (arg1.length == arg2.length) {
        return 0;
    }
    return 1;
}

int AddsLoop(DSU_t* dsu, int start, int end) {
    // Если начало и конец ребра лежат в одной компоненте связности, образуется цикл
    return (Leader(dsu, start) == Leader(dsu, end)) ? 1 : 0;
}

void AddToMST(MST_t* result, Edge_t edge) {
    result->edges[result->shift] = edge;
    result->shift++;
}


/* Читает входные данные и выполняет проверку */
int ReadSize(int* numOfVertices, int* numOfEdges) {
    if (scanf("%i", numOfVertices) != 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if (*numOfVertices < 0 || *numOfVertices > MAX_NUMBER) {
        printf("bad number of vertices\n");
        return 0;
    }

    if (scanf("%i", numOfEdges) != 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if (*numOfEdges < 0 || *numOfEdges > *numOfVertices * (*numOfVertices - 1) / 2) {
        printf("bad number of edges\n");
        return 0;
    }
    return 1;
}

/* Читает данные о ребрах и записывает ребра в список */
int ReadEdges(Edge_t* edges, int numOfVertices, int numOfEdges) {
    for (int i = 0; i < numOfEdges; ++i) {
        long long int length;
        if (scanf("%i %i %lli", &edges[i].start, &edges[i].end, &length) != 3) {
            printf("bad number of lines\n");
            return 0;
        }

        if (edges[i].start < 1 || edges[i].start > numOfVertices || edges[i].end < 1 || edges[i].end > numOfVertices) {
            printf("bad vertex\n");
            return 0;
        }
        if (length < 0 || length > INT_MAX) {
            printf("bad length\n");
            return 0;
        }
        edges[i].length = length;
    }
    return 1;
}

void FindMST(Edge_t* edges, DSU_t* dsu, MST_t* result, int numOfVertices, int numOfEdges) {
    int edgeIndex = 0;
    for (int i = 0; i < numOfVertices - 1; ++i) {
        while (edgeIndex < numOfEdges && AddsLoop(dsu, edges[edgeIndex].start - 1, edges[edgeIndex].end - 1)) {
            edgeIndex++;
        }
        if (edgeIndex >= numOfEdges) {
            return;
        }
        AddToMST(result, edges[edgeIndex]);
        Unite(dsu, edges[edgeIndex].start - 1, edges[edgeIndex].end - 1);
        edgeIndex++;
    }
}

int main(void) {
    int numOfVertices, numOfEdges;
    if (!ReadSize(&numOfVertices, &numOfEdges)) {
        return EXIT_SUCCESS;
    }

    Edge_t* edges = calloc(numOfEdges, sizeof(Edge_t));
    if (!edges) {
        return OUT_OF_MEMORY;
    }

    if (!ReadEdges(edges, numOfVertices, numOfEdges)) {
        free(edges);
        return EXIT_SUCCESS;
    }
    qsort(edges, numOfEdges, sizeof(Edge_t), Compare);

    DSU_t verticesDSU;
    InitializeDSU(&verticesDSU, numOfVertices);
    MST_t result;
    result.shift = 0;

    FindMST(edges, &verticesDSU, &result, numOfVertices, numOfEdges);
    if (result.shift != numOfVertices - 1) {
        printf("no spanning tree\n");
        free(edges);
        return EXIT_SUCCESS;
    }

    for (int i = 0; i < result.shift; ++i) {
        printf("%i %i\n", result.edges[i].start, result.edges[i].end);
    }

    free(edges);
}
