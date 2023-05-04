#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define MAX_NUMBER 5000

enum ExitCodes {
    OUT_OF_MEMORY = 1,
    BAD_NUMBER_OF_LINES,
    BAD_NUMBER_OF_VERTICES,
    BAD_NUMBER_OF_EDGES,
    BAD_VERTEX,
    BAD_LENGTH,
};

typedef struct DisjointSetUnion { // Объединение непересекающихся множеств
    int* parent; // Родитель i-ой вершины, т.е. вершина, к которой "подвешена" данная
    int* weight; // Вес i-ой вершины, т.е. количество вершин, для которой i — родитель (в т.ч. i — родитель для себя)
} DSU_t;

typedef struct Edge { // Структура ребра графа
    int start;
    int end;
    int length;
} Edge_t;

typedef struct MinimalSpanningTree { // Минимальное остовное дерево
    Edge_t* edges; // Список ребер, вошедших в каркас
    int bufferSize; // Количество записанных ребер
} MST_t;


DSU_t* InitializeDSU(DSU_t* dsu, int number) {
    dsu->parent = calloc(number, sizeof(*dsu->parent));
    dsu->weight = calloc(number, sizeof(*dsu->weight));
    if (!dsu->parent || !dsu->weight) {
        free(dsu->parent);
        free(dsu->weight);
        return NULL;
    }
    for (int i = 0; i < number; ++i) {
        dsu->parent[i] = i; // Изначально все вершины лежат в различных множетсвах (компонентах связности)
        dsu->weight[i] = 1; // Поэтому вес каждой вершины равен 1
    }
    return dsu;
}

void FreeDSU(DSU_t* dsu) {
    free(dsu->parent);
    free(dsu->weight);
}

MST_t* InitializeMST(MST_t* mst, int numOfVertices) {
    mst->bufferSize = 0;
    mst->edges = calloc(numOfVertices - 1, sizeof(*mst->edges)); // Ребер в каркасе на 1 меньше, чем вершин
    if (!mst->edges) {
        return NULL;
    }
    return mst;
}

void FreeMST(MST_t* mst) {
    free(mst->edges);
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


/* Параметр сортировки */
int Compare(const void* a, const void* b) {
    Edge_t edge1 = *(Edge_t*) a;
    Edge_t edge2 = *(Edge_t*) b;
    if (edge1.length < edge2.length) {
        return -1;
    }
    if (edge1.length == edge2.length) {
        return 0;
    }
    return 1;
}

int AddsLoop(DSU_t* dsu, int start, int end) {
    // Если начало и конец ребра лежат в одной компоненте связности, образуется цикл
    return (Leader(dsu, start) == Leader(dsu, end)) ? 1 : 0;
}

void AddToMST(MST_t* minSpanningTree, Edge_t edge) {
    minSpanningTree->edges[minSpanningTree->bufferSize] = edge;
    minSpanningTree->bufferSize++;
}


/* Читает входные данные и выполняет проверку */
int ReadSize(int* numOfVertices, int* numOfEdges) {
    if (scanf("%i %i", numOfVertices, numOfEdges) != 2) {
        printf("bad number of lines\n");
        return BAD_NUMBER_OF_LINES;
    }
    if (*numOfVertices < 0 || *numOfVertices > MAX_NUMBER) {
        printf("bad number of vertices\n");
        return BAD_NUMBER_OF_VERTICES;
    }
    if (*numOfEdges < 0 || *numOfEdges > *numOfVertices * (*numOfVertices - 1) / 2) {
        printf("bad number of edges\n");
        return BAD_NUMBER_OF_EDGES;
    }
    return EXIT_SUCCESS;
}

/* Читает данные о ребрах и записывает ребра в список */
int ReadEdges(Edge_t* edges, int numOfVertices, int numOfEdges) {
    for (int i = 0; i < numOfEdges; ++i) {
        long long int length;
        if (scanf("%i %i %lli", &edges[i].start, &edges[i].end, &length) != 3) {
            printf("bad number of lines\n");
            return BAD_NUMBER_OF_LINES;
        }

        if (edges[i].start < 1 || edges[i].start > numOfVertices || edges[i].end < 1 || edges[i].end > numOfVertices) {
            printf("bad vertex\n");
            return BAD_VERTEX;
        }
        if (length < 0 || length > INT_MAX) {
            printf("bad length\n");
            return BAD_LENGTH;
        }
        edges[i].length = length;
    }
    return EXIT_SUCCESS;
}

void FindMST(Edge_t* edges, DSU_t* dsu, MST_t* minSpanningTree, int numOfVertices, int numOfEdges) {
    int edgeIndex = 0;
    for (int i = 0; i < numOfVertices - 1; ++i) {
        /* Если при добавлении ребра возникает цикл, такое добавление недопустимо */
        while (edgeIndex < numOfEdges && AddsLoop(dsu, edges[edgeIndex].start - 1, edges[edgeIndex].end - 1)) {
            edgeIndex++;
        }
        if (edgeIndex >= numOfEdges) { // Уже перебрали все ребра
            return;
        }
        AddToMST(minSpanningTree, edges[edgeIndex]);
        Unite(dsu, edges[edgeIndex].start - 1, edges[edgeIndex].end - 1);
        edgeIndex++;
    }
}

int main(void) {
    int numOfVertices, numOfEdges;
    if (ReadSize(&numOfVertices, &numOfEdges) != EXIT_SUCCESS) {
        return EXIT_SUCCESS;
    }
    if (numOfVertices == 0) {
        printf("no spanning tree\n");
        return EXIT_SUCCESS;
    }

    Edge_t* edges = calloc(numOfEdges, sizeof(Edge_t));
    if (!edges) {
        return OUT_OF_MEMORY;
    }

    if (ReadEdges(edges, numOfVertices, numOfEdges) != EXIT_SUCCESS) {
        free(edges);
        return EXIT_SUCCESS;
    }
    qsort(edges, numOfEdges, sizeof(Edge_t), Compare);

    DSU_t verticesDSU;
    if (!InitializeDSU(&verticesDSU, numOfVertices)) {
        free(edges);
        return OUT_OF_MEMORY;
    }
    MST_t minSpanningTree;
    if (!InitializeMST(&minSpanningTree, numOfVertices)) {
        free(edges);
        FreeDSU(&verticesDSU);
        return EXIT_SUCCESS;
    }

    FindMST(edges, &verticesDSU, &minSpanningTree, numOfVertices, numOfEdges);
    if (minSpanningTree.bufferSize < numOfVertices - 1) { // Получили меньше ребер, чем необходимо => каркаса нет
        printf("no spanning tree\n");
        free(edges);
        FreeDSU(&verticesDSU);
        FreeMST(&minSpanningTree);
        return EXIT_SUCCESS;
    }

    for (int i = 0; i < minSpanningTree.bufferSize; ++i) {
        printf("%i %i\n", minSpanningTree.edges[i].start, minSpanningTree.edges[i].end);
    }

    free(edges);
    FreeDSU(&verticesDSU);
    FreeMST(&minSpanningTree);
}
