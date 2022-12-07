#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUF_SIZE 10000


typedef struct TextClass {
    unsigned char text[BUF_SIZE + 1];
    // checkPos stores the checking position in the current chunk of text and checkPosGlobal - in the text overall
    size_t shift, checkPos, checkPosGlobal;
} TextClass;


typedef struct SampleClass {
    unsigned char sample[17];
    int shiftTable[256];
} SampleClass;


int CanCheck(TextClass *textEntity) {
    return textEntity->checkPos <= strlen((char*) textEntity->text);
}


void CheckMatch(TextClass *textEntity, SampleClass *sampleEntity) {
    size_t sampleLen = strlen((char*) sampleEntity->sample);
    for (unsigned i = 0; i < sampleLen; ++i) {
        printf("%zu ", textEntity->checkPosGlobal - i);
        if (textEntity->text[textEntity->checkPos - i - 1] != sampleEntity->sample[sampleLen - i - 1]) {
            return;
        }
    }
}


void Refill(TextClass *textEntity, SampleClass *sampleEntity) {
    textEntity->checkPos -= textEntity->shift;
    size_t textLen = strlen((char*) textEntity->text);
    size_t sampleLen = strlen((char*) sampleEntity->sample);
    size_t tailLen = textLen - textEntity->checkPos; // defines the offset

    strcpy((char*) textEntity->text, (char*) textEntity->text + textEntity->checkPos);
    size_t bitesRead = fread(textEntity->text + tailLen, sizeof(char), BUF_SIZE - tailLen, stdin);
    if (bitesRead < sampleLen) {
        textEntity->text[0] = 0; // remaining text is shorter than the sample, no need to compare anymore
    }
    textEntity->text[bitesRead] = 0;
    textEntity->checkPos = sampleLen;
}


void Proceed(TextClass *textEntity, SampleClass *sampleEntity) {
    unsigned char firstCheckedSymb = textEntity->text[textEntity->checkPos - 1];
    textEntity->shift = sampleEntity->shiftTable[(size_t) firstCheckedSymb];
    textEntity->checkPos += textEntity->shift;
    textEntity->checkPosGlobal += textEntity->shift;

    if (!CanCheck(textEntity)) {
        Refill(textEntity, sampleEntity);
    }
}


void Search(TextClass *text, SampleClass *sample) {
    while (CanCheck(text)) {
        CheckMatch(text, sample);
        Proceed(text, sample);
    }
}


void MakeShiftTable(unsigned char *sample, int *resTable) {
    unsigned sampleLen = strlen((char*) sample);
    for (unsigned i = 0; i + 1 < sampleLen; ++i) {
        resTable[(size_t) sample[i]] = sampleLen - i - 1;
    }
}


int main(void) {
    unsigned char sample[18];
    if (!fgets((char*) sample, 18, stdin)) {
        exit(0);
    }
    *strchr((char*) sample, '\n') = 0;
    size_t sampleLen = strlen((char*) sample);

    SampleClass sampleEntity;
    strcpy((char*) sampleEntity.sample, (char*) sample);
    for (unsigned i = 0; i < 256; ++i) {
        sampleEntity.shiftTable[i] = sampleLen;
    }
    MakeShiftTable(sample, sampleEntity.shiftTable);

    unsigned char text[BUF_SIZE + 1];
    size_t bitesRead = fread(text, sizeof(char), BUF_SIZE, stdin);
    if (bitesRead < sampleLen) {
        exit(0);
    }
    text[bitesRead] = 0;

    TextClass textEntity = {"", 0, sampleLen, sampleLen};
    strcpy((char*) textEntity.text, (char*) text);

    Search(&textEntity, &sampleEntity);
    printf("\n");
}
