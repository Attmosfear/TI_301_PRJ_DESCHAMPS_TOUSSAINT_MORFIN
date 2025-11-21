# Projet Graphes de Markov – TI301

Étudiants : DESCHAMPS, TOUSSAINT, MORFIN
Module : TI301 – Algorithmique et Structures de Données 2

## 1. Description du projet

Le projet porte sur l'étude de graphes de Markov : construction du graphe, analyse structurelle via Tarjan et Hasse, puis étude du comportement à long terme grâce aux calculs matriciels.

## 2. Structure du projet

```
main.c              – Programme principal
utils.c/.h          – Gestion du graphe, Tarjan
hasse.c/.h          – Diagramme de Hasse, analyse des classes
matrix.c/.h         – Calculs matriciels
CMakeLists.txt, Makefile
data/               – Fichiers d'exemples
```

## 3. Compilation et exécution

Avec CMake :

```
mkdir build && cd build
cmake ..
cmake --build .
./markov
```

Avec Make :

```
make
./markov
```

## 4. Fonctionnalités principales

* Lecture du graphe et vérification qu’il s’agit d’un graphe de Markov.
* Algorithme de Tarjan pour les composantes fortement connexes.
* Construction du diagramme de Hasse et identification des classes.
* Calculs matriciels : M^n, distributions stationnaires, convergence.
* Bonus : calcul de la périodicité.

## 5. Explications techniques

Le projet est séparé en trois modules :

* **utils** : représentation du graphe, chargement, Tarjan (complexité O(V+E)).
* **hasse** : construction du diagramme de Hasse, suppression des liens transitifs, analyse structurelle.
* **matrix** : opérations matricielles et calculs des distributions.

Choix techniques : liste d’adjacence (structure légère), pile dédiée pour Tarjan, itérations pour la convergence des distributions.

## 6. Exemples d'utilisation

Dans les exemples fournis, le programme détecte différentes propriétés du graphe. Les notions utilisées sont :

* **Classe fortement connexe** : ensemble de sommets tous accessibles les uns des autres.
* **Classe transitoire** : on peut quitter la classe sans possibilité d’y revenir.
* **Classe persistante (récurrente)** : aucune sortie définitive ; une fois à l’intérieur, on y revient toujours à long terme.
* **État absorbant** : état dont on ne peut plus sortir (probabilité de rester = 1).
* **Graphe irréductible** : tout le graphe forme une seule classe fortement connexe.

Ces définitions permettent d'interpréter les sorties du programme sur les graphes simples comme sur ceux comportant plusieurs classes.

## 7. Utilisation de l’IA

Nous avons utilisé un outil d’IA pour : proposer du squelette de code, résoudre certains bugs, générer de la documentation. Tout le code a ensuite été vérifié, adapté et testé manuellement.

## 8. Tests

Les fichiers d’exemple fournis ont été utilisés pour valider : Tarjan, Hasse, calcul matriciel, convergence, périodicité.
