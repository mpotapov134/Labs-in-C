#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c) (MAX(MAX((a), (b)), (c)))


void Swap(int *val1, int *val2) {
    int temp = *val1;
    *val1 = *val2;
    *val2 = temp;
}


unsigned FindIndOfMax(int *array, unsigned rootInd, unsigned arrayLen) {
    int max = 0;
    int root = array[rootInd];
    int leftLeaf = array[rootInd * 2 + 1];
    if (rootInd * 2 + 2 >= arrayLen) { // in some cases there is no right leaf
        if (root >= leftLeaf) {
            return rootInd;
        }
        return rootInd * 2 + 1;
    }
    int rightLeaf = array[rootInd * 2 + 2];
    max = MAX3(root, leftLeaf, rightLeaf);
    if (root == max) {
        return rootInd;
    }
    if (leftLeaf == max) {
        return rootInd * 2 + 1;
    }
    return rootInd * 2 + 2;
}


void Heapify(int *array, unsigned rootInd, unsigned arrayLen) {
    if (rootInd * 2 + 1 >= arrayLen) { // current root has no leaves, therefore we should return
        return;
    }

    unsigned indexOfMax = FindIndOfMax(array, rootInd, arrayLen);
    if (indexOfMax == rootInd) { // root is the biggest, no need to swap
        return;
    }

    Swap(&array[rootInd], &array[indexOfMax]); // root is not the biggest, need to swap
    Heapify(array, indexOfMax, arrayLen); // and heapify the leaf that was changed
}


void MakeHeap(int *array, unsigned arrayLen) {
    unsigned startPos = arrayLen / 2 - 1;
    for (unsigned i = startPos; i + 1 > 0; --i) {
        Heapify(array, i, arrayLen);
    }
}


void HeapSort(int *array, unsigned arrayLen) {
    MakeHeap(array, arrayLen);
    while (arrayLen > 1) {
        Swap(&array[0], &array[arrayLen - 1]);
        Heapify(array, 0, --arrayLen);
    }
}


int main(void) {
    unsigned arrayLen = 0;
    if (! scanf("%i", &arrayLen)) {
        exit(0);
    }
    int *array = (int*) malloc(arrayLen * 4);
    if (! array) {
        exit(0);
    }
    for (unsigned i = 0; i < arrayLen; ++i) {
        scanf("%i", &array[i]);
    }

    HeapSort(array, arrayLen);

    for (unsigned i = 0; i < arrayLen; ++i) {
        printf("%i ", array[i]);
    }
    printf("\n");
    free(array);

    return EXIT_SUCCESS;
}
