#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void encrypt(char*, char*, char*, int);
int main(int argc, char** arcv) {
    char* word = "HELLO";
    char* key= "XMCKL";
    char* crypt = (char*)calloc(6, sizeof(char));
    encrypt(word, key, crypt, 5);
    printf("word:%s key:%s crypt:%s\n", word, key, crypt);
    return 0;
}

void encrypt(char* word, char* key, char* crypt, int length) {
    int i;
    int w = 0;
    int k = 0;
    int total = 0;
    for(i = 0; i < length; i++) {
        if(word[i] > 64 && word[i] < 91) {
            w = word[i] - 65;
        } else if(word[i] == 32) {
            w = 26;
        }
        if(key[i] > 64 && key[i] < 91) {
            k = key[i] - 65;
        } else if(key[i] == 32) {
            k = 26;
        }
        total = w + k;
        if(total > 26) {
            total = total-26;
        }
        if(total == 26) {
            crypt[i]=32;
        } else {
            crypt[i] = total + 65;
        }
    }
}
