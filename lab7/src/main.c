#include <stdlib.h>
#include <stdio.h>

#define MAX_NUMBER 2000

enum ReturnCodes {
    OUT_OF_MEMORY = 1,
    BAD_NUMBER_OF_LINES,
    BAD_NUMBER_OF_VERTICES,
    BAD_NUMBER_OF_EDGES,
    BAD_VERTEX,
    IMPOSSIBLE_TO_SORT,
};

enum VertexStates {
    UNMARKED = 1,
    MARKED,
    DELETED,
};

typedef struct Stack {
    int* buffer;
    int stackSize; // Индекс первой свободной ячейки;
} Stack_t;

typedef struct Graph {
    char* adjacencyMatrix;
    int numOfVertices;
} Graph_t;

void Push(Stack_t* stack, int vertex) {
    stack->buffer[stack->stackSize] = vertex;
    stack->stackSize++;
}

int Pop(Stack_t* stack) {
    int popped = stack->buffer[stack->stackSize - 1];
    stack->stackSize--;
    return popped;
}

Graph_t* InitializeGraph(Graph_t* graph, int numOfVertices) {
    graph->numOfVertices = numOfVertices;
    graph->adjacencyMatrix = calloc(numOfVertices * numOfVertices, sizeof(*graph->adjacencyMatrix));
    if (!graph->adjacencyMatrix) {
        return NULL;
    }
    return graph;
}

Stack_t* InitializeStack(Stack_t* stack, int bufferSize) {
    stack->stackSize = 0;
    stack->buffer = calloc(bufferSize, sizeof(*stack->buffer));
    if (!stack->buffer) {
        return NULL;
    }
    return stack;
}

void FreeGraph(Graph_t* graph) {
    free(graph->adjacencyMatrix);
}

void FreeStack(Stack_t* stack) {
    free(stack->buffer);
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
    if (*numOfEdges < 0 || *numOfEdges > *numOfVertices * (*numOfVertices + 1) / 2) {
        printf("bad number of edges\n");
        return BAD_NUMBER_OF_EDGES;
    }
    return EXIT_SUCCESS;
}

/* Читает данные о ребрах графа и заполняет матрицу смежности */
int CreateEdges(char* adjacencyMatrix, int numOfVertices, int numOfEdges) {
    for (int i = 0; i < numOfEdges; ++i) {
        int start, end;
        if (scanf("%i %i", &start, &end) != 2) {
            printf("bad number of lines\n");
            return BAD_NUMBER_OF_LINES;
        }

        if (start < 1 || start > numOfVertices || end < 1 || end > numOfVertices) {
            printf("bad vertex\n");
            return BAD_VERTEX;
        }

        adjacencyMatrix[(start - 1) * numOfVertices + (end - 1)] = 1;
    }
    return EXIT_SUCCESS;
}

int DepthFirstSearch(Graph_t* graph, char* state, int start, Stack_t* sortedVertices) {
    if (state[start] == DELETED) { // Данную вершину уже рассматривали
        return EXIT_SUCCESS;
    }
    if (state[start] == MARKED) { // Вершина уже помечена, значит, есть цикл
        return IMPOSSIBLE_TO_SORT;
    }
    state[start] = MARKED; // Помечаем вершину, в которой находимся
    int numOfVertices = graph->numOfVertices;
    for (int end = 0; end < numOfVertices; ++end) {
        /* Проходим по всем вершинам, в которые есть ребро из данной, и запускаем поиск из них */
        if (graph->adjacencyMatrix[start * numOfVertices + end]) {
            if (DepthFirstSearch(graph, state, end, sortedVertices) == IMPOSSIBLE_TO_SORT) {
                return IMPOSSIBLE_TO_SORT;
            }
        }
    }
    state[start] = DELETED; // Закончили рассмотрение данной вершины
    Push(sortedVertices, start + 1);
    return EXIT_SUCCESS;
}

int main() {
    int numOfVertices, numOfEdges;
    if (ReadSize(&numOfVertices, &numOfEdges) != EXIT_SUCCESS) {
        return EXIT_SUCCESS;
    }

    Graph_t graph;
    if (!InitializeGraph(&graph, numOfVertices)) {
        return OUT_OF_MEMORY;
    }

    if (CreateEdges(graph.adjacencyMatrix, numOfVertices, numOfEdges) != EXIT_SUCCESS) {
        FreeGraph(&graph);
        return EXIT_SUCCESS;
    }

    Stack_t sortedVertices;
    if (!InitializeStack(&sortedVertices, numOfVertices)) {
        FreeGraph(&graph);
        return OUT_OF_MEMORY;
    }

    char* state = calloc(numOfVertices, sizeof(char));
    if (!state) {
        FreeGraph(&graph);
        FreeStack(&sortedVertices);
        return OUT_OF_MEMORY;
    }
    for (int i = 0; i < numOfVertices; ++i) {
        state[i] = UNMARKED; // Изначально все вершины не помечены
    }

    for (int start = 0; start < numOfVertices; ++start) {
        if (state[start] == UNMARKED) {
            if (DepthFirstSearch(&graph, state, start, &sortedVertices) == IMPOSSIBLE_TO_SORT) {
                printf("impossible to sort\n");
                FreeGraph(&graph);
                FreeStack(&sortedVertices);
                free(state);
                return EXIT_SUCCESS;
            }
        }
    }

    while (sortedVertices.stackSize > 0) {
        printf("%i ", Pop(&sortedVertices));
    }
    printf("\n");

    FreeGraph(&graph);
    FreeStack(&sortedVertices);
    free(state);
}
