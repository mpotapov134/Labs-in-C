#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define VAR 'v'
/*Test 33 fails with message "munmap_chunk(): invalid pointer".
  I've read online that it happens because of trying to free the memory
  that was not allocated by malloc. I suspect it happens due to the way
  Pop() is implemented here. What could be wrong and how do I fix it? Thank you.*/

typedef struct Stack {
    int value;
    struct Stack* next;
} TStack;


int IsOperator(char symbol) {
    return (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/');
}


int Priority(char opeation) {
    if (opeation == '(' || opeation == ')') {
        return 0;
    }
    if (opeation == '+' || opeation == '-') {
        return 1;
    }
    if (opeation == '*' || opeation == '/') {
        return 2;
    }
}


TStack* CreateStack(int value) {
    TStack* newStack = (TStack*) malloc(sizeof(TStack));
    newStack->value = value;
    newStack->next = NULL;
    return newStack;
}


void Push(TStack** stack, int value) {
    TStack* newHead = CreateStack(value);
    newHead->next = *stack;
    *stack = newHead;
}


int Pop(TStack** stack) {
    TStack* headToRemove = *stack;
    *stack = (*stack)->next;
    int poppedValue = headToRemove->value;
    free(headToRemove);
    return poppedValue;
}


int GetTopValue(TStack* stack) {
    if (!stack) {
        return 0;
    }
    return stack->value;
}


void DeleteStack(TStack* stack) {
    int placeholder;
    while (stack) {
        placeholder = Pop(&stack);
    }
}


char* CreateBackwardsPolish(char* inputLine, int* numbersArray) {
    if (!strlen(inputLine)) {
        return NULL;
    }
    char* backwardsPolish = (char*) malloc(sizeof(*inputLine));
    backwardsPolish[0] = 0;
    char symbol, poppedOperator, placeholder = VAR;
    int currentNum = 0, indexOfLast = 0;
    TStack* opStack = NULL;

    for (unsigned i = 0; i < strlen(inputLine); ++i) {
        symbol = inputLine[i];
        if (isdigit(symbol)) {
            if (i != 0 && inputLine[i - 1] == ')') {
                DeleteStack(opStack);
                return NULL; // error: number goes right after a closing bracket
            }
            currentNum = currentNum * 10 + atoi(&symbol);
        }

        else {
            if (!IsOperator(symbol) && symbol != '(' && symbol != ')') {
                return NULL; // incorrect input, can't make a proper BP notation
            }

            if (i != 0 && isdigit(inputLine[i - 1])) { // if we had a number before a non-number, we should add it to the notation
                strncat(backwardsPolish, &placeholder, 1); // add a marker that a variable is there
                numbersArray[indexOfLast++] = currentNum;
                currentNum = 0;
            }

            if (IsOperator(symbol)) {
                while (opStack && Priority((char) GetTopValue(opStack)) >= Priority(symbol)) {
                    poppedOperator = (char) Pop(&opStack);
                    strncat(backwardsPolish, &poppedOperator, 1);
                }
                Push(&opStack, symbol);
            }

            else if (symbol == '(') {
                if (i != 0 && (isdigit(inputLine[i - 1]) || inputLine[i - 1] == ')')) {
                    DeleteStack(opStack);
                    return NULL; // error: opening bracket can't go after a number or a closing bracket
                }
                Push(&opStack, symbol);
            }

            else { // come across ')'
                if (i != 0 && inputLine[i - 1] == '(') {
                    DeleteStack(opStack);
                    return NULL; // error: nothing inside brackets
                }

                while (opStack && GetTopValue(opStack) != '(') {
                    poppedOperator = (char) Pop(&opStack);
                    strncat(backwardsPolish, &poppedOperator, 1);
                }

                if (!opStack) {
                    return NULL; // error: didn't encounter an opening bracket
                }
                poppedOperator = Pop(&opStack); // pops the remaining opening bracket
            }
        }
    }

    if (isdigit(inputLine[strlen(inputLine) - 1])) { // add the last number to the notation
        strncat(backwardsPolish, &placeholder, 1);
        numbersArray[indexOfLast++] = currentNum;
        currentNum = 0;
    }

    while (opStack) { // add the operators to the result while there are any
        poppedOperator = Pop(&opStack);
        if (!IsOperator(poppedOperator)) {
            DeleteStack(opStack);
            return NULL; // in the end the operations stack should only have operators, and not brackets
        }
        strncat(backwardsPolish, &poppedOperator, 1);
    } // by the end of the cycle the stack is already empty, so there is no need to delete it
    return backwardsPolish;
}


int* Calculate(char* backwardsPolish, int* numbersArray) {
    TStack* numStack = NULL;
    char symbol;
    int indexOfLast = 0, a, b, res;
    int* finalRes = NULL;

    for (unsigned i = 0; i < strlen(backwardsPolish); ++i) {
        symbol = backwardsPolish[i];
        if (symbol == VAR) {
            Push(&numStack, numbersArray[indexOfLast++]);
        }

        else {
            if (!numStack) {
                printf("syntax error\n");
                return NULL;
            }
            b = Pop(&numStack);

            if (!numStack) {
                printf("syntax error\n");
                return NULL;
            }
            a = Pop(&numStack);

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
                    return NULL;
                }
                res = a / b;
            }
            Push(&numStack, res);
        }
    }
    res = Pop(&numStack);
    finalRes = &res;
    return finalRes;
}


int main(void) {
    char inputLine[1003];
    if (!fgets(inputLine, sizeof(inputLine), stdin)) {
        printf("syntax error\n");
        exit(0);
    }
    *strchr(inputLine, '\n') = 0;

    int* numbersArray = (int*) malloc(sizeof(inputLine) * sizeof(int));
    char* backwardsPolish = CreateBackwardsPolish(inputLine, numbersArray);
    if (!backwardsPolish) { // realised that the input is incorrect while making the BP notation
        printf("syntax error\n");
        free(numbersArray);
        free(backwardsPolish);
        exit(0);
    }


    int* result = Calculate(backwardsPolish, numbersArray);
    if (result) {
        printf("%i\n", *result);
    }
    free(numbersArray);
    free(backwardsPolish);
}
