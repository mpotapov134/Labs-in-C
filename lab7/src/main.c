#include <stdlib.h>
#include <stdio.h>

#define MAX_NUMBER 2000

enum ExitCodes {
    SUCCESS = 100,
    BAD_NUMBER_OF_LINES = 101,
    BAD_NUMBER_OF_Vertices,
    BAD_NUMBER_OF_EDGES,
    BAD_VERTEX,
    IMPOSSIBLE_TO_SORT,
    OUT_OF_MEMORY,
};

/* Структура односвязного списка вершин, в которые есть ребро из данной вершины */
typedef struct VertexList {
    int vertex;
    struct VertexList* next;
} VertexList_t;

/* Структура вершины графа */
typedef struct Vertex {
    int vertex;
    VertexList_t* listOfAdjacent;
} Vertex_t;

typedef struct LinearAllocator {
    void *basePointer;
    size_t offset;
} LinearAllocator_t;

static int AllocInit(LinearAllocator_t* allocator, size_t size) {
    allocator->basePointer = malloc(size);
    if (!allocator->basePointer) {
        return OUT_OF_MEMORY;
    }
    allocator->offset = 0;
    return SUCCESS;
}

static void* Allocate(LinearAllocator_t* allocator, size_t reqSize) {
    void* pointerToAllocated = allocator->basePointer + allocator->offset;
    allocator->offset += reqSize;
    return pointerToAllocated;
}

static void FreeAll(LinearAllocator_t* allocator) {
    free(allocator->basePointer);
}

/* Читает входные данные и создает граф */
int Initialize(int* numOfVertices, int* numOfEdges, Vertex_t graph[MAX_NUMBER]) {
    int scanReturn = scanf("%i", numOfVertices);
    if (scanReturn != 1) {
        printf("bad number of lines\n");
        return BAD_NUMBER_OF_LINES;
    }
    if (*numOfVertices < 0 || *numOfVertices > MAX_NUMBER) {
        printf("bad number of vertices\n");
        return BAD_NUMBER_OF_Vertices;
    }

    scanReturn = scanf("%i", numOfEdges);
    if (scanReturn != 1) {
        printf("bad number of lines\n");
        return BAD_NUMBER_OF_LINES;
    }
    if (*numOfEdges < 0 || *numOfEdges > *numOfVertices * (*numOfVertices + 1) / 2) {
        printf("bad number of edges\n");
        return BAD_NUMBER_OF_EDGES;
    }

    for (int i = 0; i < *numOfVertices; ++i) {
        graph[i].vertex = i + 1;
        graph[i].listOfAdjacent = NULL;
    }
    return SUCCESS;
}

/* Читает данные о ребрах графа и заполняет списки смежности для вершин графа */
int CreateEdges(int numOfVertices, int numOfEdges, Vertex_t graph[MAX_NUMBER], LinearAllocator_t* allocator) {
    for (int edge = 0; edge < numOfEdges; ++edge) {
        int start, end;
        int scanReturn = scanf("%i%i", &start, &end);
        if (scanReturn != 2) {
            printf("bad number of lines\n");
            return BAD_NUMBER_OF_LINES;
        }

        if (start < 1 || start > numOfVertices || end < 1 || end > numOfVertices) {
            printf("bad vertex\n");
            return BAD_VERTEX;
        }

        /* Создаем новый элемент в списке связанных вершин */
        VertexList_t* newAdjacent = Allocate(allocator, sizeof(*newAdjacent));
        newAdjacent->vertex = end;
        newAdjacent->next = graph[start - 1].listOfAdjacent;
        graph[start - 1].listOfAdjacent = newAdjacent;
    }
    return SUCCESS;
}

/* Выполняет топологическую сортировку */
int Sort(int numOfVertices, Vertex_t graph[MAX_NUMBER], int result[MAX_NUMBER]) {
    char state[MAX_NUMBER]; // Состояния вершин графа
    for (int i = 0; i < numOfVertices; ++i) {
        state[i] = 'N';
    }

    for (int i = 0; i < numOfVertices; ++i) {
        /* Восстанавливаем ранее вычеркнутые вершины */
        for (int vertex = 0; vertex < numOfVertices; ++vertex) {
            if (state[vertex] == 'S') {
                state[vertex] = 'N';
            }
        }

        /* Вычеркиваем все вершины, в которые есть дуга */
        for (int vertexIndex = 0; vertexIndex < numOfVertices; ++vertexIndex) {
            VertexList_t* currentAdjacent = graph[vertexIndex].listOfAdjacent;
            while(currentAdjacent) {
                if (state[currentAdjacent->vertex - 1] == 'N') {
                    state[currentAdjacent->vertex - 1] = 'S';
                }
                currentAdjacent = currentAdjacent->next;
            }
        }

        char found = 0; // Флаг, по которому будем определять, нашлась ли не вычекнутая вершина
        /* Ищем первую не вычеркнутую вершину, записываем ее в ответ и удаляем все исходящие из нее дуги */
        for (int vertex = 0; vertex < numOfVertices; ++vertex) {
            if (state[vertex] == 'N') {
                found = 1;
                result[i] = vertex + 1;
                state[vertex] = 'D';
                graph[vertex].listOfAdjacent = NULL;
                break;
            }
        }

        /* Не вычеркнутая вершина не нашлась */
        if (!found) {
            printf("impossible to sort\n");
            return IMPOSSIBLE_TO_SORT;
        }
    }
    return SUCCESS;
}

int main(void) {
    int numOfVertices, numOfEdges;
    struct Vertex graph[MAX_NUMBER];
    int result[MAX_NUMBER];

    int returnCode = Initialize(&numOfVertices, &numOfEdges, graph);
    if (returnCode != SUCCESS) {
        exit(0);
    }

    LinearAllocator_t allocator;
    returnCode = AllocInit(&allocator, sizeof(VertexList_t) * numOfEdges);
    if (returnCode != SUCCESS) {
        exit(0);
    }

    returnCode = CreateEdges(numOfVertices, numOfEdges, graph, &allocator);
    if (returnCode != SUCCESS) {
        FreeAll(&allocator);
        exit(0);
    }

    returnCode = Sort(numOfVertices, graph, result);
    if (returnCode != SUCCESS) {
        FreeAll(&allocator);
        exit(0);
    }

    for (int i = 0; i < numOfVertices; ++i) {
        printf("%i ", result[i]);
    }
    printf("\n");
    FreeAll(&allocator);
}
