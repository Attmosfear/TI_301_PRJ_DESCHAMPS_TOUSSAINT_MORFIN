#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"


// ============= FONCTIONS UTILITAIRE =============

static int min(int a, int b) {
    return (a < b) ? a : b;
}

 
cell *create_cell(int end_edge, float weight) {
    cell *c = malloc(sizeof(cell));
    c->end_edge = end_edge;
    c->weight = weight;
    c->next = NULL;
    return c;
}

list_adj *create_empty_list() {
    list_adj *l = malloc(sizeof(list_adj));
    l->head = NULL;
    return l;
}

void add_head(list_adj *l, int end_edge, float weight) {
    cell *new_cell = create_cell(end_edge, weight);
    new_cell->next = l->head;
    l->head = new_cell;
}

graph *create_empty_graph(int num_edges) {
    graph *g = malloc(sizeof(graph));
    g->num_edges = num_edges;
    g->edges = malloc(num_edges * sizeof(list_adj));
    for (int i = 0; i < num_edges; i++) g->edges[i].head = NULL;
    return g;
}

void print_list(list_adj *l) {
    cell *c = l->head;
    while (c) {
        printf("(%d %.2f) -> ", c->end_edge, c->weight);
        c = c->next;
    }
    printf("NULL\n");
}

void print_graph(graph *g) {
    for (int i = 0; i < g->num_edges; i++) {
        printf("Sommet %d: ", i + 1);
        print_list(&g->edges[i]);
    }
}

graph *read_graph_from_file(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (!file) { perror("open"); exit(EXIT_FAILURE); }

    int nbvert, depart, arrivee;
    float proba;

    if (fscanf(file, "%d", &nbvert) != 1) { perror("read nbvert"); exit(EXIT_FAILURE); }
    graph *g = create_empty_graph(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3)
        add_head(&g->edges[depart - 1], arrivee, proba);

    fclose(file);
    return g;
}


int is_markov_graph(graph *g) {
    int valid = 1;
    for (int i = 0; i < g->num_edges; i++) {
        cell *c = g->edges[i].head;
        float sum = 0;
        while (c) { sum += c->weight; c = c->next; }
        if (sum < 0.99 || sum > 1.01) {
            printf("Sommet %d invalide, somme=%.2f\n", i + 1, sum);
            valid = 0;
        }
    }
    if (valid) printf("Le graphe est un graphe de Markov\n");
    else printf("Le graphe n'est pas un graphe de Markov\n");
    return valid;
}


static char *getID(int i) {
    static char buffer[10];
    char temp[10];
    int index = 0;
    i--;
    while (i >= 0) {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }
    for (int j = 0; j < index; j++) buffer[j] = temp[index - j - 1];
    buffer[index] = '\0';
    return buffer;
}

void write_mermaid_file(graph *g, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (!f) {
        perror("Cannot open file for writing");
        return;
    }

    fprintf(f, "---\nconfig:\nlayout: elk\ntheme: neo\nlook: neo\n---\nflowchart LR\n");

    for (int i = 0; i < g->num_edges; i++)
        fprintf(f, "%s((%d))\n", getID(i + 1), i + 1);

    for (int i = 0; i < g->num_edges; i++) {
        char from[10];
        strcpy(from, getID(i + 1));

        cell *c = g->edges[i].head;
        while (c) {
            fprintf(f, "%s -->|%.2f|%s\n", from, c->weight, getID(c->end_edge));
            c = c->next;
        }
    }

    fclose(f);
}

 
stack* create_stack(int capacity) {
    stack *s = malloc(sizeof(stack));
    s->data = malloc(capacity * sizeof(int));
    s->top = -1;
    s->capacity = capacity;
    return s;
}

void push(stack *s, int value) {
    if (s->top >= s->capacity - 1) {
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity * sizeof(int));
    }
    s->data[++s->top] = value;
}

int pop(stack *s) {
    if (s->top < 0) {
        fprintf(stderr, "Erreur: pile vide\n");
        exit(EXIT_FAILURE);
    }
    return s->data[s->top--];
}

int peek(stack *s) {
    if (s->top < 0) return -1;
    return s->data[s->top];
}

int is_empty(stack *s) {
    return s->top < 0;
}

