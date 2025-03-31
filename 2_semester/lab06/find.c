#include <stdio.h>
#include <stdlib.h>

#include "person.h"

void usage() {
    printf("Usage: program filename\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage();
        return 1;
    }
    person p;
    FILE *in = fopen(argv[1], "r");
    if (!in) {
        perror("Can't open file\n");
        return 2;
    }

    while (fread(&p, sizeof(p), 1, in)) {
        if (p.has_medal == 1 && p.maths_score < 40) {
            printf("%s\t%d\t%d\t\tPassed\n", p.name, p.has_medal, p.maths_score);
        } else {
            printf("%s\t%d\t%d\t\tFailed\n", p.name, p.has_medal, p.maths_score);
        }
    }

    if (fclose(in) == EOF) {
        perror("Can't close file\n");
        return 3;
    }

    return 0;
}