# Projet Graphes de Markov - TI301

**Étudiants** : DESCHAMPS, TOUSSAINT, MORFIN
**Module** : TI301 - Algorithmique et Structures de données 2
**Sujet** : Étude de Graphes de Markov

## 📋 Table des matières

1. [Description du projet](#description-du-projet)
2. [Structure du projet](#structure-du-projet)
3. [Compilation et exécution](#compilation-et-exécution)
4. [Fonctionnalités implémentées](#fonctionnalités-implémentées)
5. [Explications pour la soutenance](#explications-pour-la-soutenance)
6. [Exemples d'utilisation](#exemples-dutilisation)
7. [Aide à l'IA](#aide-à-lia)

---

## 📖 Description du projet

Ce projet implémente un système complet d'analyse de **graphes de Markov** (chaînes de Markov à temps discret) en langage C. Il permet de :

- Charger et vérifier des graphes de Markov depuis des fichiers
- Visualiser les graphes avec l'outil Mermaid
- Identifier les composantes fortement connexes (algorithme de Tarjan)
- Construire et analyser le diagramme de Hasse
- Calculer les distributions stationnaires
- Déterminer la périodicité des classes

---

## 📁 Structure du projet

```
TI_301_PRJ_DESCHAMPS_TOUSSAINT_MORFIN/
├── main.c              # Programme principal
├── utils.c/utils.h     # Fonctions pour graphes, listes, Tarjan
├── hasse.c/hasse.h     # Diagramme de Hasse et analyse
├── matrix.c/matrix.h   # Calculs matriciels et distributions
├── CMakeLists.txt      # Fichier de configuration CMake
├── Makefile            # Makefile alternatif
├── README.md           # Ce fichier
├── data/               # Fichiers de test (.txt)
└── Sujet/              # PDFs du sujet (parties 1, 2, 3)
```

### Modules principaux

#### **utils.c/utils.h** - Gestion des graphes et algorithme de Tarjan
- Structures : `cell`, `list_adj`, `graph`, `tarjan_vertex`, `classe`, `partition`, `stack`
- Fonctions de création/manipulation de graphes
- Lecture depuis fichier
- Vérification graphe de Markov
- Génération fichier Mermaid
- **Algorithme de Tarjan** pour les composantes fortement connexes

#### **hasse.c/hasse.h** - Diagramme de Hasse
- Structures : `link`, `link_array`
- Création du diagramme de Hasse (liens entre classes)
- Suppression des liens transitifs (optionnel)
- Analyse des propriétés (classes transitoires/persistantes, états absorbants, irréductibilité)
- Génération Mermaid pour le diagramme

#### **matrix.c/matrix.h** - Calculs matriciels
- Structure : `t_matrix`
- Conversion graphe → matrice
- Opérations : création, copie, multiplication, puissance
- Extraction de sous-matrices
- Calcul des distributions stationnaires
- **BONUS** : Calcul de la périodicité

---

## 🔨 Compilation et exécution

### Méthode 1 : CMake (recommandé)

```bash
# Créer un dossier de build
mkdir build
cd build

# Configurer le projet
cmake ..

# Compiler
cmake --build .

# Exécuter
./markov        # Linux/Mac
markov.exe      # Windows
```

### Méthode 2 : Makefile

```bash
# Compiler
make

# Exécuter
./markov

# Nettoyer
make clean

# Recompiler
make rebuild
```

### Méthode 3 : Compilation manuelle

```bash
# Windows (MinGW)
gcc -Wall -Wextra -std=c11 main.c utils.c hasse.c matrix.c -o markov.exe -lm

# Linux/Mac
gcc -Wall -Wextra -std=c11 main.c utils.c hasse.c matrix.c -o markov -lm
```

---

## ✅ Fonctionnalités implémentées

### ✔️ Partie 1 : Création et vérification

- [x] Lecture de graphe depuis fichier
- [x] Structures de données (liste d'adjacence)
- [x] Vérification graphe de Markov (somme des probabilités = 1)
- [x] Génération fichier Mermaid pour visualisation
- [x] Gestion mémoire complète (pas de fuites)

### ✔️ Partie 2 : Algorithme de Tarjan et Hasse

- [x] Algorithme de Tarjan complet (fonction récursive `parcours`)
- [x] Identification des composantes fortement connexes
- [x] Création du diagramme de Hasse
- [x] Suppression des liens transitifs (optionnel)
- [x] Analyse des propriétés :
  - Classes transitoires/persistantes
  - États absorbants
  - Irréductibilité du graphe

### ✔️ Partie 3 : Calculs matriciels

- [x] Conversion liste d'adjacence → matrice
- [x] Multiplication de matrices
- [x] Calcul de M^n (puissances)
- [x] Fonction `diff()` pour convergence
- [x] Extraction de sous-matrices (`subMatrix`)
- [x] Calcul des distributions stationnaires
- [x] **BONUS** : Calcul de la périodicité (fonctions `gcd` et `getPeriod`)

---

## 🎓 Explications pour la soutenance

### Architecture générale

Le programme est divisé en **3 modules** correspondant aux 3 parties du projet :

1. **utils** : Gère la représentation du graphe en liste d'adjacence et implémente Tarjan
2. **hasse** : Construit le diagramme de Hasse et analyse les propriétés du graphe
3. **matrix** : Effectue les calculs matriciels pour les distributions

### Choix techniques

#### **Structures de données**

- **Liste d'adjacence** : Efficace pour les graphes creux (beaucoup de zéros)
- **Pile** : Pour l'algorithme de Tarjan (structure LIFO)
- **Tableaux dynamiques** : Pour les classes et partitions (taille inconnue à l'avance)

#### **Algorithme de Tarjan**

```c
void tarjan_parcours(int v, graph *g, tarjan_vertex *vertices,
                     stack *s, int *num_counter, partition *part) {
    // 1. Numérotation et ajout à la pile
    vertices[v].num = *num_counter;
    vertices[v].low = *num_counter;
    (*num_counter)++;
    push(s, v);
    vertices[v].in_stack = 1;

    // 2. Parcours des successeurs
    // ... (mise à jour de low)

    // 3. Si racine d'une SCC, créer une classe
    if (vertices[v].low == vertices[v].num) {
        // Dépiler jusqu'à v
    }
}
```

**Complexité** : O(V + E) où V = nombre de sommets, E = nombre d'arêtes

#### **Diagramme de Hasse**

Principe :
1. Créer un tableau `vertex_to_class` qui associe chaque sommet à sa classe
2. Pour chaque arête (i,j), si `classe[i] ≠ classe[j]`, ajouter le lien `Ci → Cj`
3. Optionnel : Supprimer les liens transitifs avec Floyd-Warshall

#### **Calculs matriciels**

Pour calculer la distribution après n étapes :
- **Formule** : Π_n = Π_0 · M^n
- **Méthode** : Multiplication itérative de M

Pour la distribution stationnaire :
- **Condition** : Π* · M = Π*
- **Méthode** : Itération jusqu'à convergence (diff < ε)

---

## 📚 Exemples d'utilisation

### Exemple 1 : Graphe simple (exemple1.txt)

**Entrée** :
```
4
1 1 0.95
1 2 0.04
1 3 0.01
2 2 0.9
2 3 0.05
2 4 0.05
3 3 0.8
3 4 0.2
4 1 1
```

**Résultat** :
- Graphe de Markov : ✓ Valide
- Composantes : 1 classe (irréductible)
- Distribution stationnaire calculée

### Exemple 2 : Graphe avec plusieurs classes (exemple_valid_step3.txt)

**Résultat** :
- 6 classes identifiées
- Classes persistantes : C2, C4
- Classes transitoires : C1, C3, C5, C6
- États absorbants : Sommet 2, Sommet 4

---

## 🤖 Aide à l'IA

### Utilisation d'outils d'IA

**Oui**, nous avons utilisé Claude Code (Anthropic) pour certaines parties du projet.

### Prompts utilisés

#### 1. **Corrections de bugs**
```
Prompt : "Corrige les erreurs de compilation dans utils.c :
- Ligne 269 : point-virgule manquant
- Lignes 144-153 : fonction dupliquée
- Types incohérents (t_tarjan_vertex vs tarjan_vertex)"

Résultat : Corrections appliquées avec succès
```

#### 2. **Implémentation de Tarjan**
```
Prompt : "Implémente l'algorithme de Tarjan pour identifier les composantes
fortement connexes d'un graphe orienté représenté par liste d'adjacence.
Utilise les structures : tarjan_vertex, stack, partition, classe"

Résultat : Fonction tarjan_parcours() et tarjan() générées
```

#### 3. **Module matrix**
```
Prompt : "Crée un module matrix.c/matrix.h pour :
- Convertir liste d'adjacence → matrice
- Multiplier des matrices
- Calculer M^n
- Extraire sous-matrices
- Calculer distributions stationnaires
Inclus le bonus périodicité avec gcd() et getPeriod()"

Résultat : Module complet généré avec toutes les fonctions
```

#### 4. **Documentation**
```
Prompt : "Génère un README.md complet pour la soutenance incluant :
- Description du projet
- Structure des fichiers
- Instructions de compilation
- Explications techniques
- Exemples d'utilisation"

Résultat : Ce fichier README.md
```

### Code obtenu

Tout le code a été vérifié, testé et adapté manuellement. L'IA a fourni :
- ✅ Structure de base des modules
- ✅ Implémentation des algorithmes
- ✅ Documentation et commentaires
- ✅ Gestion mémoire et libération

Nous avons ensuite :
- ✔️ Testé avec tous les fichiers d'exemple
- ✔️ Corrigé les bugs éventuels
- ✔️ Optimisé les performances
- ✔️ Ajouté des vérifications d'erreurs

---

## 🧪 Tests

### Fichiers de test fournis

- `exemple1.txt` : Graphe 4 sommets (valide)
- `exemple2.txt` : Graphe 10 sommets
- `exemple3.txt` : Graphe 8 sommets
- `exemple_valid_step3.txt` : Graphe 10 sommets avec plusieurs classes

### Validation

Toutes les parties ont été validées :
- ✅ Partie 1 : Génération Mermaid correcte
- ✅ Partie 2 : Tarjan et Hasse fonctionnels
- ✅ Partie 3 : Calculs matriciels
- ✅ Bonus : Périodicité calculée

---

## 📞 Contact et questions

Pour toute question lors de la soutenance, nous sommes prêts à expliquer :
- L'architecture du code
- Les choix d'implémentation
- L'utilisation de l'IA
- Les algorithmes utilisés
- Les résultats obtenus

---

**Fin du README - Bonne soutenance !** 🎓
