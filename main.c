#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "hasse.h"
#include "matrix.h"

void print_distribution(float *dist, int size) {
    printf("Distribution: [ ");
    for (int i = 0; i < size; i++) {
        printf("%.4f ", dist[i]);
    }
    printf("]\n");
}

int main() {
    char name[100], filename[200];

    printf("========================================\n");
    printf("  PROJET GRAPHES DE MARKOV - TI301\n");
    printf("========================================\n\n");

    printf("Nom du fichier à lire dans le dossier data : ");
    scanf("%99s", name);
    strcpy(filename, "data/");
    strcat(filename, name);

    // ========== PARTIE 1 : Création et vérification du graphe ==========
    printf("\n=== PARTIE 1 : CRÉATION ET VÉRIFICATION ===\n");

    graph *g = read_graph_from_file(filename);
    printf("\nGraphe chargé depuis %s\n", filename);
    printf("Nombre de sommets : %d\n\n", g->num_edges);

    printf("Affichage du graphe (liste d'adjacence) :\n");
    print_graph(g);

    printf("\nVérification du graphe de Markov :\n");
    is_markov_graph(g);

    printf("\nGénération du fichier Mermaid (graph.mmd)...\n");
    write_mermaid_file(g, "graph.mmd");
    printf("✓ Fichier graph.mmd généré avec succès\n");

    // ========== PARTIE 2 : Algorithme de Tarjan et Hasse ==========
    printf("\n=== PARTIE 2 : ALGORITHME DE TARJAN ===\n");

    partition *part = tarjan(g);
    print_partition(part);

    printf("\n=== PARTIE 2 : DIAGRAMME DE HASSE ===\n");

    link_array *links = create_hasse_diagram(g, part);
    printf("\nNombre de liens entre classes : %d\n", links->nb_links);

    printf("\nLiens entre classes :\n");
    for (int i = 0; i < links->nb_links; i++) {
        printf("  %s -> %s\n",
               part->classes[links->links[i].from_classe].name,
               part->classes[links->links[i].to_classe].name);
    }

    printf("\nGénération du fichier Mermaid (hasse.mmd)...\n");
    write_hasse_mermaid(part, links, "hasse.mmd");
    printf("✓ Fichier hasse.mmd généré avec succès\n");

    // Analyse des propriétés
    analyze_graph_properties(part, links);

    // ========== PARTIE 3 : CALCULS MATRICIELS ==========
    printf("\n=== PARTIE 3 : CALCULS MATRICIELS ===\n");

    // Créer la matrice de transitions
    t_matrix M = graphToMatrix(g);
    printf("\nMatrice de transitions M :\n");
    printMatrix(M);

    // Calculer M^3
    printf("Calcul de M^3 :\n");
    t_matrix M3 = matrixPower(M, 3);
    printMatrix(M3);

    // Calculer M^7
    printf("Calcul de M^7 :\n");
    t_matrix M7 = matrixPower(M, 7);
    printMatrix(M7);

    // Recherche de la distribution stationnaire
    printf("Recherche de la distribution stationnaire avec epsilon=0.01 :\n");
    t_matrix M_prev = createEmptyMatrix(M.rows);
    t_matrix M_curr = createEmptyMatrix(M.rows);
    copyMatrix(&M_curr, M);

    int n = 1;
    float epsilon = 0.01f;
    float difference;

    do {
        copyMatrix(&M_prev, M_curr);
        t_matrix M_temp = createEmptyMatrix(M.rows);
        multiplyMatrices(M_curr, M, &M_temp);
        copyMatrix(&M_curr, M_temp);
        freeMatrix(&M_temp);

        difference = diff(M_curr, M_prev);
        n++;
    } while (difference >= epsilon && n < 1000);

    printf("M^%d (différence < %.2f) :\n", n, epsilon);
    printMatrix(M_curr);

    // Distribution stationnaire pour chaque classe persistante
    printf("\n=== DISTRIBUTIONS STATIONNAIRES PAR CLASSE ===\n");
    for (int i = 0; i < part->nb_classes; i++) {
        // Vérifier si la classe est persistante
        int is_persistent = 1;
        for (int j = 0; j < links->nb_links; j++) {
            if (links->links[j].from_classe == i) {
                is_persistent = 0;
                break;
            }
        }

        if (is_persistent && part->classes[i].nb_vertices > 0) {
            printf("\nClasse %s (PERSISTANTE) :\n", part->classes[i].name);
            t_matrix sub = subMatrix(M, *part, i);

            printf("Sous-matrice de la classe :\n");
            printMatrix(sub);

            float *stationary = findStationaryDistribution(sub, 0.0001f, 10000);
            printf("Distribution stationnaire : ");
            print_distribution(stationary, sub.rows);

            free(stationary);
            freeMatrix(&sub);
        }
    }

    // ========== BONUS : PÉRIODICITÉ ==========
    printf("\n=== BONUS : PÉRIODICITÉ DES CLASSES ===\n");

    for (int i = 0; i < part->nb_classes; i++) {
        if (part->classes[i].nb_vertices > 0) {
            t_matrix sub = subMatrix(M, *part, i);
            int period = getPeriod(sub);
            printf("Classe %s : période = %d\n", part->classes[i].name, period);
            freeMatrix(&sub);
        }
    }

    // ========== LIBÉRATION MÉMOIRE ==========
    freeMatrix(&M);
    freeMatrix(&M3);
    freeMatrix(&M7);
    freeMatrix(&M_prev);
    freeMatrix(&M_curr);
    free_link_array(links);
    free_partition(part);
    free_graph(g);

    printf("\n========================================\n");
    printf("  Programme terminé avec succès !\n");
    printf("========================================\n");

    return 0;
}
