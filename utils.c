#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


cell *create_cell(int end_edge, float weight){
    cell *c = malloc(sizeof(cell));
    c->end_edge = end_edge;
    c->weight = weight;
    c->next = NULL;
    return c;
}

list *create_empty_list(){
    list *l = malloc(sizeof(list));
    l->head = NULL;
    return l;
}

void add_head(list *l, int end_edge, float weight){
    cell *new_cell = create_cell(end_edge, weight);
    new_cell->next = l->head;
    l->head = new_cell;
}

graph *create_empty_graph(int num_edges){
    graph *g = malloc(sizeof(graph));
    g->edges = (list *)malloc(num_edges * sizeof(list *));
    for (int i = 0; i < num_edges; i++){
        g->edges[i] = create_empty_list();
    }
    g->num_edges = num_edges;
    return g;
}

void print_list(list *l){
    cell *c = l->head;
    printf("[head %p] ", l->head);
    while (c != NULL)
    {
        printf("(%d %f) %p->", c->end_edge, c->weight, c);
        c = c->next;
    }
}

void print_graph(graph *g)
{
    for (int i = 0; i < g->num_edges; i++)
    {
        printf("Liste pour le sommet %d: ", i);
        print_list(g->edges[i]);
    }
}

graph *readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt"); // read-only, text
    int nbvert, depart, arrivee;
    float proba;
    graph *g;
    if (file==NULL)
    {
    perror("Could not open file for reading");
    exit(EXIT_FAILURE);
    }
    // first line contains number of vertices
    if (fscanf(file, "%d", &nbvert) != 1)
    {
    perror("Could not read number of vertices");
    exit(EXIT_FAILURE);
    }
    g = create_empty_graph(nbvert);
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3)
    {
    // on obtient, pour chaque ligne du fichier les valeurs
    // depart, arrivee, et proba
    add_head(g->edges[depart], arrivee, proba);
    }
    fclose(file);
    return g;
   }


static char *getID(int i) {
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}
