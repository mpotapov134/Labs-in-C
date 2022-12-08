#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#define BUF_SIZE 10000


typedef struct TextClass {
    char text[BUF_SIZE + 1];
    int hash, index, indexGlobal;
    size_t length;
} TextClass;


typedef struct SampleClass {
    char sample[17];
    int hash;
} SampleClass;


int CalcHash(char *string, size_t chunkLength) {
    int res = 0;
    for (unsigned i = 0; i < chunkLength; ++i) {
        res += (((unsigned char) string[i] % 3) * pow(3, i));
    }
    return res;
}


int CanCheck(TextClass *textEntity) {
    return textEntity->length != 0;
}


void CheckMatch(TextClass *textEntity, SampleClass *sampleEntity) {
    if (textEntity->hash != sampleEntity->hash) {
        return;
    }
    for (unsigned i = 0; i < strlen(sampleEntity->sample); ++i) {
        printf("%i ", textEntity->indexGlobal + i);
        if (textEntity->text[textEntity->index - 1 + i] != sampleEntity->sample[i]) {
            return;
        }
    }
}


void Refill(TextClass *textEntity, SampleClass *sampleEntity) {
    size_t chunkLength = strlen(sampleEntity->sample);
    size_t offset = chunkLength - 1;
    memmove(textEntity->text, textEntity->text + textEntity->length - offset, offset);
    size_t bitesRead = fread(textEntity->text + offset, sizeof(char), BUF_SIZE - offset, stdin);
    textEntity->text[bitesRead + offset] = 0;
    textEntity->length = bitesRead + offset;
}


void Proceed(TextClass *textEntity, SampleClass *sampleEntity) {
    size_t chunkLength = strlen(sampleEntity->sample);
    textEntity->hash /= 3;
    textEntity->index ++;
    textEntity->indexGlobal ++;

    int indOfLast = textEntity->index - 1 + chunkLength - 1;
    if (indOfLast >= (int) textEntity->length) {
        Refill(textEntity, sampleEntity);
        textEntity->index = 1;
        indOfLast = chunkLength - 1;
    }
    if (textEntity->length >= chunkLength) {
        textEntity->hash += ((unsigned char) textEntity->text[indOfLast] % 3) * pow(3, chunkLength - 1);
    }
    else {
        textEntity->text[0] = 0; // remaining text is shorter than the sample, no need to compare anymore
        textEntity->length = 0;
    }
}


void Search(TextClass *textEntity, SampleClass *sampleEntity) {
    while (CanCheck(textEntity)) {
        CheckMatch(textEntity, sampleEntity);
        Proceed(textEntity, sampleEntity);
    }
}


int main(void) {
    setlocale(LC_ALL, "Russian");

    char sample[18];
    if (!fgets(sample, 18, stdin)) {
        exit(0);
    }
    *strchr(sample, '\n') = 0;
    int sampleHash = CalcHash(sample, strlen(sample));
    printf("%i ", sampleHash);

    char text[BUF_SIZE + 1];
    size_t bitesRead = fread(text, sizeof(char), BUF_SIZE, stdin);
    if (bitesRead < strlen(sample)) {
        exit(0);
    }
    text[bitesRead] = 0;

    TextClass textEntity = {"", CalcHash(text, strlen(sample)), 1, 1, bitesRead};
    strcpy(textEntity.text, text);
    SampleClass sampleEntity = {"", sampleHash};
    strcpy(sampleEntity.sample, sample);

    Search(&textEntity, &sampleEntity);
    printf("\n");
}
