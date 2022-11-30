#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct TextClass {
    unsigned char text[17];
    size_t shift, checkPos;
} TextClass;


typedef struct SampleClass {
    unsigned char sample[17];
    int shiftTable[256];
} SampleClass;


void IsMatch(TextClass *textEntity, SampleClass *sampleEntity) {
    int counter = 0;
    for (unsigned i = strlen((char*)textEntity->text); i > 0; --i) {
        printf("%li ", textEntity->checkPos - counter++);
        if (textEntity->text[i - 1] != sampleEntity->sample[i - 1]) {
            return;
        }
    }
}


void Proceed(TextClass *textEntity, SampleClass *sampleEntity) {
    char shiftArray[17];
    textEntity->shift = sampleEntity->shiftTable[(size_t)textEntity->text[strlen((char*)textEntity->text) - 1]];
    textEntity->checkPos += textEntity->shift;
    if (fread(shiftArray, sizeof(char), textEntity->shift, stdin) != textEntity->shift) {
        textEntity->text[0] = 0;
        return;
    }
    for (unsigned i = 0; i < strlen((char*)textEntity->text) - textEntity->shift; ++i) {
        textEntity->text[i] = textEntity->text[i + textEntity->shift];
    }
    for (unsigned i = strlen((char*)textEntity->text) - textEntity->shift; i < strlen((char*)textEntity->text); ++i) {
        textEntity->text[i] = shiftArray[i - (strlen((char*)textEntity->text) - textEntity->shift)];
    }
}


int TextEnded(TextClass *textEntity) {
    return strlen((char*)textEntity->text) == 0;
}


void Search(TextClass *text, SampleClass *sample) {
    while (!TextEnded(text)) {
        IsMatch(text, sample);
        Proceed(text, sample);
    }
}


void MakeShiftTable(unsigned char *sample, int *resTable) {
    unsigned sampleLen = strlen((char*)sample);
    for (unsigned i = 0; i + 1 < sampleLen; ++i) {
        resTable[(size_t)sample[i]] = sampleLen - i - 1;
    }
}


int main(void) {
    unsigned char sample[18];
    if (!fgets((char*)sample, 18, stdin)) {
        exit(0);
    }
    *strchr((char*)sample, '\n') = 0;

    int shiftTable[256];
    for (unsigned i = 0; i < 256; ++i) {
        shiftTable[i] = strlen((char*)sample);
    }
    MakeShiftTable(sample, shiftTable);

    SampleClass sampleEntity;
    strcpy((char*)sampleEntity.sample, (char*)sample);
    for (unsigned i = 0; i < 256; ++i) {
        sampleEntity.shiftTable[i] = shiftTable[i];
    }

    unsigned char text[18];
    if (fread(text, sizeof(char), strlen((char*)sample), stdin) != strlen((char*)sample)) {
        exit(0);
    }
    text[strlen((char*)sample)] = 0;

    TextClass textEntity = {"", 0, strlen((char*)sample)};
    strcpy((char*)textEntity.text, (char*)text);

    Search(&textEntity, &sampleEntity);
    printf("\n");
}
