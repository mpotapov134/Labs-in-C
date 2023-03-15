#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define VAR 'v'
// test 33 fails with error munmap_chunk(): invalid pointer and I don't see what's wrong

typedef struct Stack {
    int value;
    struct Stack* next;
} TStack;

TStack* Push(TStack* stack, int value) {
    TStack* newNode = (TStack*) malloc(sizeof(TStack));
    newNode->value = value;
    newNode->next = stack;
    return newNode;
}

TStack* Pop(TStack* stack, int* poppedValue) { // assert stack is not NULL
    TStack* head = stack;
    if (poppedValue) *poppedValue = stack->value; // record the value only when needed
    stack = stack->next;
    free(head);
    return stack;
}

int Peep(TStack* stack) {
    return stack ? stack->value : 0;
}

void Delete(TStack* stack) {
    while (stack) {
        stack = Pop(stack, NULL);
    }
}

int IsOperator(char symbol) {
    return (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/');
}

int Priority(int symbol) {
    // 1 for + and -, 2 for * and /, 0 for parentheses
    return IsOperator(symbol) ? (symbol == '+' || symbol == '-' ? 1 : 2) : 0;
}

int CreateBP(char* inputLine, int* numbersArray, char* backwardsPolish) { // return 1 on success, 0 on failure
    if (!strlen(inputLine)) return 0;

    TStack* opStack = NULL;

    char prevIsNumber = 0;
    char symbol, poppedOperator, marker = VAR;
    int currentNum = 0, indexOfLast = 0;

    for (unsigned i = 0; i < strlen(inputLine); ++i) {
        symbol = inputLine[i];

        if (!isdigit(symbol) && !IsOperator(symbol) && symbol != '(' && symbol != ')') { // invalid character
            Delete(opStack);
            return 0;
        }

        if (isdigit(symbol)) {
            currentNum = currentNum * 10 + atoi(&symbol);
            prevIsNumber = 1;
        }

        else {
            if (prevIsNumber) {
                marker = 'v'; // for some reason marker just resets to empty after a few iterations
                backwardsPolish = strncat(backwardsPolish, &marker, 1); // add a marker that a variable is there
                numbersArray[indexOfLast++] = currentNum; // and store the number in a separate list
                currentNum = 0;
                prevIsNumber = 0;
            }

            if (IsOperator(symbol)) {
                while (opStack && Priority(Peep(opStack)) >= Priority(symbol)) {
                    opStack = Pop(opStack, (int*) &poppedOperator);
                    backwardsPolish = strncat(backwardsPolish, &poppedOperator, 1);
                }
                opStack = Push(opStack, symbol);
            }

            else if (symbol == '(') {
                opStack = Push(opStack, symbol);
            }

            else { // come across ')'
                if (i != 0 && inputLine[i - 1] == '(') { // error: nothing inside brackets
                    Delete(opStack);
                    return 0;
                }

                while (opStack && Peep(opStack) != '(') {
                    opStack = Pop(opStack, (int*) &poppedOperator);
                    backwardsPolish = strncat(backwardsPolish, &poppedOperator, 1);
                }

                if (!opStack) { // error: didn't encounter an opening bracket
                    return 0;
                }
                opStack = Pop(opStack, NULL); // pops the remaining opening bracket
            }
        }
    }

    if (prevIsNumber) { // add the last number to the notation
        marker = 'v';
        backwardsPolish = strncat(backwardsPolish, &marker, 1);
        numbersArray[indexOfLast++] = currentNum;
    }

    while (opStack) { // add the remaining operators to the result
        opStack = Pop(opStack, (int*) &poppedOperator);

        if (!IsOperator(poppedOperator)) { // errror: unused parentheses left
            Delete(opStack);
            return 0;
        }

        backwardsPolish = strncat(backwardsPolish, &poppedOperator, 1);
    }

    return 1;
}

int Calculate(char* backwardsPolish, int* numbersArray, int* finalRes) { // return 1 on success, 0 on failure
    TStack* numStack = NULL;

    char symbol;
    int indexOfLast = 0, a, b, res;

    for (unsigned i = 0; i < strlen(backwardsPolish); ++i) {
        symbol = backwardsPolish[i];

        if (symbol == VAR) numStack = Push(numStack, numbersArray[indexOfLast++]);

        else {
            if (!numStack) {
                printf("syntax error\n");
                Delete(numStack);
                return 0;
            }
            numStack = Pop(numStack, &b);

            if (!numStack) {
                printf("syntax error\n");
                Delete(numStack);
                return 0;
            }
            numStack = Pop(numStack, &a);

            if (symbol == '+') {
                res = a + b;
            }
            else if (symbol == '-') {
                res = a - b;
            }
            else if (symbol == '*') {
                res = a * b;
            }
            else {
                if (b == 0) {
                    printf("division by zero\n");
                    Delete(numStack);
                    return 0;
                }
                res = a / b;
            }
            numStack = Push(numStack, res);
        }
    }

    numStack = Pop(numStack, finalRes);
    return 1;
}

int main() {
    char inputLine[1002];
    if (!fgets(inputLine, sizeof(inputLine), stdin)) {
        printf("syntax error\n");
        exit(0);
    }
    *strchr(inputLine, '\n') = 0;

    int* numbersArray = (int*) malloc(sizeof(inputLine) * sizeof(int));
    char* backwardsPolish = (char*) malloc(sizeof(*inputLine));

    if (!CreateBP(inputLine, numbersArray, backwardsPolish)) {
        printf("syntax error\n");
        free(numbersArray);
        free(backwardsPolish);
        exit(0);
    }

    int answer;
    if (Calculate(backwardsPolish, numbersArray, &answer)) {
        printf("%i\n", answer);
    }
    free(numbersArray);
    free(backwardsPolish);
}
