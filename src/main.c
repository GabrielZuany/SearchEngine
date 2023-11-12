#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <diretório_entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Hello, world!\n");

    return 0;
}
