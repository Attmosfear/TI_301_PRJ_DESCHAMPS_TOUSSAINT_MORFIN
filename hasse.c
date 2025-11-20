#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hasse.h"
#include "utils.h"

// ============= FONCTIONS POUR LES LIENS =============

link_array* create_link_array() {
    link_array *la = malloc(sizeof(link_array));
    la->capacity = 10;
    la->links = malloc(la->capacity * sizeof(link));
    la->nb_links = 0;
    return la;
}

void add_link(link_array *la, int from, int to) {
    // Vérifier si le lien existe déjà
    if (link_exists(la, from, to)) return;

    // Agrandir le tableau si nécessaire
    if (la->nb_links >= la->capacity) {
        la->capacity *= 2;
        la->links = realloc(la->links, la->capacity * sizeof(link));
    }

    la->links[la->nb_links].from_classe = from;
    la->links[la->nb_links].to_classe = to;
    la->nb_links++;
}

int link_exists(link_array *la, int from, int to) {
    for (int i = 0; i < la->nb_links; i++) {
        if (la->links[i].from_classe == from && la->links[i].to_classe == to) {
            return 1;
        }
    }
    return 0;
}

void free_link_array(link_array *la) {
    free(la->links);
    free(la);
}

// ============= CRÉATION DU TABLEAU SOMMET -> CLASSE =============

int* create_vertex_to_class_map(partition *part, int num_vertices) {
    int *map = malloc(num_vertices * sizeof(int));

    // Initialiser à -1 (aucune classe)
    for (int i = 0; i < num_vertices; i++) {
        map[i] = -1;
    }

    // Pour chaque classe, associer ses sommets
    for (int c = 0; c < part->nb_classes; c++) {
        classe *current_class = &part->classes[c];
        for (int v = 0; v < current_class->nb_vertices; v++) {
            int vertex_id = current_class->vertices[v].id;
            map[vertex_id - 1] = c;  // Convertir en index base 0
        }
    }

    return map;
}

// ============= CRÉATION DU DIAGRAMME DE HASSE =============

link_array* create_hasse_diagram(graph *g, partition *part) {
    link_array *links = create_link_array();
    int *vertex_to_class = create_vertex_to_class_map(part, g->num_edges);

    // Pour chaque sommet du graphe
    for (int i = 0; i < g->num_edges; i++) {
        int Ci = vertex_to_class[i];  // Classe du sommet i

        // Parcourir tous les successeurs de i
        cell *current = g->edges[i].head;
        while (current) {
            int j = current->end_edge - 1;  // Convertir en index base 0
            int Cj = vertex_to_class[j];    // Classe du sommet j

            // Si les classes sont différentes, ajouter le lien
            if (Ci != Cj) {
                add_link(links, Ci, Cj);
            }

            current = current->next;
        }
    }

    free(vertex_to_class);
    return links;
}

// ============= SUPPRESSION DES LIENS TRANSITIFS (OPTIONNEL) =============

