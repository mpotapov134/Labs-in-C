#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#define BUF_SIZE 100000


typedef struct TextClass {
    char text[BUF_SIZE + 1];
    int hash, index, indexGlobal;
    size_t length;
} TextClass;


typedef struct SampleClass {
    char sample[17];
    int hash;
    size_t length;
} SampleClass;


int CalcHash(char *string, size_t chunkLength) {
    int res = 0;
    for (unsigned i = 0; i < chunkLength; ++i) {
        res += (((unsigned char) string[i] % 3) * pow(3, i));
    }
    return res;
}


void CheckMatch(TextClass *textEntity, SampleClass *sampleEntity) {
    if (textEntity->hash != sampleEntity->hash) {
        return;
    }
    for (unsigned i = 0; i < sampleEntity->length; ++i) {
        printf("%u ", textEntity->indexGlobal + i);
        if (textEntity->text[textEntity->index - 1 + i] != sampleEntity->sample[i]) {
            return;
        }
    }
}


void Refill(TextClass *textEntity, SampleClass *sampleEntity) {
    size_t chunkLength = sampleEntity->length;
    size_t offset = chunkLength - 1;
    memmove(textEntity->text, textEntity->text + textEntity->length - offset, offset);
    size_t bytesRead = fread(textEntity->text + offset, sizeof(char), BUF_SIZE - offset, stdin);
    textEntity->text[bytesRead + offset] = 0;
    textEntity->length = bytesRead + offset;
}


int MoveIfPossible(TextClass *textEntity, SampleClass *sampleEntity) {
    size_t chunkLength = sampleEntity->length;
    textEntity->index ++;
    textEntity->indexGlobal ++;
    int indOfLast = textEntity->index - 1 + chunkLength - 1; // index of the last symbol in the text to be compared with the sample

    if (indOfLast >= textEntity->length) { // the buffer is too short to check, need to refill
        Refill(textEntity, sampleEntity);
        textEntity->index = 1;
        indOfLast = chunkLength - 1;
        if (indOfLast >= textEntity->length) {
            return 0; // refilling the buffer didn't help, can't proceed anymore
        }
    }

    textEntity->hash /= 3;
    textEntity->hash += ((unsigned char) textEntity->text[indOfLast] % 3) * pow(3, chunkLength - 1);
    return 1; // the buffer is still long enough, so we can proceed successfully
}


void Search(TextClass *textEntity, SampleClass *sampleEntity) {
    int canMove = textEntity->length >= sampleEntity->length;
    while (canMove) {
        CheckMatch(textEntity, sampleEntity);
        canMove = MoveIfPossible(textEntity, sampleEntity);
    }
}


int main(void) {
    setlocale(LC_ALL, "Russian");

    char sample[18];
    if (!fgets(sample, sizeof(sample), stdin)) {
        exit(0);
    }
    *strchr(sample, '\n') = 0;
    size_t sampleLen = strlen(sample);
    int sampleHash = CalcHash(sample, sampleLen);
    printf("%i ", sampleHash);

    char text[BUF_SIZE + 1];
    size_t bytesRead = fread(text, sizeof(char), BUF_SIZE, stdin);
    text[bytesRead] = 0;

    TextClass textEntity = {"", CalcHash(text, sampleLen), 1, 1, bytesRead};
    strcpy(textEntity.text, text);
    SampleClass sampleEntity = {"", sampleHash, sampleLen};
    strcpy(sampleEntity.sample, sample);

    Search(&textEntity, &sampleEntity);
    printf("\n");
}
