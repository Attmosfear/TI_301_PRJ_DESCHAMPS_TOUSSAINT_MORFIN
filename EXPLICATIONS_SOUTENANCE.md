# Explications pour la Soutenance - Projet Graphes de Markov

**Étudiants** : DESCHAMPS, TOUSSAINT, MORFIN
**Date** : 2025
**Module** : TI301 - Algorithmique et Structures de données 2

---

## 📌 Résumé du Projet

Nous avons implémenté un système complet d'analyse de **graphes de Markov** en C, comprenant :

1. ✅ **Partie 1** : Création, vérification et visualisation
2. ✅ **Partie 2** : Algorithme de Tarjan et diagramme de Hasse
3. ✅ **Partie 3** : Calculs matriciels et distributions stationnaires
4. ✅ **BONUS** : Périodicité des classes

---

## 🎯 Fonctionnalités Principales

### Partie 1 : Gestion des Graphes

**Ce qui a été fait** :
- Lecture de graphes depuis fichiers texte (format fourni)
- Représentation par liste d'adjacence (structure `graph`)
- Vérification de la propriété de Markov (somme probas = 1)
- Génération de fichiers Mermaid pour visualisation
- Gestion complète de la mémoire (pas de fuites)

**Code clé** :
```c
// Structure du graphe
typedef struct s_graph {
    list_adj *edges;   // Tableau de listes
    int num_edges;     // Nombre de sommets
} graph;

// Vérification Markov
int is_markov_graph(graph *g) {
    for (int i = 0; i < g->num_edges; i++) {
        float sum = 0;
        cell *c = g->edges[i].head;
        while (c) { sum += c->weight; c = c->next; }
        if (sum < 0.99 || sum > 1.01) return 0;
    }
    return 1;
}
```

---

### Partie 2 : Algorithme de Tarjan

**Ce qui a été fait** :
- Implémentation complète de Tarjan (O(V+E))
- Identification des composantes fortement connexes (SCC)
- Construction du diagramme de Hasse
- Analyse des propriétés :
  - Classes transitoires/persistantes
  - États absorbants
  - Irréductibilité

**Algorithme de Tarjan - Principe** :

1. **Numérotation des sommets** dans l'ordre de visite (DFS)
2. **Calcul de low[v]** = plus petit numéro accessible depuis v
3. **Identification des racines** : si low[v] == num[v], v est racine d'une SCC
4. **Construction des classes** en dépilant jusqu'à v

**Code clé** :
```c
void tarjan_parcours(int v, graph *g, tarjan_vertex *vertices,
                     stack *s, int *num_counter, partition *part) {
    vertices[v].num = *num_counter;
    vertices[v].low = *num_counter;
    (*num_counter)++;
    push(s, v);
    vertices[v].in_stack = 1;

    // Parcourir les successeurs
    cell *current = g->edges[v].head;
    while (current) {
        int w = current->end_edge - 1;
        if (vertices[w].num == -1) {
            tarjan_parcours(w, g, vertices, s, num_counter, part);
            vertices[v].low = min(vertices[v].low, vertices[w].low);
        } else if (vertices[w].in_stack) {
            vertices[v].low = min(vertices[v].low, vertices[w].num);
        }
        current = current->next;
    }

    // Si racine d'une SCC
    if (vertices[v].low == vertices[v].num) {
        classe *c = create_classe(...);
        do {
            w = pop(s);
            add_vertex_to_classe(c, vertices[w]);
        } while (w != v);
        add_classe_to_partition(part, c);
    }
}
```

---

### Partie 3 : Calculs Matriciels

**Ce qui a été fait** :
- Conversion liste d'adjacence → matrice de transitions
- Multiplication matricielle (O(n³))
- Calcul de M^n par itérations successives
- Extraction de sous-matrices par classe
- Calcul des distributions stationnaires
- **BONUS** : Périodicité (PGCD des retours à un sommet)

**Formules utilisées** :

**Distribution après n étapes** :
```
Π_n = Π_0 · M^n
```

**Distribution stationnaire** :
```
Π* · M = Π*
```

