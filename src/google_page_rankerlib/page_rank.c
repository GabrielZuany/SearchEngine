#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "google_page_rankerlib/page_rank.h"
#include "containerslib/forward_list.h"
#include <math.h>

double calc_page_rank(double *page_rank, ForwardList** out_links, ForwardList** in_links, int n_pages, int page_id) {
    // power method initialization
    double *page_rank_new = malloc(n_pages * sizeof(double));
    for (int i = 0; i < n_pages; i++) {
        page_rank_new[i] = 1.0 / n_pages;
    }

    // power method update
    double delta = 0.0;
    double sum = 0.0;
    do {
        for (int i = 0; i < n_pages; i++) {
            sum = 0.0;
            Node* in_links_node = forward_list_get_head_node(in_links[i]);
            
            // SUM(j E In(i)) { page_rank[k-1](j) / |out_links(j)| }
            while (in_links_node != NULL) {
                int j = *(int*)forward_list_get_head_value(in_links_node);
                if (forward_list_size(out_links[j]) != 0) {
                    sum += page_rank[j] / forward_list_size(out_links[j]);
                }

                j = *(int*)forward_list_get_next_value(in_links_node);
                in_links_node = forward_list_get_next_node(in_links_node);
            }

            // page_rank[k](i) = (1 - alpha) / N + alpha * SUM(j E In(i)) { page_rank[k-1](j) / |out_links(j)| }
            page_rank_new[i] = (1 - DAMPING_FACTOR) / n_pages + DAMPING_FACTOR * sum;
            if (forward_list_size(out_links[i]) == 0) {
                //page_rank_new[i] += DAMPING_FACTOR * page_rank[i] / n_pages;
                page_rank_new[i] += DAMPING_FACTOR * page_rank[i];
            }
        }

        // power method convergence
        // delta = (1 / pages) * SUM(i) { |page_rank[k](i) - page_rank[k-1](i)| }
        delta = 0.0;
        for (int i = 0; i < n_pages; i++) {
            delta += fabs(page_rank_new[i] - page_rank[i]);
        }
        delta /= n_pages;

        // page_rank[k-1](i) = page_rank[k](i)
        for (int i = 0; i < n_pages; i++) {
            page_rank[i] = page_rank_new[i];
        }

    } while (delta > EPSILON);

    double result = page_rank[page_id];
    free(page_rank_new);
    return result;
}
