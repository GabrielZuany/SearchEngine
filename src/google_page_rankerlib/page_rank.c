#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/google_page_rankerlib/page_rank.h"
#include <math.h>
#include <stdbool.h>

ForwardList** google_page_ranker_read_out_links(char* graph_path){
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
    rewind(graph_file);

    ForwardList** out_links = malloc(n_lines * sizeof(ForwardList*));
    for(int i = 0; i < n_lines; i++){ out_links[i] = forward_list_construct(); }

    int fl_id = 0;
    char* filename = malloc(100 * sizeof(char));
    int* n_links = malloc(sizeof(int));
    while(!feof(graph_file)) {
        fscanf(graph_file, "%s", filename);
        fscanf(graph_file, "%d", n_links);
        // char** links = malloc(*(n_links) * sizeof(char*));
        char links[*(n_links)][100];

        // add na TST(1) o filename(doc atual) com o fl_id associado

        for(int i = 0; i < *(n_links); i++){
            fscanf(graph_file, "%s", links[i]);

            // add na TST(2) o links[i] com o fl_id associado (NAO ATUALIZA O ID)
            // se o cara ja estiver na TST(2), entao eu add na lista[ID] dele o filename(doc atual)
            // se o cara nao estiver na TST(2), entao eu crio a lista[ID] dele e add o filename(doc atual)
            // doc_out -> doc_in (doc_out tem um link para doc_in)

            forward_list_push_front(out_links[fl_id], links[i]);
        }

        forward_list_push_front(out_links[fl_id], filename); // filename is the head of the list

        fl_id++;
    }

    // in_links:
    // enquanto le os out_links, add numa tst cada cara que for lendo
    // se o cara não estiver na TST(2) (nao tiver um valor >>ID<< associado), entao eu crio a lista, senao, eu add na lista[ID].

    fclose(graph_file);
    free(filename);
    free(n_links);
    return out_links;
}

ForwardList* get_out_links_from_page(ForwardList** out_links, char* filename){
    // recupera o Id associado ao filename na TST
    // retorna out_links[id]
}

ForwardList* get_in_links_from_page(ForwardList** in_links, char* filename){
    // recupera o Id associado ao filename na TST(2)
    // retorna in_links[id]
}

void __init_page_rank(double *page_rank, ForwardList** out_links, ForwardList** in_links, int n_pages, int page_id) {
    static bool already_initialized = false;
    if (already_initialized) { return; }

    // power method initialization
    double *page_rank_new = malloc(n_pages * sizeof(double));
    for (int i = 0; i < n_pages; i++) { page_rank_new[i] = 1.0 / n_pages; }

    // power method update
    double delta = 0.0;
    double sum = 0.0;
    do {
        // Calculate page rank for each page
        for (int i = 0; i < n_pages; i++) {
            sum = 0.0;
            Node* in_links_node = forward_list_get_head_node(in_links[i]);
            
            // SUM(j E In(i)) { page_rank[k-1](j) / |out_links(j)| }
            while (in_links_node != NULL) {
                int page_j_E_In_i = *(int*)node_get_value(in_links_node);
                int size = forward_list_size(out_links[page_j_E_In_i]) - 1; // -1 because the first element is the filename itself
                if(size != 0){ sum += page_rank[page_j_E_In_i] / size; }
                in_links_node = forward_list_goto_next(in_links_node); 
            }

            // page_rank[k](i) = (1 - alpha) / N + alpha * SUM(j E In(i)) { page_rank[k-1](j) / |out_links(j)| }
            page_rank_new[i] = ((1 - DAMPING_FACTOR) / n_pages) + DAMPING_FACTOR * sum;
            if ((forward_list_size(out_links[i]) - 1) == 0) {
                //page_rank_new[i] += DAMPING_FACTOR * page_rank[i] / n_pages;
                page_rank_new[i] += DAMPING_FACTOR * page_rank[i];
            }
        }

        // power method convergence
        // delta = (1 / pages) * SUM(i) { |page_rank[k](i) - page_rank[k-1](i)| }
        delta = 0.0;
        for (int i = 0; i < n_pages; i++) { delta += fabs(page_rank_new[i] - page_rank[i]); }
        delta /= n_pages;

        // page_rank[k-1](i) = page_rank[k](i)
        for (int i = 0; i < n_pages; i++) { page_rank[i] = page_rank_new[i]; }

    } while (delta > EPSILON);

    double result = page_rank[page_id];
    free(page_rank_new);
    already_initialized = true;
}

double get_page_rank(double *page_rank,  ForwardList** out_links, ForwardList** in_links, int n_pages, int page_id){
    __init_page_rank(page_rank, out_links, in_links, n_pages, page_id);
    return page_rank[page_id];
}