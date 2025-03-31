#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "person.h"

void usage() {
    printf("Usage: program filename\n");
}

int readperson(person *p) {
    return scanf("%s\t%d\t%d\n", p->name, &p->has_medal, &p->maths_score) == 3;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage();
        return 1;
    }
    person p;
    FILE *out = fopen(argv[1], "wb");
    if (!out) {
        perror("Can't open file\n");
        return 2;
    }

    while (readperson(&p)) {
        fwrite(&p, sizeof(p), 1, out);
    }

    if (fclose(out) == EOF) {
        perror("Can't close file\n");
        return 3;
    }

    return 0;
}