#include <stdlib.h>
#include <stdio.h>


void Swap(int *src, int *dst) {
    int temp = *src;
    *src = *dst;
    *dst = temp;
}


int MakePartition(int *array, int left, int right) {
    int baseElement = array[(left + right) / 2];
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


void QuickSort(int *array, int left, int right) {
    if (right - left < 1) {
        return;
    }
    int separationPoint = MakePartition(array, left, right);
    QuickSort(array, left, separationPoint);
    QuickSort(array, separationPoint + 1, right);
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

    int *array = (int *) malloc(arrayLen * 4);
    if (! array) {
        exit(0);
    }

    for (unsigned i = 0; i < arrayLen; ++i) {
        if (! scanf("%i", &array[i])) {
            exit(0);
        }
    }

    QuickSort(array, 0, arrayLen - 1);

    PrintArray(array, arrayLen);

    free(array);

    return EXIT_SUCCESS;
}
