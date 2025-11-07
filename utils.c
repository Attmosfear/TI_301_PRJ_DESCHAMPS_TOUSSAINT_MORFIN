#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"

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
    // Ouvre le fichier en lecture texte
    FILE *file = fopen(filename, "rt");
    if (!file) { perror("open"); exit(EXIT_FAILURE); }

    int nbvert, depart, arrivee;
    float proba;

    // Lit le nombre de sommets (première ligne)
    if (fscanf(file, "%d", &nbvert) != 1) { perror("read nbvert"); exit(EXIT_FAILURE); }
    graph *g = create_empty_graph(nbvert);

    // Lit chaque ligne suivante : départ, arrivée, probabilité
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3)
        add_head(&g->edges[depart - 1], arrivee, proba);

    fclose(file);
    return g;
}


int is_markov_graph(graph *g) {
    int valid = 1;
    // Parcourt tous les sommets
    for (int i = 0; i < g->num_edges; i++) {
        cell *c = g->edges[i].head;
        float sum = 0;
        // Calcule la somme des probabilités sortantes
        while (c) { sum += c->weight; c = c->next; }
        // Vérifie si la somme est proche de 1
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
    // en tête de config
    fprintf(f, "---\nconfig:\nlayout: elk\ntheme: neo\nlook: neo\n---\nflowchart LR\n");
    // Définit les sommets
    for (int i = 0; i < g->num_edges; i++)
        fprintf(f, "%s((%d))\n", getID(i + 1), i + 1);
    // Ajoute les arêtes avec leurs probabilités
    for (int i = 0; i < g->num_edges; i++) {
        cell *c = g->edges[i].head;
        while (c) {
            fprintf(f, "%s -->|%.2f|%s\n", getID(i + 1), c->weight, getID(c->end_edge));
            c = c->next;
        }
    }
    fclose(f);
}


