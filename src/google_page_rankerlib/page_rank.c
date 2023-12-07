#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "include/google_page_rankerlib/page_rank.h"
#include "google_page_rankerlib/page_rank.h"
#include <math.h>
#include <stdbool.h>
// gcc -o teste_page_rank src/containerslib/node.c src/containerslib/forward_list.c src/containerslib/utils.c src/google_page_rankerlib/page_rank.c src/main.c src/containerslib/string_st.c src/containerslib/tst.c  src/containerslib/exceptions.c -g

// no launch.json
// "cwd": "/home/zuany/LinuxWorkEnvironmnet/SearchEngine/",
// "program": "/home/zuany/LinuxWorkEnvironmnet/SearchEngine/teste_page_rank",

struct PageRank
{
    ForwardList** out_links;
    ForwardList** in_links;
    int n_pages;
    double* page_rank;
    StringSt* tst_out;
    StringSt* tst_in;
};

PageRank* page_rank_init(int n_pages){
    PageRank* page_rank = malloc(sizeof(PageRank));
    page_rank->out_links = NULL;
    page_rank->in_links = NULL;
    page_rank->n_pages = n_pages;
    page_rank->page_rank = malloc(n_pages * sizeof(double));
    page_rank->tst_out = NULL;
    page_rank->tst_in = NULL;
    return page_rank;
}

void page_rank_finish(PageRank* self){
    for(int i = 0; i < self->n_pages; i++){ 
        if(self->out_links[i] != NULL){ forward_list_destroy(self->out_links[i]); }
        if(self->in_links[i] != NULL){ forward_list_destroy(self->in_links[i]); }
    }
    free(self->out_links);
    free(self->in_links);
    free(self->page_rank);
    stringst_finish(self->tst_out);
    stringst_finish(self->tst_in);
    free(self);
}

void print_string(char* str){
    printf("%s", str);
}
PageRank* page_rank_build_links(PageRank* self, char* graph_path){
    // filename number of links link1 link2 ... linkN
    // 24011.txt 7 3391.txt 12241.txt 12682.txt 6762.txt 30380.txt 17661-8.txt 22322-8.txt
    FILE* graph_file = fopen(graph_path, "r");
    if (graph_file == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", graph_path);
        exit(EXIT_FAILURE);
    }

    int n_lines = 0;
    char ch;
    while(!feof(graph_file)){
        ch = fgetc(graph_file);
        if(ch == '\n'){
            n_lines++;
        }
    }
    n_lines--; // desconsidera a ultima linha
    rewind(graph_file);

    StringSt* tst_out = stringst_init();
    StringSt* tst_in = stringst_init();

    ForwardList** out_links = calloc(n_lines + 2, sizeof(ForwardList*));
    ForwardList** in_links = NULL;
    for(int i = 0; i < n_lines + 2; i++){ out_links[i] = forward_list_construct(); }

    int tst_out_id = 0;
    int tst_in_id = 0;
    char* filename = malloc(100 * sizeof(char));
    int* n_links = malloc(sizeof(int));
    char** links = NULL;
    while(!feof(graph_file)) {
        if (fscanf(graph_file, "%s", filename)) {};
        if (fscanf(graph_file, "%d", n_links)) {};
        links = calloc(*(n_links) , sizeof(char*));

        // add na TST(out) o filename(doc atual) com o tst_out_id associado
        int* tst_out_id_ptr = malloc(sizeof(int));
        *tst_out_id_ptr = tst_out_id;
        stringst_put(tst_out, filename, tst_out_id_ptr);

        // add na TST(in) os links com o tst_in_id associado
        for(int i = 0; i < *(n_links); i++){
            links[i] = calloc(sizeof(char), 50);
            if (fscanf(graph_file, "%s", links[i])) {};

            if(out_links[tst_out_id] != NULL){ // links[i](out_links[tst_out_id]) -> filename
                forward_list_push_front(out_links[tst_out_id], links[i]);
            }else{
                out_links[tst_out_id] = forward_list_construct();
                forward_list_push_front(out_links[tst_out_id], links[i]);
            }

            // add na TST(in) o links[i] com o tst_in_id associado
            // erro: está dando um replace quando cria um novo id.
            /*
            id: 5
            out_links_from_page 12241.txt: [19848-8.txt, 17661-8.txt, 30380.txt, 6762.txt, 305.txt, 25911-8.txt, 3391.txt]
            in_links_from_page 12241.txt: [27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt, 27603-8.txt]page_rank_value: 0.15000000
            */
            if(!stringst_contains(tst_in, links[i])){
                int* tst_in_id_ptr = malloc(sizeof(int));
                *tst_in_id_ptr = tst_in_id;
                stringst_put(tst_in, links[i], tst_in_id_ptr);
                in_links = realloc(in_links, (tst_in_id + 1) * sizeof(ForwardList*));
                in_links[tst_in_id] = forward_list_construct();
                forward_list_push_front(in_links[tst_in_id], filename);
                tst_in_id++;
            }else{
                int* tst_in_id_ptr = (int*)stringst_get(tst_in, links[i]);
                forward_list_push_front(in_links[*tst_in_id_ptr], filename);
            }
        }
        tst_out_id++;
    }
    
    self->out_links = out_links;
    self->in_links = in_links;
    self->tst_out = tst_out;
    self->tst_in = tst_in;

    // fclose(graph_file);

    char* key = "12241.txt";
    int* id = (int*)stringst_get(self->tst_out, key);
    printf("id: %d\n", *id);
    ForwardList* out_links_from_page = get_out_links_from_page(self, key);
    printf("out_links_from_page %s: ", key);
    forward_list_print(out_links_from_page, print_string);

    ForwardList* in_links_from_page = get_in_links_from_page(self, key);
    printf("\nin_links_from_page %s: ", key);
    forward_list_print(in_links_from_page, print_string);
    
    return self;
}

