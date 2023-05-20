#include <stdlib.h>
#include <stdio.h>
#define MAX(a, b) ((a) >= (b)) ? (a) : (b)

enum ExitCodes {
    INPUT_ERROR = 101,
    OUT_OF_MEMORY,
};

void PrintItemsInfo(int totalSize, int count, int size, const int* weights, const int* prices, const int* costMatrix) {
    if (!costMatrix[count * (totalSize + 1) + size]) {
        return;
    }

    if (costMatrix[count * (totalSize + 1) + size] ==
        costMatrix[(count - 1) * (totalSize + 1) + size]) { // сумма осталась та же, значит, элемент с номером count не положили
        PrintItemsInfo(totalSize, count - 1, size, weights, prices, costMatrix);
    }
    else {
        PrintItemsInfo(totalSize, count - 1, size - weights[count - 1], weights, prices, costMatrix);
        printf("%i %i\n", weights[count - 1], prices[count - 1]);
    }
}

void CalcMaxCost(int totalNumber, int totalSize, const int* weights, const int* prices, int* costMatrix) {
    for (int i = 0; i <= totalNumber; ++i) {
        costMatrix[i * (totalSize + 1)] = 0; // если вместимость == 0, итоговая стоимость будет == 0
    }
    for (int i = 0; i <= totalSize; ++i) {
        costMatrix[i] = 0; // если можно положить 0 первых предметов, итоговая стоимость будет == 0
    }

    for (int count = 1; count <= totalNumber; ++count) {
        for (int size = 1; size <= totalSize; ++size) {
            if (size >= weights[count - 1]) {
                costMatrix[count * (totalSize + 1) + size] = MAX(
                    costMatrix[(count - 1) * (totalSize + 1) + size],
                    costMatrix[(count - 1) * (totalSize + 1) + (size - weights[count - 1])] + prices[count - 1]
                );
            }
            else costMatrix[count * (totalSize + 1) + size] = costMatrix[(count - 1) * (totalSize + 1) + size];
        }
    }
}

int main(void) {
    int totalNumber, totalSize;
    if (scanf("%i %i", &totalNumber, &totalSize) != 2) {
        exit(INPUT_ERROR);
    }

    int* weights = calloc(totalNumber, sizeof(int));
    int* prices = calloc(totalNumber, sizeof(int));
    if (!weights || !prices) {
        free(weights);
        exit(OUT_OF_MEMORY);
    }

    for (int i = 0; i < totalNumber; ++i) {
        if (scanf("%i %i", &weights[i], &prices[i]) != 2) {
            free(weights);
            free(prices);
            exit(INPUT_ERROR);
        }
    }

    int* costMatrix = calloc((totalNumber + 1) * (totalSize + 1), sizeof(int));
    if (!costMatrix) {
        free(weights);
        free(prices);
        exit(OUT_OF_MEMORY);
    }
    CalcMaxCost(totalNumber, totalSize, weights, prices, costMatrix);

    printf("%i\n", costMatrix[totalNumber * (totalSize + 1) + totalSize]);

    PrintItemsInfo(totalSize, totalNumber, totalSize, weights, prices, costMatrix);
    free(weights);
    free(prices);
    free(costMatrix);
}
