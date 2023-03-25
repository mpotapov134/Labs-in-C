#include <stdlib.h>
#include <stdio.h>
#define MAX(a, b) (a >= b) ? a : b

void PrintItemsInfo(int totalSize, int count, int size, int weights[], int prices[], int costMatrix[][totalSize + 1]) { // totalSize is only used to define the matrix size
    if (!costMatrix[count][size]) return;

    if (costMatrix[count][size] == costMatrix[count - 1][size]) { // сумма осталась та же, значит, элемент с номером count не положили
        PrintItemsInfo(totalSize, count - 1, size, weights, prices, costMatrix);
    }
    else {
        PrintItemsInfo(totalSize, count - 1, size - weights[count - 1], weights, prices, costMatrix);
        printf("%i %i\n", weights[count - 1], prices[count - 1]);
    }
}

void CalcMaxCost(int totalNumber, int totalSize, int weights[], int prices[], int costMatrix[][totalSize + 1]) {
    for (int i = 0; i <= totalNumber; ++i) costMatrix[i][0] = 0; // если вместимость == 0, итоговая стоимость будет == 0
    for (int i = 0; i <= totalSize; ++i) costMatrix[0][i] = 0; // если можно положить 0 первых предметов, итоговая стоимость будет == 0

    for (int count = 1; count <= totalNumber; ++count) {
        for (int size = 1; size <= totalSize; ++size) {
            if (size >= weights[count - 1]) {
                costMatrix[count][size] = MAX(costMatrix[count - 1][size], costMatrix[count - 1][size - weights[count - 1]] + prices[count - 1]);
            }
            else costMatrix[count][size] = costMatrix[count - 1][size];
        }
    }
}

int main(void) {
    int totalNumber, totalSize;
    if (2 != scanf("%i %i", &totalNumber, &totalSize)) exit(0);

    int weights[totalNumber], prices[totalNumber];
    for (int i = 0; i < totalNumber; ++i) {
        if (2 != scanf("%i %i", &weights[i], &prices[i])) exit(0);
    }

    int costMatrix[totalNumber + 1][totalSize + 1];
    CalcMaxCost(totalNumber, totalSize, weights, prices, costMatrix);

    printf("%i\n", costMatrix[totalNumber][totalSize]);

    PrintItemsInfo(totalSize, totalNumber, totalSize, weights, prices, costMatrix);
}
