#ifndef __UTILS_H__
#define __UTILS_H__

typedef struct s_cell {
    int end_edge;
    float weight;
    struct s_cell *next;
} cell;

typedef struct s_list_adj {
    cell *head;
} list_adj;

typedef struct s_graph {
    list_adj *edges;
    int num_edges;
} graph;

cell *create_cell(int end_edge, float weight);
list_adj *create_empty_list();
graph *create_empty_graph(int num_edges);
void add_head(list_adj *l, int end_edge, float weight);
void print_list(list_adj *l);
void print_graph(graph *g);
graph *read_graph_from_file(const char *filename);
int is_markov_graph(graph *g);
void write_mermaid_file(graph *g, const char *filename);

#endif

