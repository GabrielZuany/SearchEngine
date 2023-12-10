#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "google_page_rankerlib/page_rank.h"
#include "page_indexerlib/index.h"
#include "containerslib/utils.h"
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

struct PageRank
{
    ForwardList** out_links;    // ForwardList<int>* -> out_links[page_id] -> list of page_ids
    ForwardList** in_links;     // ForwardList<int>* -> in_links[page_id] -> list of page_ids
    StringSt* tst_out_links;    // St<string, int> -> tst_out_links[page_name] -> page_id in ForwardList
    StringSt* tst_in_links;     // St<string, int> -> tst_in_links[page_name] -> page_id in ForwardList
    StringSt* page_idpage_map;  // St<string, int> -> page_idpage_map[page_name] -> page_id (general map)
    double* page_rank;          // page_rank[page_id] -> page_rank of page_id
    int n_pages;                // number of pages (general map size)
};

PageRank* page_rank_init(Index* index){
    PageRank* page_rank = malloc(sizeof(PageRank));
    page_rank->out_links = NULL;
    page_rank->in_links = NULL;
    page_rank->n_pages = index_get_num_pages(index);
    page_rank->page_rank = malloc(page_rank->n_pages * sizeof(double));
    page_rank->tst_out_links = NULL;
    page_rank->tst_in_links = NULL;
    page_rank->page_idpage_map = NULL;
    return page_rank;
}

void __page_rank_set_page_idpage_map(PageRank* self, char* folderpath){
    char* filepath = utils_pathcat(folderpath, "index.txt");
    FILE* index_file = fopen(filepath, "r");
    if (index_file == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", filepath);
        exit(EXIT_FAILURE);
    }

    int len = 0;
    int size = 4;
    char **idpage_page_map = malloc(__SIZEOF_POINTER__ * size);
    StringSt* page_idpage_map = stringst_init();

    // iterate over all lines in index
    char *page_name = NULL;
    int page_name_length = 0;
    ssize_t read;
    while ((read = getline(&page_name, &page_name_length, index_file)) != -1) {
        if (page_name[read - 1] == '\n')
            page_name[read - 1] = '\0';

        char *strdup_page_name = strdup(page_name);
        if (len == size) {
            size <<= 1;
            idpage_page_map = realloc(idpage_page_map, __SIZEOF_POINTER__ * size);
        }
        idpage_page_map[len++] = strdup_page_name;
        int* val = malloc(sizeof(int));     // recently created (2023-12-23 :: 04:27)
        *val = len-1;                       // recently created (2023-12-23 :: 04:27)
        stringst_put(page_idpage_map, strdup_page_name, val);
        free(strdup_page_name);      // recently created (2023-12-23 :: 04:27)
    }
    free(page_name);
    free(filepath);
    fclose(index_file);
    free(idpage_page_map);          // recently created (2023-12-23 :: 04:27)
    self->page_idpage_map = page_idpage_map;
}

void page_rank_finish(PageRank* self){
    for(int i = 0; i < self->n_pages; i++){ 
        if(self->out_links[i] != NULL){ forward_list_destroy(self->out_links[i]); }
        if(self->in_links[i] != NULL){ forward_list_destroy(self->in_links[i]); }
    }
    free(self->out_links);
    free(self->in_links);
    free(self->page_rank);
    stringst_finish(self->tst_in_links, NULL);
    stringst_finish(self->tst_out_links, NULL);
    stringst_finish(self->page_idpage_map, free);
    free(self);
}