void removeTransitiveLinks(link_array *p_link_array) {
    if (!p_link_array || p_link_array->nb_links == 0) return;

    // Créer une matrice d'accessibilité
    int max_class = 0;
    for (int i = 0; i < p_link_array->nb_links; i++) {
        if (p_link_array->links[i].from_classe > max_class)
            max_class = p_link_array->links[i].from_classe;
        if (p_link_array->links[i].to_classe > max_class)
            max_class = p_link_array->links[i].to_classe;
    }
    max_class++;

    // Matrice d'accessibilité directe
    int **direct = calloc(max_class, sizeof(int*));
    for (int i = 0; i < max_class; i++) {
        direct[i] = calloc(max_class, sizeof(int));
    }

    // Remplir la matrice directe
    for (int i = 0; i < p_link_array->nb_links; i++) {
        int from = p_link_array->links[i].from_classe;
        int to = p_link_array->links[i].to_classe;
        direct[from][to] = 1;
    }

    // Calculer la fermeture transitive avec Floyd-Warshall
    int **transitive = calloc(max_class, sizeof(int*));
    for (int i = 0; i < max_class; i++) {
        transitive[i] = calloc(max_class, sizeof(int));
        for (int j = 0; j < max_class; j++) {
            transitive[i][j] = direct[i][j];
        }
    }

    for (int k = 0; k < max_class; k++) {
        for (int i = 0; i < max_class; i++) {
            for (int j = 0; j < max_class; j++) {
                if (transitive[i][k] && transitive[k][j]) {
                    transitive[i][j] = 1;
                }
            }
        }
    }

    // Supprimer les liens transitifs
    link_array *new_links = create_link_array();
    for (int i = 0; i < p_link_array->nb_links; i++) {
        int from = p_link_array->links[i].from_classe;
        int to = p_link_array->links[i].to_classe;
        int is_transitive = 0;

        // Vérifier s'il existe un chemin intermédiaire
        for (int k = 0; k < max_class; k++) {
            if (k != from && k != to && direct[from][k] && transitive[k][to]) {
                is_transitive = 1;
                break;
            }
        }

        if (!is_transitive) {
            add_link(new_links, from, to);
        }
    }

    // Copier les nouveaux liens
    free(p_link_array->links);
    p_link_array->links = new_links->links;
    p_link_array->nb_links = new_links->nb_links;
    p_link_array->capacity = new_links->capacity;
    free(new_links);

    // Libérer les matrices
    for (int i = 0; i < max_class; i++) {
        free(direct[i]);
        free(transitive[i]);
    }
    free(direct);
    free(transitive);
}

// ============= GÉNÉRATION DU FICHIER MERMAID =============

void write_hasse_mermaid(partition *part, link_array *links, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (!f) {
        perror("Cannot open file for writing");
        return;
    }

    // En-tête
    fprintf(f, "---\nconfig:\nlayout: elk\ntheme: neo\nlook: neo\n---\nflowchart TB\n");

    // Définir les classes (nœuds)
    for (int i = 0; i < part->nb_classes; i++) {
        fprintf(f, "%s[", part->classes[i].name);

        // Afficher les sommets de la classe
        fprintf(f, "{");
        for (int v = 0; v < part->classes[i].nb_vertices; v++) {
            fprintf(f, "%d", part->classes[i].vertices[v].id);
            if (v < part->classes[i].nb_vertices - 1) fprintf(f, ",");
        }
        fprintf(f, "}]\n");
    }

    // Ajouter les liens
    for (int i = 0; i < links->nb_links; i++) {
        fprintf(f, "%s --> %s\n",
                part->classes[links->links[i].from_classe].name,
                part->classes[links->links[i].to_classe].name);
    }

    fclose(f);
}

// ============= ANALYSE DES PROPRIÉTÉS DU GRAPHE =============

void analyze_graph_properties(partition *part, link_array *links) {
    printf("\n=== ANALYSE DES PROPRIÉTÉS DU GRAPHE ===\n\n");

    // Déterminer si chaque classe est transitoire ou persistante
    int *has_outgoing_link = calloc(part->nb_classes, sizeof(int));

    for (int i = 0; i < links->nb_links; i++) {
        has_outgoing_link[links->links[i].from_classe] = 1;
    }

    printf("Classification des classes:\n");
    for (int i = 0; i < part->nb_classes; i++) {
        printf("  %s: ", part->classes[i].name);

        if (has_outgoing_link[i]) {
            printf("TRANSITOIRE (on peut sortir de cette classe)\n");
        } else {
            printf("PERSISTANTE (on ne peut pas sortir de cette classe)\n");

            // Vérifier si c'est un état absorbant
            if (part->classes[i].nb_vertices == 1) {
                printf("    -> État %d est ABSORBANT\n", part->classes[i].vertices[0].id);
            }
        }
    }

    // Vérifier si le graphe est irréductible
    printf("\nIrréductibilité:\n");
    if (part->nb_classes == 1) {
        printf("  Le graphe est IRRÉDUCTIBLE (une seule classe)\n");
    } else {
        printf("  Le graphe n'est PAS irréductible (%d classes)\n", part->nb_classes);
    }

    free(has_outgoing_link);
}
