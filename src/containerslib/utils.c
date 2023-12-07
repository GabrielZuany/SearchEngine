#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "containerslib/utils.h"
#include "containerslib/exceptions.h"

char *utils_pathcat(const char *path1, const char *path2) {
    size_t path1_length = strlen(path1);
    size_t path2_length = strlen(path2);

    char *path = malloc(path1_length + path2_length + 1);
    if (path == NULL)
        exception_throw_failure("utils_pathcat - Failed to allocate memory for path");

    memcpy(path, path1, path1_length);
    memcpy(path + path1_length, path2, path2_length + 1);

    return path;
}

void utils_inplacestrtolower(char *str) {
    for ( ; *str; ++str) *str = tolower(*str);
}
