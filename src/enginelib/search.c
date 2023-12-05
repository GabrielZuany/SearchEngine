#include "include/enginelib/search.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INIT_QUERIES_SIZE 10
#define INIT_QUERY_SIZE 10

char*** enginelib_search_init() {
    char ***queries = malloc(INIT_QUERIES_SIZE * sizeof(char *));
    size_t queries_size = INIT_QUERIES_SIZE;
    size_t queries_curr_size = 0;
    char *query = NULL;
    size_t query_length = 0;

    while(getline(&query, &query_length, stdin) != -1) {
        if(queries_curr_size == queries_size) {
            queries_size *= 2;
            queries = realloc(queries, queries_size * sizeof(char *));
        }
        queries[queries_curr_size++] = malloc(INIT_QUERY_SIZE * sizeof(char));


        char *token = NULL;
        const char *delim = " ";
        do {
            token = strtok(query, delim);
            if(token != NULL && token[strlen(token) - 1] == '\n')
                token[strlen(token) - 1] = '\0';

            size_t query_curr_size = 0;
            size_t query_size = INIT_QUERY_SIZE;

            if(token != NULL) {
                if(query_curr_size == query_size) {
                    query_size *= 2;
                    queries[queries_curr_size] = realloc(queries[queries_curr_size], query_size * sizeof(char));
                }
                queries[queries_curr_size][query_curr_size++] = token;
            }
        } while(token != NULL);
    }

    return queries;
}
