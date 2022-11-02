#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


void BadInput() {
    printf("bad input\n");
    exit(0);
}


void Swap(char *ind1, char *ind2) {
    char temp = *ind1;
    *ind1 = *ind2;
    *ind2 = temp;
}


int GetValueOrDie(int digit) { /* I could just return the ASCII code of the symbol,
                                  so that it was possible to use any symbols but then the tests would fail */
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
    int symbolCounter[256] = {0}; // set to 256, so any ASCII symbol could be used
    for (unsigned i = 0; i < strlen(src); ++i) {
        int currentSymbol = GetValueOrDie(src[i]);
        if (symbolCounter[currentSymbol] > 0) {
            res = 1;
            break;
        }
        ++ symbolCounter[currentSymbol];
    }
    return res;
}


unsigned FindLastValueLessThanNext(char *src) {
    unsigned res;
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


void ReverseTail(char *src) {
    for (unsigned i = 0; i < strlen(src) / 2; ++ i) {
        Swap(&src[i], &src[strlen(src) - i - 1]);
    }
}


int CanGeneratePermutations(char *src) {
    for (unsigned i = 0; i + 1 < strlen(src); ++i) {
        if (src[i] < src[i + 1]) {
            return 1;
        }
    }
    return 0;
}


void GenerateNextPermutation(char *src) {
    unsigned indLast = FindLastValueLessThanNext(src);
    unsigned indGreater = FindGreaterValueWithMaxIndex(src, indLast);
    Swap(&src[indLast], &src[indGreater]);
    ReverseTail(src + indLast + 1);
}


void GeneratePermutations(char *src, int nPermutations) {
    int permutationsCounter = 0;
    while (CanGeneratePermutations(src) && permutationsCounter < nPermutations) {
        GenerateNextPermutation(src);
        printf("%s\n", src);
        ++ permutationsCounter;
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
