#ifndef __HASSE_H__
#define __HASSE_H__

#include "utils.h"

// Structure pour représenter un lien entre deux classes
typedef struct s_link {
    int from_classe;  // Index de la classe de départ
    int to_classe;    // Index de la classe d'arrivée
} link;

// Structure pour stocker un tableau de liens
typedef struct s_link_array {
    link *links;
    int nb_links;
    int capacity;
} link_array;

// Fonctions pour les liens
link_array* create_link_array();
void add_link(link_array *la, int from, int to);
int link_exists(link_array *la, int from, int to);
void free_link_array(link_array *la);

// Création du tableau qui associe chaque sommet à sa classe
int* create_vertex_to_class_map(partition *part, int num_vertices);

// Création du diagramme de Hasse (liens entre classes)
link_array* create_hasse_diagram(graph *g, partition *part);

// Suppression des liens transitifs (optionnel)
void removeTransitiveLinks(link_array *p_link_array);

// Génération du fichier Mermaid pour le diagramme de Hasse
void write_hasse_mermaid(partition *part, link_array *links, const char *filename);

// Détermination des caractéristiques du graphe
void analyze_graph_properties(partition *part, link_array *links);

#endif
