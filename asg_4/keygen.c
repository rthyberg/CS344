#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char** argv) {
    if(argc < 2) {
        printf("USAGE: %s length\n",argv[0]);
        return 1;

    }
    srand(time(0));
    int count = atoi(argv[1]);
    int i;
    int rng;
    for(i = 0; i < count; i++) {
        rng = rand()%27;
        if(rng == 26) {
            printf(" ");
        } else {
            printf("%c", ('A' + rng));
        }
    }
    printf("\n");
    return 0;
}
