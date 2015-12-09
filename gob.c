
#include <stdio.h>
#include "gob.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Which file do you want to dump?\n");
        return 1;
    }

    char *const path = argv[1];

    int dumped = gob_dump_symbols(path);
    if (dumped < 0) {
        printf("Had a problem dumping symbols\n");
        return dumped;
    }

    int traced = gob_trace(path);
    if (traced < 0) {
        printf("Had a problem tracing\n");
        return traced;
    }

    return 0;
}