PageRank* page_rank_build_links(PageRank* self, char* graph_path){
    // filename number of links link1 link2 ... linkN
    // 24011.txt 7 3391.txt 12241.txt 12682.txt 6762.txt 30380.txt 17661-8.txt 22322-8.txt
    __page_rank_set_page_idpage_map(self, graph_path);

    graph_path = utils_pathcat(graph_path, "graph.txt");
    FILE* graph_file = fopen(graph_path, "r");
    if (graph_file == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", graph_path);
        exit(EXIT_FAILURE);
    }

    // get number of pages that have outlinks
    int n_lines = 0;
    while(!feof(graph_file)){ (fgetc(graph_file) == '\n') ? n_lines++ : n_lines; }
    rewind(graph_file); // reset file pointer to start of file

    StringSt* tst_out_links = stringst_init();
    StringSt* tst_in_links  = stringst_init();

    ForwardList** out_links = malloc(self->n_pages * sizeof(ForwardList*));
    ForwardList** in_links  = malloc(self->n_pages * sizeof(ForwardList*));
    for(int i = 0; i < self->n_pages; i++){ out_links[i] = forward_list_construct(); }
    for(int i = 0; i < self->n_pages; i++){ in_links[i] = forward_list_construct(); }

    char* filename = malloc(256 * sizeof(char));
    int* n_links   = malloc(sizeof(int));
    char** links   = NULL;
    for(int i = 0; i < n_lines; i++) {
        if (fscanf(graph_file, "%s", filename)) {};
        if (fscanf(graph_file, "%d", n_links))  {};
        links = malloc(*(n_links) * sizeof(char*));

        // add na TST(out) o filename(doc atual) com o tst_out_id associado
        int* pageId = (int*)stringst_get(self->page_idpage_map, filename);
        stringst_put(tst_out_links, filename, pageId);

        // add na TST(in) os links com o tst_in_id associado
        for(int i = 0; i < *(n_links); i++){
            links[i] = malloc(sizeof(char) * 256);
            if (fscanf(graph_file, "%s", links[i])) {};

            if(out_links[*pageId] != NULL){ // links[i](out_links[tst_out_id]) -> filename
                forward_list_push_front(out_links[*pageId], links[i]);
            }else{
                out_links[*pageId] = forward_list_construct();
                forward_list_push_front(out_links[*pageId], links[i]);
            }

            // add na TST(in) o links[i] com o tst_in_id associado
            if(!stringst_contains(tst_in_links, links[i])){
                int* pageid = stringst_get(self->page_idpage_map, links[i]);
                stringst_put(tst_in_links, links[i], pageid);
                forward_list_push_front(in_links[*pageid], strdup(filename));
            }else{
                int* pageid = stringst_get(self->page_idpage_map, links[i]);
                forward_list_push_front(in_links[*pageid], strdup(filename));
            }
        }
        free(links);
    }
    
    self->out_links = out_links;
    self->in_links = in_links;
    self->tst_out_links = tst_out_links;
    self->tst_in_links = tst_in_links;
    
    fclose(graph_file);    
    free(filename);
    free(n_links);
    free(graph_path);
    return self;
}

void __init_page_rank(PageRank* self) {
    static bool already_initialized = false;
    if (already_initialized) { return; }

    // power method initialization
    double *page_rank_k_1 = malloc(self->n_pages * sizeof(double));
    // for (int i = 0; i < self->n_pages; i++) { page_rank_k_1[i] = 1.0 / (double)self->n_pages; }

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
            Node* in_links_node = forward_list_get_head_node(in_links[i]);
            while(in_links_node != NULL){ // for each document j in In(i)
                char* document = (char*)node_get_value(in_links_node);
                int j = *(int*)stringst_get(self->tst_out_links, document); // get list id of document j
                int out_j_size = forward_list_size(out_links[j]);
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
            page_rank_k_1[i] = sum;
        }
        
        // power method convergence
        // delta = (1 / pages) * SUM(i) { |page_rank[k](i) - page_rank[k-1](i)| }
        delta_k = 0.0;
        for (int i = 0; i < self->n_pages; i++) { delta_k += fabs(page_rank_k_1[i] - self->page_rank[i]); }
        delta_k /= self->n_pages;
        
        for(int i = 0; i<self->n_pages; i++){ self->page_rank[i] = page_rank_k_1[i];}

        // printf("delta_k: %.8f\n", delta_k);
        if(delta_k < EPSLON){break;}        
    }while(true);
    
    free(page_rank_k_1);
    already_initialized = true;
}

double page_rank_get(PageRank* self, char* page){
    int* page_id_ptr = (int*)stringst_get(self->tst_out_links, page);
    if(page_id_ptr == NULL){ return 0.0; }
    int page_id = *page_id_ptr;
    __init_page_rank(self);
    return self->page_rank[page_id];
}