ForwardList* get_out_links_from_page(PageRank* self, char* filename){
    // recupera o Id associado ao filename na TST(1)
    int* id = (int*)stringst_get(self->tst_out, filename);
    if(id == NULL){ return NULL; }
    return self->out_links[*id];
}

ForwardList* get_in_links_from_page(PageRank* self, char* filename){
    // recupera o Id associado ao filename na TST(2)
    int* id = (int*)stringst_get(self->tst_in, filename);
    if(id == NULL){ return NULL; }
    return self->in_links[*id];
}

void __init_page_rank(PageRank* self) {
    static bool already_initialized = false;
    if (already_initialized) { return; }

    // power method initialization
    double *page_rank_new = malloc(self->n_pages * sizeof(double));
    for (int i = 0; i < self->n_pages; i++) { page_rank_new[i] = 1.0 / self->n_pages; }

    // power method update
    double delta = 0.0;
    double sum = 0.0;
    do {
        // Calculate page rank for each page
        for (int i = 0; i < self->n_pages; i++) {
            sum = 0.0;
            Node* in_links_node = forward_list_get_head_node(self->in_links[i]);
            
            // SUM(j E In(i)) { page_rank[k-1](j) / |out_links(j)| }
            while (in_links_node != NULL) {
                char* document = (char*)node_get_value(in_links_node);
                int page_j_E_In_i = *(int*)stringst_get(self->tst_out, document);
                int size = forward_list_size(self->out_links[page_j_E_In_i]);
                if(size != 0){ sum += self->page_rank[page_j_E_In_i] / size; }
                in_links_node = forward_list_goto_next(in_links_node); 
            }

            // page_rank[k](i) = (1 - alpha) / N + alpha * SUM(j E In(i)) { page_rank[k-1](j) / |out_links(j)| }
            page_rank_new[i] = ((1 - DAMPING_FACTOR) / self->n_pages) + DAMPING_FACTOR * sum;
            if ((forward_list_size(self->out_links[i]) - 1) == 0) {
                //page_rank_new[i] += DAMPING_FACTOR * page_rank[i] / n_pages;
                page_rank_new[i] += DAMPING_FACTOR * self->page_rank[i];
            }
        }

        // power method convergence
        // delta = (1 / pages) * SUM(i) { |page_rank[k](i) - page_rank[k-1](i)| }
        delta = 0.0;
        for (int i = 0; i < self->n_pages; i++) { delta += fabs(page_rank_new[i] - self->page_rank[i]); }
        delta /= self->n_pages;

        // page_rank[k-1](i) = page_rank[k](i)
        for (int i = 0; i < self->n_pages; i++) { self->page_rank[i] = page_rank_new[i]; }

    } while (delta > EPSILON);

    free(page_rank_new);
    already_initialized = true;
}

double page_rank_get(PageRank* self, char* page){
    int* page_id_ptr = (int*)stringst_get(self->tst_out, page);
    if(page_id_ptr == NULL){ return 0.0; }
    int page_id = *page_id_ptr;
    __init_page_rank(self);
    return self->page_rank[page_id];
}

double* page_rank_get_all(PageRank* self){
    __init_page_rank(self);
    return self->page_rank;
}