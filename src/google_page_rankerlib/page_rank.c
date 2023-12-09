#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "include/google_page_rankerlib/page_rank.h"
#include "google_page_rankerlib/page_rank.h"
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
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
        if(ch == '\n'){ n_lines++; }
    }
    rewind(graph_file);

    StringSt* tst_out = stringst_init();
    StringSt* tst_in = stringst_init();

    ForwardList** out_links = calloc(n_lines + 1, sizeof(ForwardList*));
    ForwardList** in_links = calloc(self->n_pages, sizeof(ForwardList*));
    for(int i = 0; i < n_lines + 1; i++){ out_links[i] = forward_list_construct(); }
    for(int i = 0; i < self->n_pages; i++){ in_links[i] = forward_list_construct(); }

    int tst_out_id = 0;
    int tst_in_id = 0;
    int* tst_in_id_ptr = NULL;
    int* tst_out_id_ptr = NULL;

    char* filename = malloc(100 * sizeof(char));
    int* n_links = malloc(sizeof(int));
    char** links = NULL;
       
    for(int i = 0; i < n_lines; i++) {
        if (fscanf(graph_file, "%s", filename)) {};     // warning suppression
        if (fscanf(graph_file, "%d", n_links)) {};      // warning suppression
        links = calloc(*(n_links) , sizeof(char*));

        // add na TST(out) o filename(doc atual) com o tst_out_id associado
        tst_out_id_ptr = malloc(sizeof(int));
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
           char* new_doc = strdup(filename);
            if(!stringst_contains(tst_in, links[i])){
                tst_in_id_ptr = malloc(sizeof(int));
                *tst_in_id_ptr = tst_in_id;
                stringst_put(tst_in, links[i], tst_in_id_ptr);
                forward_list_push_front(in_links[tst_in_id], new_doc);
                tst_in_id++;
            }else{
                int* tst_in_id_ptr = (int*)stringst_get(tst_in, links[i]);
                forward_list_push_front(in_links[*tst_in_id_ptr], new_doc);
            }
        }
        tst_out_id++;
        free(links);
    }
    
    self->out_links = out_links;
    self->in_links = in_links;
    self->tst_out = tst_out;
    self->tst_in = tst_in;

    fclose(graph_file);
    /* free(filename); */
    free(n_links);
    
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
    double *page_rank_k_1 = malloc(self->n_pages * sizeof(double));

    // power method update
    ForwardList** in_links = self->in_links;
    ForwardList** out_links = self->out_links;
    double delta_k = 0.0;
    double sum = 0.0;
    bool is_first_iteration = true;
    do{
        delta_k = 0.0;
        
        if(is_first_iteration){
            for (int i = 0; i < self->n_pages; i++) {self->page_rank[i] = page_rank_k_1[i] = 1.0 / (double)self->n_pages; }
            is_first_iteration = false;
            continue;
        }

        for(int i = 0; i < self->n_pages; i++){
            sum = 0.0;
            // SUM FUNCTION
            Node* in_links_node = forward_list_get_head_node(in_links[i]);  // set of pages that link to page i
            while(in_links_node != NULL){                                   // for each document j in In(i) loop
                char* document = (char*)node_get_value(in_links_node);
                if(document == NULL){ continue; }
                int j = *(int*)stringst_get(self->tst_out, document);       // get list id of document j
                int out_j_size = forward_list_size(out_links[j]);           // get number of outlinks in document j
                sum += page_rank_k_1[j] / (double)out_j_size;
                in_links_node = forward_list_goto_next(in_links_node);
            }
            sum*=DAMPING_FACTOR;

            sum+= (1 - DAMPING_FACTOR) / (double)self->n_pages;

            int out_i_size = forward_list_size(out_links[i]);
            if(out_i_size == 0){ // dangling node
                sum += DAMPING_FACTOR * page_rank_k_1[i];
            }
            // sum is the PR(i) of the current iteration
            // TEM QUE ATUALIZAR NA POSICAO DESSE CARA, NAO NO i, OLHA A CHAMADA DA FUNCAO DE BAIXO!!!
            // o primeiro que ele pega eh o e.txt (i = 4), mas o primeiro que ele atualiza eh o a.txt (i = 0)
            // ele tem que atualizar na posicao do i, nao no i
                        
            page_rank_k_1[i] = sum;
        }

        // power method convergence
        // delta = (1 / pages) * SUM(i) { |page_rank[k](i) - page_rank[k-1](i)| }
        delta_k = 0.0;
        for (int i = 0; i < self->n_pages; i++) { delta_k += fabs(page_rank_k_1[i] - self->page_rank[i]); }
        delta_k /= self->n_pages;
        
        for(int i = 0; i<self->n_pages; i++){ self->page_rank[i] = page_rank_k_1[i];}

        if(delta_k < EPSLON){break;}        
    }while(true);

    free(page_rank_k_1);
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
