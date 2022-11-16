#include <stdlib.h>
#include <stdio.h>


void Swap(int *src, int *dst) {
    int temp = *src;
    *src = *dst;
    *dst = temp;
}


int MakePartition(int *array, int len) {
    int baseElement = array[(len - 1) / 2];
    int left = 0, right = len - 1;
    while (1) {
        while (array[left] < baseElement) {
            left ++;
        }
        while (baseElement < array[right]) {
            right --;
        }
        if (left >= right) {
            return right;
        }
        Swap(&array[left ++], &array[right --]);
    }
}


void QuickSort(int *array, int len) {
    if (len <= 1) {
        return;
    }
    int separationPoint = MakePartition(array, len);
    QuickSort(array, separationPoint + 1);
    QuickSort(array + separationPoint + 1, len - separationPoint - 1);
}


void PrintArray(int *array, int arrayLen) {
    for (int i = 0; i < arrayLen; ++i) {
        printf("%i ", array[i]);
    }
    printf("\n");
}


int main(void) {
    unsigned arrayLen = 0;
    if (! scanf("%u", &arrayLen)) {
        exit(0);
    }

    int *array = (int *) malloc(arrayLen * sizeof(int));
    if (! array) {
        exit(0);
    }

    for (unsigned i = 0; i < arrayLen; ++i) {
        if (! scanf("%i", &array[i])) {
            exit(0);
        }
    }

    QuickSort(array, arrayLen);

    PrintArray(array, arrayLen);

    free(array);

    return EXIT_SUCCESS;
}