void free_stack(stack *s) {
    free(s->data);
    free(s);
}

 
classe* create_classe(const char *name) {
    classe *c = malloc(sizeof(classe));
    c->name = malloc(strlen(name) + 1);
    strcpy(c->name, name);
    c->capacity = 10;
    c->vertices = malloc(c->capacity * sizeof(tarjan_vertex));
    c->nb_vertices = 0;
    return c;
}

void add_vertex_to_classe(classe *c, tarjan_vertex v) {
    if (c->nb_vertices >= c->capacity) {
        c->capacity *= 2;
        c->vertices = realloc(c->vertices, c->capacity * sizeof(tarjan_vertex));
    }
    c->vertices[c->nb_vertices++] = v;
}

void print_classe(classe *c) {
    printf("Composante %s: {", c->name);
    for (int i = 0; i < c->nb_vertices; i++) {
        printf("%d", c->vertices[i].id);
        if (i < c->nb_vertices - 1) printf(",");
    }
    printf("}\n");
}

void free_classe(classe *c) {
    free(c->name);
    free(c->vertices);
    free(c);
}

 
partition* create_partition() {
    partition *p = malloc(sizeof(partition));
    p->capacity = 10;
    p->classes = malloc(p->capacity * sizeof(classe));
    p->nb_classes = 0;
    return p;
}

void add_classe_to_partition(partition *p, classe *c) {
    if (p->nb_classes >= p->capacity) {
        p->capacity *= 2;
        p->classes = realloc(p->classes, p->capacity * sizeof(classe));
    }
    p->classes[p->nb_classes++] = *c;
}

void print_partition(partition *p) {
    printf("\n=== PARTITION DU GRAPHE ===\n");
    for (int i = 0; i < p->nb_classes; i++) {
        print_classe(&p->classes[i]);
    }
    printf("Nombre total de classes : %d\n", p->nb_classes);
}

void free_partition(partition *p) {
    for (int i = 0; i < p->nb_classes; i++) {
        free(p->classes[i].name);
        free(p->classes[i].vertices);
    }
    free(p->classes);
    free(p);
}

 
tarjan_vertex* init_tarjan_vertices(graph *g) {
    tarjan_vertex *vertices = malloc(g->num_edges * sizeof(tarjan_vertex));
    for (int i = 0; i < g->num_edges; i++) {
        vertices[i].id = i + 1;
        vertices[i].num = -1;
        vertices[i].low = -1;
        vertices[i].in_stack = 0;
    }
    return vertices;
}

void tarjan_parcours(int v, graph *g, tarjan_vertex *vertices, stack *s, int *num_counter, partition *part) {
    vertices[v].num = *num_counter;
    vertices[v].low = *num_counter;
    (*num_counter)++;

    push(s, v);
    vertices[v].in_stack = 1;

    cell *current = g->edges[v].head;
    while (current) {
        int w = current->end_edge - 1; // Convertir en index (base 0)

        if (vertices[w].num == -1) {
            tarjan_parcours(w, g, vertices, s, num_counter, part);
            vertices[v].low = min(vertices[v].low, vertices[w].low);
        } else if (vertices[w].in_stack) {
            vertices[v].low = min(vertices[v].low, vertices[w].num);
        }

        current = current->next;
    }

    if (vertices[v].low == vertices[v].num) {
        char name[20];
        sprintf(name, "C%d", part->nb_classes + 1);
        classe *c = create_classe(name);

        int w;
        do {
            w = pop(s);
            vertices[w].in_stack = 0;
            add_vertex_to_classe(c, vertices[w]);
        } while (w != v);

        add_classe_to_partition(part, c);
        free(c);
    }
}

partition* tarjan(graph *g) {
    tarjan_vertex *vertices = init_tarjan_vertices(g);
    stack *s = create_stack(g->num_edges);
    partition *part = create_partition();
    int num_counter = 0;

    for (int v = 0; v < g->num_edges; v++) {
        if (vertices[v].num == -1) {
            tarjan_parcours(v, g, vertices, s, &num_counter, part);
        }
    }

    free_stack(s);
    free(vertices);
    return part;
}

 
void free_list(list_adj *l) {
    cell *current = l->head;
    while (current) {
        cell *next = current->next;
        free(current);
        current = next;
    }
}

void free_graph(graph *g) {
    if (!g) return;
    for (int i = 0; i < g->num_edges; i++) {
        free_list(&g->edges[i]);
    }
    free(g->edges);
    free(g);
}