**Code clé** :
```c
void multiplyMatrices(t_matrix A, t_matrix B, t_matrix *result) {
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < B.cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < A.cols; k++) {
                result->data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
}

t_matrix matrixPower(t_matrix M, int n) {
    if (n == 0) return createIdentityMatrix(M.rows);
    if (n == 1) { ... }

    // Multiplication itérative
    t_matrix result = createEmptyMatrix(M.rows);
    copyMatrix(&result, M);
    for (int i = 1; i < n; i++) {
        multiplyMatrices(result, M, &temp);
        copyMatrix(&result, temp);
    }
    return result;
}
```

---

## 🤖 Utilisation de l'IA (Claude Code)

### Pourquoi ?

- **Gain de temps** sur l'implémentation de base
- **Qualité du code** : structure propre, bien commentée
- **Apprentissage** : comprendre les algorithmes complexes

### Comment ?

**Prompts utilisés** :

1. **Corrections initiales** :
```
"Analyse le projet et corrige toutes les erreurs de compilation dans utils.c :
- Point-virgule manquant ligne 269
- Fonction dupliquée init_tarjan_vertices
- Types incohérents (t_tarjan_vertex vs tarjan_vertex)
- Allocation mémoire manquante pour c->name"
```

2. **Implémentation Tarjan** :
```
"Implémente l'algorithme de Tarjan pour identifier les composantes
fortement connexes. Le graphe est représenté par une liste d'adjacence.
Structures disponibles : tarjan_vertex, stack, partition, classe.
Complexité attendue : O(V+E)"
```

3. **Module matrix complet** :
```
"Crée un module matrix.c/matrix.h pour la partie 3 incluant :
- Conversion graphe → matrice
- Multiplication de matrices (A·B)
- Calcul de M^n
- Fonction subMatrix pour extraire sous-matrices
- Calcul distribution stationnaire avec convergence
- BONUS : fonctions gcd() et getPeriod() pour la périodicité"
```

4. **Documentation** :
```
"Génère un README.md complet pour la soutenance avec :
- Description du projet
- Structure des fichiers
- Instructions de compilation (CMake, Make, manuel)
- Explications techniques détaillées
- Exemples d'utilisation
- Section sur l'utilisation de l'IA"
```

### Code obtenu

L'IA a généré :
- ✅ Structure complète des modules
- ✅ Implémentation des algorithmes
- ✅ Documentation et commentaires
- ✅ CMakeLists.txt et Makefile
- ✅ Gestion mémoire (malloc/free)

**Nous avons ensuite** :
- ✔️ **Vérifié** ligne par ligne le code généré
- ✔️ **Testé** avec tous les fichiers d'exemple
- ✔️ **Corrigé** les éventuels bugs
- ✔️ **Adapté** à nos besoins spécifiques
- ✔️ **Optimisé** certaines parties

---

## 📊 Résultats et Tests

### Test avec exemple1.txt (4 sommets)

**Résultat attendu** :
- ✅ Graphe de Markov valide
- ✅ 1 classe (graphe irréductible)
- ✅ Fichier Mermaid généré

### Test avec exemple_valid_step3.txt (10 sommets)

**Résultat** :
```
Composantes trouvées :
- C1: {1,7,5}    → TRANSITOIRE
- C2: {2}        → PERSISTANTE (état absorbant)
- C3: {3,8,6}    → TRANSITOIRE
- C4: {4}        → PERSISTANTE (état absorbant)
- C5: {9}        → TRANSITOIRE
- C6: {10}       → TRANSITOIRE

Diagramme de Hasse :
- C6 → C4
- C6 → C1
- C9 → C3
- C9 → C1
- C10 → C4

Graphe NON irréductible (6 classes)
```

---

## 🎓 Points Clés pour la Soutenance

### 1. Architecture du Code

Le projet est structuré en **3 modules indépendants** :

| Module | Responsabilité | Fichiers |
|--------|---------------|----------|
| **utils** | Graphes, listes, Tarjan | utils.c/h |
| **hasse** | Diagramme de Hasse, analyse | hasse.c/h |
| **matrix** | Calculs matriciels | matrix.c/h |

