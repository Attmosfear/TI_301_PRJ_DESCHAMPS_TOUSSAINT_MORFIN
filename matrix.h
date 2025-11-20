#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "utils.h"
#include "hasse.h"

// Structure pour représenter une matrice
typedef struct s_matrix {
    float **data;     // Données de la matrice
    int rows;         // Nombre de lignes
    int cols;         // Nombre de colonnes
} t_matrix;

// ========== FONCTIONS DE CRÉATION ET MANIPULATION ==========

/**
 * @brief Crée une matrice de transitions à partir d'un graphe
 * @param g Le graphe (liste d'adjacence)
 * @return La matrice de transitions
 */
t_matrix graphToMatrix(graph *g);

/**
 * @brief Crée une matrice vide (remplie de zéros)
 * @param n Taille de la matrice (n x n)
 * @return La matrice créée
 */
t_matrix createEmptyMatrix(int n);

/**
 * @brief Crée une matrice identité
 * @param n Taille de la matrice (n x n)
 * @return La matrice identité
 */
t_matrix createIdentityMatrix(int n);

/**
 * @brief Copie une matrice source dans une matrice destination
 * @param dest Matrice destination (doit être déjà allouée)
 * @param src Matrice source
 */
void copyMatrix(t_matrix *dest, t_matrix src);

/**
 * @brief Multiplie deux matrices
 * @param A Première matrice
 * @param B Deuxième matrice
 * @param result Matrice résultat (doit être déjà allouée)
 */
void multiplyMatrices(t_matrix A, t_matrix B, t_matrix *result);

/**
 * @brief Calcule la différence entre deux matrices (somme des valeurs absolues)
 * @param M Première matrice
 * @param N Deuxième matrice
 * @return La somme des |m_ij - n_ij|
 */
float diff(t_matrix M, t_matrix N);

/**
 * @brief Affiche une matrice
 * @param m La matrice à afficher
 */
void printMatrix(t_matrix m);

/**
 * @brief Libère la mémoire d'une matrice
 * @param m La matrice à libérer
 */
void freeMatrix(t_matrix *m);

// ========== FONCTIONS POUR LA PARTIE 3 ==========

/**
 * @brief Extrait une sous-matrice correspondant à une composante
 * @param matrix La matrice originale du graphe
 * @param part La partition du graphe
 * @param compo_index L'index de la composante à extraire
 * @return La sous-matrice correspondant à la composante
 */
t_matrix subMatrix(t_matrix matrix, partition part, int compo_index);

/**
 * @brief Calcule M^n (puissance n d'une matrice)
 * @param M La matrice
 * @param n L'exposant
 * @return M^n
 */
t_matrix matrixPower(t_matrix M, int n);

/**
 * @brief Calcule la distribution après n étapes
 * @param initial_dist Distribution initiale (vecteur ligne)
 * @param M Matrice de transitions
 * @param n Nombre d'étapes
 * @param size Taille de la distribution
 * @return La distribution après n étapes
 */
float* computeDistribution(float *initial_dist, t_matrix M, int n, int size);

/**
 * @brief Trouve la distribution stationnaire
 * @param M La matrice de transitions
 * @param epsilon Seuil de convergence
 * @param max_iter Nombre maximal d'itérations
 * @return La distribution stationnaire
 */
float* findStationaryDistribution(t_matrix M, float epsilon, int max_iter);

// ========== BONUS : PÉRIODICITÉ ==========

/**
 * @brief Calcule le PGCD (Plus Grand Commun Diviseur)
 * @param vals Tableau de valeurs
 * @param nbvals Nombre de valeurs
 * @return Le PGCD
 */
int gcd(int *vals, int nbvals);

/**
 * @brief Calcule la période d'une matrice (classe)
 * @param sub_matrix La sous-matrice de la classe
 * @return La période de la classe
 */
int getPeriod(t_matrix sub_matrix);

#endif
