#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int base1, base2, has_fractional = 0;

void bad_input(void) {
    printf("bad input\n");
    exit(0);
}


int value(int digit) {
    int result;
    if ('0' <= digit && digit <= '9') {
        result = digit - '0';
    }
    else if ('a' <= digit && digit <= 'f') {
        result = digit - 'a' + 10;
    }
    else if ('A' <= digit && digit <= 'F') {
        result = digit - 'A' + 10;
    }
    else {
        bad_input();
    }
    if (result >= base1) {
        bad_input();
    }
    return result;
}


char to_char(int digit) {
    if (0 <= digit && digit <= 9) {
        return digit + '0';
    }
    return digit + 'a' - 10;
}


void swap(char *s1, char *s2) {
    char temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}


void reverse(char *string){
    long length = strlen(string);
    for (int i = 0; i < length / 2; ++i) {
        swap(&string[i], &string[length - i - 1]);
    }
}


void to_internal(char b1_number[13], long long *res_internal) {
    int i = 0;
    long long int_part = 0, numerator = 0, denominator = 1;
    if (strchr(b1_number, '.')) {
        has_fractional = 1;
    }
    if (b1_number[strlen(b1_number) - 1] == '.' || b1_number[0] == '.') {
        bad_input();
    }
    while (b1_number[i] != '.' && i < strlen(b1_number)) {
        int_part = int_part * base1 + value(b1_number[i]);
        ++i;
    }
    ++i;
    for (i; i < strlen(b1_number); ++i) {
        numerator = numerator * base1 + value(b1_number[i]);
        denominator *= base1;
    }

    res_internal[0] = int_part;
    res_internal[1] = numerator;
    res_internal[2] = denominator;
}


void to_b2(long long *internal_representation) {
    long long int_part = internal_representation[0],
        numerator = internal_representation[1],
        denominator = internal_representation[2];

    char b2_int_part[50], b2_fract_part[12];
    int x, i = 0;

    b2_int_part[0] = '0';
    while (int_part > 0) {
        x = int_part % base2;
        int_part /= base2;
        b2_int_part[i] = to_char(x);
        ++i;
    }
    if (i == 0) {
        ++i;
    }
    b2_int_part[i] = 0;
    reverse(b2_int_part);
    printf("%s", b2_int_part);

    if (has_fractional) {
        for (int i = 0; i < 12; ++i) {
            x = (numerator * base2) / denominator;
            numerator = numerator * base2 - x * denominator;
            b2_fract_part[i] = to_char(x);
            // printf("%c", to_char(x));
        }
        b2_fract_part[12] = 0;
        printf(".%s", b2_fract_part);
    }
    printf("\n");
}


void main(void) {
    int b1, b2;
    if (2 != scanf("%i%i", &b1, &b2)) { // base input
        bad_input();
    }

    if (b1 < 2 || 16 < b1 || b2 < 2 || 16 < b2) { // base check
        bad_input();
    }
    base1 = b1;
    base2 = b2;

    char b1_number[13];
    scanf("%13s", b1_number); // original number input

    // translate original number to internal representation
    long long res_internal[3], *res_internal_pointer = &res_internal[0];
    to_internal(b1_number, res_internal_pointer);

    // translate internal to base2 representation and print
    to_b2(res_internal);
}
