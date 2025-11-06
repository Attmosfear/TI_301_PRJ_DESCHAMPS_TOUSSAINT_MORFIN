#ifndef __UTILS_H__
#define __UTILS_H__

typedef struct s_cell {
    int end_edge;
    float weight;
    s_cell *next;
} cell;

typedef struct s_list {
    cell *head;
} list;

typedef struct s_graph {
    list *edges;
    int num_edges;
} graph;

cell *create_cell(int end_edge, float weight);
list *create_empty_list();
void add_head(list *l, int end_edge, float weight);
graph *create_empty_graph(int num_edges);
void print_list(list *l);
void print_graph(graph *g);

#endif