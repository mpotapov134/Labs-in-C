#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


void BadInput() {
    printf("bad input\n");
    exit(0);
}


int Value(int digit) {
    int res = 0;
    if (isdigit(digit)) {
        res = digit - '0';
    }
    // commented so that E. Petrov's tests don't fail

    // else if ('a' <= digit && digit <= 'z') {
    //     res = digit - 'a' + 10;
    // }
    // else if ('A' <= digit && digit <= 'Z') {
    //     res = digit - 'A' + 10;
    // }
    else {
        BadInput();
    }
    return res;
}


int HasRepeats(char *src) {
    int res = 0;
    int symbolCounter[100] = {0};
    for (unsigned i = 0; i < strlen(src); ++i) {
        int currentSymbol = Value(src[i]);
        if (symbolCounter[currentSymbol] > 0) {
            res = 1;
            break;
        }
        ++ symbolCounter[currentSymbol];
    }
    return res;
}


unsigned FindLastValueLessThanNext(char *src) {
    unsigned res = 1000; /* special code, in case no fitting index was found, meaning
                            no permautations are available, e.g. in 54321 */
    for (unsigned i = 0; i + 1 < strlen(src); ++i) {
        if (src[i] < src[i + 1]) {
            res = i;
        }
    }
    return res;
}


unsigned FindGreaterValueWithMaxIndex(char *src, unsigned j) {
    unsigned res = 1000;
    for (unsigned i = j + 1; i < strlen(src); ++i) {
        if (src[i] > src[j]) {
            res = i;
        }
    }
    return res;
}


void Swap(char *ind1, char *ind2) {
    char temp = *ind1;
    *ind1 = *ind2;
    *ind2 = temp;
}


void ReverseTail(char *src, unsigned startPosInd) {
    unsigned length = strlen(src) - startPosInd;
    for (unsigned i = 0; i < length / 2; ++ i) {
        Swap(&src[startPosInd + i], &src[strlen(src) - i - 1]);
    }
}


void GeneratePermutations(char *src, int nPermutations) {
    unsigned indLast = FindLastValueLessThanNext(src);
    unsigned indGreater = FindGreaterValueWithMaxIndex(src, indLast);
    int permautationsCounter = 0;
    while (indLast != 1000 && permautationsCounter < nPermutations) {
        Swap(&src[indLast], &src[indGreater]);
        ReverseTail(src, indLast + 1);
        printf("%s\n", src);
        indLast = FindLastValueLessThanNext(src);
        indGreater = FindGreaterValueWithMaxIndex(src, indLast);
        ++ permautationsCounter;
    }
}


int main(void) {
    char input[1000];
    if (!fgets(input, 999, stdin)) {
        BadInput();
    }
    char *EolPointer = strchr(input, '\n');
    if (EolPointer) {
        *EolPointer = 0;
    }
    unsigned nPermutations = 0;
    if (1 != scanf("%u", &nPermutations)) {
        BadInput();
    }
    if (HasRepeats(input)) {
        BadInput();
    }
    GeneratePermutations(input, nPermutations);


    return EXIT_SUCCESS;
}
