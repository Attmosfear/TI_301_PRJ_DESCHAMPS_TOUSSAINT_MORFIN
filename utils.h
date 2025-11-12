#ifndef __UTILS_H__
#define __UTILS_H__


// Fonctions pour les listes et les graphs
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

typedef struct s_tarjan_vertex {
    int id;
    int num;
    int low;
    int in_stack;
} tarjan_vertex;

typedef struct s_classe {
    char *name;
    tarjan_vertex *vertices;
    int nb_vertices;
    int capacity;
} classe;

typedef struct s_partition {
    classe *classes;
    int nb_classes;
    int capacity;
} partition;

typedef struct s_stack {
    int *data;
    int top;
    int capacity;
} stack;

tarjan_vertex* init_tarjan_vertices(graph *g);
void tarjan_parcours(int v, graph *g, tarjan_vertex *vertices, stack *stack, int *num_counter, partition *partition);

// Fonctions pour la pile
stack* create_stack(int capacity);
void push(stack *s, int value);
int pop(stack *s);
int peek(stack *s);
int is_empty(stack *s);
void free_stack(stack *s);

// Fonctions pour les classes
classe* create_classe(const char *name);
void add_vertex_to_classe(classe *c, tarjan_vertex v);
void print_classe(classe *c);
void free_classe(classe *c);

// Fonctions pour la partition
partition* create_partition();
void add_classe_to_partition(partition *p, classe *c);
void print_partition(partition *p);
void free_partition(partition *p);



#endif