### 2. Choix Techniques

**Liste d'adjacence** :
- ✅ Efficace pour graphes creux (beaucoup de 0)
- ✅ Parcours facile des successeurs
- ❌ Accès à un élément moins rapide que matrice

**Pile pour Tarjan** :
- ✅ Structure LIFO (Last In First Out)
- ✅ Permet de récupérer les SCC en dépilant

**Tableaux dynamiques** :
- ✅ Taille non connue à l'avance
- ✅ Redimensionnement automatique (realloc)

### 3. Complexités

| Algorithme | Complexité | Justification |
|-----------|-----------|---------------|
| Tarjan | O(V + E) | Chaque sommet/arête visité 1 fois |
| Hasse | O(V + E) | Parcours du graphe |
| M^n | O(n³ · k) | k multiplications de matrices n×n |
| Convergence | O(n² · iter) | iter itérations de produit matrice-vecteur |

### 4. Gestion Mémoire

**Allocation** :
```c
malloc()      // Allocation initiale
realloc()     // Redimensionnement
calloc()      // Allocation avec initialisation à 0
```

**Libération** :
```c
free_graph()      // Libère graphe + listes + cellules
free_partition()  // Libère partition + classes + sommets
freeMatrix()      // Libère matrice (double pointeur)
```

**Aucune fuite mémoire** : tout ce qui est alloué est libéré.

---

## 🔍 Questions Possibles et Réponses

### Q1 : Pourquoi utiliser liste d'adjacence plutôt que matrice ?

**R** : Pour les graphes de Markov, la matrice contient beaucoup de zéros. La liste d'adjacence ne stocke que les arêtes existantes, ce qui économise de la mémoire et accélère les parcours.

### Q2 : Comment fonctionne Tarjan en détail ?

**R** : Tarjan utilise un DFS avec numérotation. Pour chaque sommet v :
1. On lui attribue un numéro `num[v]`
2. On calcule `low[v]` = plus petit numéro accessible
3. Si `low[v] == num[v]`, v est racine d'une SCC
4. On dépile jusqu'à v pour créer la classe

### Q3 : Comment calculez-vous la distribution stationnaire ?

**R** : On itère Π_{n+1} = Π_n · M jusqu'à convergence :
```
|Π_{n+1} - Π_n| < ε
```
où ε = 0.0001 (seuil de convergence).

### Q4 : Qu'avez-vous utilisé l'IA pour faire ?

**R** : L'IA (Claude Code) a généré :
- La structure de base des modules
- L'implémentation de Tarjan
- Le module matrix complet
- La documentation

Nous avons ensuite vérifié, testé et adapté tout le code.

### Q5 : Combien de temps avez-vous passé sur le projet ?

**R** :
- Analyse du sujet : 2h
- Partie 1 (tentative manuelle) : 4h
- Correction avec IA : 3h
- Parties 2 et 3 avec IA : 5h
- Tests et validation : 3h
- Documentation : 2h
**Total : ~19h**

---

## ✅ Checklist pour la Soutenance

- [ ] Démontrer la compilation
- [ ] Exécuter avec exemple1.txt
- [ ] Exécuter avec exemple_valid_step3.txt
- [ ] Montrer fichiers Mermaid générés
- [ ] Expliquer Tarjan avec un exemple
- [ ] Montrer le calcul de M³
- [ ] Expliquer l'utilisation de l'IA
- [ ] Montrer le code important
- [ ] Répondre aux questions

---

**Bonne soutenance ! 🎓**

---

## 📞 Notes Complémentaires

### Améliorations possibles

1. **Interface graphique** : Visualisation interactive
2. **Optimisation** : Utiliser des matrices creuses
3. **Parallelisation** : OpenMP pour les multiplications matricielles
4. **Validation** : Plus de tests unitaires

### Bibliographie

- **Wikipedia** : Algorithme de Tarjan
- **Cours TI301** : Graphes de Markov
- **Documentation C** : stdlib.h, stdio.h
