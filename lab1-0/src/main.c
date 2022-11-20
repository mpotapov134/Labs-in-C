#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>


typedef struct TextClass {
    char text[17];
    size_t length;
    size_t shift, checkPos;
} TextClass;


typedef struct SampleClass {
    char sample[17];
    size_t length;
    int shiftTable[UCHAR_MAX + 1];
} SampleClass;


void IsMatch(TextClass *textEntity, SampleClass *sampleEntity) {
    int counter = 0;
    for (int i = textEntity->length - 1; i >= 0; --i) {
        printf("%li ", textEntity->checkPos - counter++);
        if (textEntity->text[i] != sampleEntity->sample[i]) {
            return;
        }
    }
}


void Proceed(TextClass *textEntity, SampleClass *sampleEntity) {
    char shiftArray[17];
    textEntity->shift = sampleEntity->shiftTable[(size_t)textEntity->text[textEntity->length - 1]];
    textEntity->checkPos += textEntity->shift;
    if (fread(shiftArray, sizeof(char), textEntity->shift, stdin) != textEntity->shift) {
        textEntity->text[0] = 0;
        textEntity->length = 0;
        return;
    }
    for (unsigned i = 0; i < textEntity->length - textEntity->shift; ++i) {
        textEntity->text[i] = textEntity->text[i + textEntity->shift];
    }
    for (unsigned i = textEntity->length - textEntity->shift; i < textEntity->length; ++i) {
        textEntity->text[i] = shiftArray[i - (textEntity->length - textEntity->shift)];
    }
}


int TextEnded(TextClass *textEntity) {
    return textEntity->length == 0;
}


void Search(TextClass *text, SampleClass *sample) {
    while (!TextEnded(text)) {
        IsMatch(text, sample);
        Proceed(text, sample);
    }
}


void MakeShiftTable(char *sample, int *resTable) {
    unsigned sampleLen = strlen(sample);
    for (unsigned i = 0; i + 1 < sampleLen; ++i) {
        resTable[(size_t)sample[i]] = sampleLen - i - 1;
    }
}


int main(void) {
    char sample[18];
    if (!fgets(sample, 18, stdin)) {
        exit(0);
    }
    *strchr(sample, '\n') = 0;

    int shiftTable[UCHAR_MAX + 1];
    for (unsigned i = 0; i < 256; ++i) {
        shiftTable[i] = strlen(sample);
    }
    MakeShiftTable(sample, shiftTable);

    SampleClass sampleEntity;
    strcpy(sampleEntity.sample, sample);
    sampleEntity.length = strlen(sample);
    for (unsigned i = 0; i < UCHAR_MAX + 1; ++i) {
        sampleEntity.shiftTable[i] = shiftTable[i];
    }

    char text[18];
    if (fread(text, sizeof(char), sampleEntity.length, stdin) != sampleEntity.length) {
        exit(0);
    }

    TextClass textEntity = {"", sampleEntity.length, 0, sampleEntity.length};
    strcpy(textEntity.text, text);

    Search(&textEntity, &sampleEntity);
    printf("\n");
}
