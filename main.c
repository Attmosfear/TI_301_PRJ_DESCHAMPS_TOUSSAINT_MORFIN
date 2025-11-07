#include <stdio.h>
#include <string.h>
#include "utils.h"

int main() {
    char name[100], filename[200];
    printf("Nom du fichier à lire dans le dossier data : ");
    scanf("%99s", name);
    strcpy(filename, "data/");
    strcat(filename, name);
    graph *g = read_graph_from_file(filename);
    print_graph(g);
    is_markov_graph(g);
    write_mermaid_file(g, "graph.mmd");
    return 0;
}
