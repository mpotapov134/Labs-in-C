#include <stdlib.h>
#include <stdio.h>

#define MAX_NUMBER 2000
#define OUT_OF_MEMORY -1

typedef struct ResultStack {
    int buffer[MAX_NUMBER];
    int stackSize;
} Stack_t;

typedef struct Graph {
    char adjacencyMatrix[MAX_NUMBER][MAX_NUMBER];
    Stack_t* result;
} Graph_t;

void Push(Stack_t* stack, int vertex) {
    stack->buffer[stack->stackSize] = vertex;
    stack->stackSize++;
}

int Pop(Stack_t* stack) {
    int res = stack->buffer[stack->stackSize - 1];
    stack->stackSize--;
    return res;
}

Graph_t* InitializeGraph() {
    Graph_t* graph = calloc(1, sizeof(Graph_t));
    if (!graph) {
        return NULL;
    }
    graph->result = calloc(1, sizeof(Stack_t));
    if (!graph->result) {
        free(graph);
        return NULL;
    }
    return graph;
}

void FreeGraph(Graph_t* graph) {
    free(graph->result);
    free(graph);
}

/* Читает входные данные и выполняет проверку */
int Initialize(int* numOfVertices, int* numOfEdges) {
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
    if (*numOfEdges < 0 || *numOfEdges > *numOfVertices * (*numOfVertices + 1) / 2) {
        printf("bad number of edges\n");
        return 0;
    }
    return 1;
}

/* Читает данные о ребрах графа и заполняет матрицу смежности */
int CreateEdges(int numOfVertices, int numOfEdges, char adjacencyMatrix[][MAX_NUMBER]) {
    for (int edge = 0; edge < numOfEdges; ++edge) {
        int start, end;
        if (scanf("%i%i", &start, &end) != 2) {
            printf("bad number of lines\n");
            return 0;
        }

        if (start < 1 || start > numOfVertices || end < 1 || end > numOfVertices) {
            printf("bad vertex\n");
            return 0;
        }

        adjacencyMatrix[start - 1][end - 1] = 1;
    }
    return 1;
}

int DFS(Graph_t* graph, char state[], int numOfVertices, int start) {
    if (state[start] == 'D') { // dead-end
        return 1;
    }
    if (state[start] == 'M') { // visited before
        return 0; // impossible
    }
    state[start] = 'M'; // marked
    for (int end = 0; end < numOfVertices; ++end) {
        if (graph->adjacencyMatrix[start][end]) {
            if (!DFS(graph, state, numOfVertices, end)) {
                return 0; // impossible
            }
        }
    }
    state[start] = 'D'; // deleted
    Push(graph->result, start + 1);
    return 1;
}

int main() {
    int numOfVertices, numOfEdges;
    if (!Initialize(&numOfVertices, &numOfEdges)) {
        exit(EXIT_SUCCESS);
    }

    Graph_t* graph = InitializeGraph();
    if (!graph) {
        exit(OUT_OF_MEMORY);
    }

    if (!CreateEdges(numOfVertices, numOfEdges, graph->adjacencyMatrix)) {
        FreeGraph(graph);
        exit(EXIT_SUCCESS);
    }

    char state[MAX_NUMBER];
    for (int i = 0; i < numOfVertices; ++i) {
        state[i] = 'U'; // unmarked
    }

    for (int i = 0; i < numOfVertices; ++i) {
        if (state[i] == 'U') {
            if (!DFS(graph, state, numOfVertices, i)) {
                printf("impossible to sort\n");
                FreeGraph(graph);
                exit(EXIT_SUCCESS);
            }
        }
    }

    while (graph->result->stackSize > 0) {
        printf("%i ", Pop(graph->result));
    }
    printf("\n");

    FreeGraph(graph);
}
