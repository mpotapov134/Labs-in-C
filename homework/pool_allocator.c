#include <stdlib.h>
#include <stdio.h>

// будем хранить указатели на свободные блоки памяти в других свободных блоках по принципу односвязного списка

#define MAX_SIZE 1024
#define BLOCK_SIZE sizeof(void*) // размер блоков должен быть соответствующим

enum ExitCodes {
    INITIALIZATION_ERROR = 101,
    FREE_ERROR,
    OUT_OF_MEMORY,
};

typedef struct PoolAllocator {
    void *start; // указатель на начало выделенной памяти
    size_t *listHead; // указатель на начало списка свободных блоков
    void *end; // указатель на последний блок в памяти
} PoolAlloc_t;

int Initialize(PoolAlloc_t *allocator) {
    if (!allocator) {
        return 0;
    }

    allocator->start = malloc(MAX_SIZE);
    if (!allocator->start) {
        return 0;
    }
    allocator->listHead = allocator->start; // изначально вершина в списке блоков совпадает с началом всего объема памяти

    int blockCount = MAX_SIZE / BLOCK_SIZE - 1;
    printf("%i\n", blockCount);
    for (int i = 0; i < blockCount; ++i) {
        size_t *currentBlockAddress = allocator->start + BLOCK_SIZE * i; // указатель на i-ый блок памяти
        // в i-ый блок записываем указатель на i+1-ый; т.о. currentBlockAddress — это указатель на указатель на i+1-ый блок размера size_t
        *((size_t **) currentBlockAddress) = currentBlockAddress + 1;
    }

    allocator->end = allocator->start + blockCount * BLOCK_SIZE; // записываем адрес последнего блока
    *((size_t **) (allocator->end)) = NULL; // в последний блок записываем NULL-указатель
    // в результате получаем односвязный список указателей на блоки памяти
    return 1;
}

void* PAlloc(PoolAlloc_t *allocator, size_t allocSize) {
    if (!allocator || !allocSize || allocSize > BLOCK_SIZE) { // нельзя выделить больше, чем BLOCK_SIZE
        return NULL;
    }

    if (!allocator->listHead) { // список свободных пустой, памяти нет
        return NULL;
    }

    void *allocatedBlock = allocator->listHead; // возвращать будем первый блок из свободных
    size_t *nextFreeBlock = *((size_t **) allocator->listHead); // по указателю listHead лежит указатель на след. свободный блок
    allocator->listHead = nextFreeBlock; // передвигаем голову списка на следующий элемент
    return allocatedBlock;
}

int PFree(PoolAlloc_t *allocator, void *pointer) {
    if (!allocator || !pointer) {
        return 0;
    }

    // проверяем, что указатель не выходит за пределы выделенного объема и кратен размеру блока
    if (pointer < allocator->start || allocator->end < pointer || (pointer - allocator->start) % BLOCK_SIZE != 0) {
        return 0;
    }

    // записываем в блок, который надо освободить, указатель на начало списка (на первый свободный блок),
    // т.е. вставляем осовбождаемый блок в начало списка
    *((size_t **) pointer) = allocator->listHead;
    // теперь освобожденный блок будет в начале списка свободных, поэтому
    // указатель на голову списка равен указателю на освобожденный блок
    allocator->listHead = pointer;
    return 1;
}

void Destruct(PoolAlloc_t *allocator) {
    free(allocator->start);
}

// Пример работы аллокатора
int main() {
    PoolAlloc_t allocator;
    if (!Initialize(&allocator)) {
        exit(INITIALIZATION_ERROR);
    }

    int *pointersList[130]; // будем хранить указатели на выделенные нам блоки памяти, чтобы освободить их впоследствии
    for (int i = 0; i < 130; ++i) {
        int *example = PAlloc(&allocator, sizeof(*example));
        if (!example) {
            printf("Нет свободной памяти!\n");
            continue;
        }

        pointersList[i] = example; // сохраняем указатель
        *example = i * i;
        printf("%i\n", *example);
    }

    for (int i = 0; i < 130; ++i) {
        if (!PFree(&allocator, pointersList[i])) {
            printf("Некорректный указатель\n");
            continue;
        }
    }
    printf("-----\n");

    int *example = PAlloc(&allocator, sizeof(*example));
    if (!PFree(&allocator, example + 1)) {
        printf("Некорректный указатель\n");
    }

    if (!PFree(&allocator, example)) {
        printf("Некорректный указатель\n");
    }
    else {
        printf("Память освобождена\n");
    }

    Destruct(&allocator);
}
