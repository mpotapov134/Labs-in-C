#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUF_SIZE 10000


typedef struct TextClass {
    unsigned char text[BUF_SIZE + 1];
    // checkPos stores the checking position in the current chunk of text and checkPosGlobal - in the text overall
    size_t shift, checkPos, checkPosGlobal, length;
} TextClass;


typedef struct SampleClass {
    unsigned char sample[17];
    int shiftTable[256];
    size_t length;
} SampleClass;


void MakeShiftTable(unsigned char *sample, int *resTable) {
    unsigned sampleLen = strlen((char*) sample);
    for (unsigned i = 0; i + 1 < sampleLen; ++i) {
        resTable[(size_t) sample[i]] = sampleLen - i - 1;
    }
}


int CanCheck(TextClass *textEntity) {
    return textEntity->checkPos <= textEntity->length; // means the sample can be placed in the text
}


void CheckMatch(TextClass *textEntity, SampleClass *sampleEntity) {
    size_t sampleLen = sampleEntity->length;
    for (unsigned i = 0; i < sampleLen; ++i) {
        printf("%zu ", textEntity->checkPosGlobal - i);
        if (textEntity->text[textEntity->checkPos - 1 - i] != sampleEntity->sample[sampleLen - 1 - i]) {
            return;
        }
    }
}


void Refill(TextClass *textEntity, SampleClass *sampleEntity) {
    textEntity->checkPos -= textEntity->shift;
    size_t sampleLen = sampleEntity->length;
    size_t offset = textEntity->length - textEntity->checkPos;

    memmove(textEntity->text, textEntity->text + textEntity->checkPos, offset);
    size_t bytesRead = fread(textEntity->text + offset, sizeof(char), BUF_SIZE - offset, stdin);
    textEntity->text[bytesRead + offset] = 0;
    textEntity->length = bytesRead + offset;
    textEntity->checkPos = sampleLen;
}


void Proceed(TextClass *textEntity, SampleClass *sampleEntity) {
    unsigned char firstCheckedSymb = textEntity->text[textEntity->checkPos - 1];
    textEntity->shift = sampleEntity->shiftTable[(size_t) firstCheckedSymb];
    textEntity->checkPos += textEntity->shift;
    textEntity->checkPosGlobal += textEntity->shift;
}


void Search(TextClass *textEntity, SampleClass *sampleEntity) {
    while (CanCheck(textEntity)) {
        CheckMatch(textEntity, sampleEntity);
        Proceed(textEntity, sampleEntity);
        if (!CanCheck(textEntity)) {
            Refill(textEntity, sampleEntity);
        }
    }
}


int main(void) {
    unsigned char sample[18];
    if (!fgets((char*) sample, sizeof(sample), stdin)) {
        exit(0);
    }
    *strchr((char*) sample, '\n') = 0;
    size_t sampleLen = strlen((char*) sample);

    unsigned char text[BUF_SIZE + 1];
    size_t bytesRead = fread(text, sizeof(char), BUF_SIZE, stdin);
    text[bytesRead] = 0;

    SampleClass sampleEntity;
    strcpy((char*) sampleEntity.sample, (char*) sample);
    for (unsigned i = 0; i < 256; ++i) {
        sampleEntity.shiftTable[i] = sampleLen;
    }
    MakeShiftTable(sample, sampleEntity.shiftTable);
    sampleEntity.length = sampleLen;

    TextClass textEntity = {"", 0, sampleLen, sampleLen, bytesRead};
    strcpy((char*) textEntity.text, (char*) text);

    Search(&textEntity, &sampleEntity);
    printf("\n");
}
