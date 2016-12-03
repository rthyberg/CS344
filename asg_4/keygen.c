#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char** argv) {
    if(argc < 2) {
        printf("USAGE: %s length\n",argv[0]);
        return 1;

    }
    srand(time(0));
    int count = atoi(argv[1]); // take in input as string and convert to num
    int i;
    int rng;
    for(i = 0; i < count; i++) {
        rng = rand()%27;  // generate random key by mod
        if(rng == 26) {   //  if 26 make space
            printf(" ");
        } else {
            printf("%c", ('A' + rng)); // add the offset to the starint letter of A
        }
    }
    printf("\n"); // add newline to file
    return 0;
}
