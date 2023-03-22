#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define VAR 'v'
#define MAX_LEN 1000

typedef struct Stack {
    int value;
    struct Stack* next;
} Stack_t;

Stack_t* Push(Stack_t* stack, int value) {
    Stack_t* newNode = malloc(sizeof(Stack_t));
    newNode->value = value;
    newNode->next = stack;
    return newNode;
}

Stack_t* Pop(Stack_t* stack, int* poppedValue) { // assert stack is not NULL
    Stack_t* head = stack;
    if (poppedValue) *poppedValue = stack->value; // record the value only when needed
    stack = stack->next;
    free(head);
    return stack;
}

int Peek(Stack_t* stack) {
    return stack ? stack->value : 0;
}

void Delete(Stack_t* stack) {
    while (stack) {
        stack = Pop(stack, NULL);
    }
}

int IsOperator(char symbol) {
    return (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/');
}

int Priority(int symbol) {
    // 1 for + and -, 2 for * and /, 0 for parentheses
    if (IsOperator(symbol)) {
        if (symbol == '+' || symbol == '-') return 1;
        else return 2;
    }
    else return 0;
}

int CreateBP(char* inputLine, int* numbersArray, char* backwardsPolish) { // return 1 on success, 0 on failure
    if (!strlen(inputLine)) return 0;

    Stack_t* opStack = NULL;

    int poppedOperator, currentNum = 0, indexOfLast = 0;;
    char prevIsNumber = 0, marker = VAR;

    for (unsigned i = 0; i < strlen(inputLine); ++i) {
        char symbol = inputLine[i];

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
                backwardsPolish = strncat(backwardsPolish, &marker, 1); // add a marker that a variable is there
                numbersArray[indexOfLast++] = currentNum; // and store the number in a separate list
                currentNum = 0;
                prevIsNumber = 0;
            }

            if (IsOperator(symbol)) {
                while (opStack && Priority(Peek(opStack)) >= Priority(symbol)) {
                    opStack = Pop(opStack, &poppedOperator);
                    backwardsPolish = strncat(backwardsPolish, (char*) &poppedOperator, 1);
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

                while (opStack && Peek(opStack) != '(') {
                    opStack = Pop(opStack, &poppedOperator);
                    backwardsPolish = strncat(backwardsPolish, (char*) &poppedOperator, 1);
                }

                if (!opStack) { // error: didn't encounter an opening bracket
                    return 0;
                }
                opStack = Pop(opStack, NULL); // pops the remaining opening bracket
            }
        }
    }

    if (prevIsNumber) { // add the last number to the notation
        backwardsPolish = strncat(backwardsPolish, (char*) &marker, 1);
        numbersArray[indexOfLast++] = currentNum;
    }

    while (opStack) { // add the remaining operators to the result
        opStack = Pop(opStack, &poppedOperator);

        if (!IsOperator(poppedOperator)) { // errror: unused parentheses left
            Delete(opStack);
            return 0;
        }

        backwardsPolish = strncat(backwardsPolish, (char*) &poppedOperator, 1);
    }

    return 1;
}

int Calculate(char* backwardsPolish, int* numbersArray, int* finalRes) { // return 1 on success, 0 on failure
    Stack_t* numStack = NULL;

    int indexOfLast = 0, first, second, res;

    for (unsigned i = 0; i < strlen(backwardsPolish); ++i) {
        char symbol = backwardsPolish[i];

        if (symbol == VAR) numStack = Push(numStack, numbersArray[indexOfLast++]);

        else {
            if (!numStack) {
                printf("syntax error\n");
                return 0;
            }
            numStack = Pop(numStack, &second);

            if (!numStack) {
                printf("syntax error\n");
                return 0;
            }
            numStack = Pop(numStack, &first);

            if (symbol == '+') res = first + second;

            else if (symbol == '-') res = first - second;

            else if (symbol == '*') res = first * second;

            else {
                if (second == 0) {
                    printf("division by zero\n");
                    Delete(numStack);
                    return 0;
                }
                res = first / second;
            }
            numStack = Push(numStack, res);
        }
    }

    numStack = Pop(numStack, finalRes);
    Delete(numStack);
    return 1;
}

int main() {
    char inputLine[MAX_LEN + 2]; // MAX_LEN for the string itself + 1 for a potential \n + 1 for the terminating 0
    if (!fgets(inputLine, MAX_LEN + 2, stdin)) {
        printf("syntax error\n");
        exit(0);
    }
    char* newlineSymbPointer = strchr(inputLine, '\n');
    if (newlineSymbPointer) *newlineSymbPointer = 0;

    int* numbersArray = malloc(sizeof(inputLine) * sizeof(int));
    char* backwardsPolish = malloc(sizeof(inputLine));
    backwardsPolish[0] = 0;

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
