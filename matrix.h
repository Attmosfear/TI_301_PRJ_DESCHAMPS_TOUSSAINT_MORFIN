#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "utils.h"
#include "hasse.h"

// Structure pour représenter une matrice
typedef struct s_matrix {
    float **data;
    int rows;
    int cols;
} t_matrix;

// ========== FONCTIONS DE CRÉATION ET MANIPULATION ==========

t_matrix graphToMatrix(graph *g);

t_matrix createEmptyMatrix(int n);

t_matrix createIdentityMatrix(int n);

void copyMatrix(t_matrix *dest, t_matrix src);

void multiplyMatrices(t_matrix A, t_matrix B, t_matrix *result);

float diff(t_matrix M, t_matrix N);

void printMatrix(t_matrix m);

void freeMatrix(t_matrix *m);

// ========== FONCTIONS POUR LA PARTIE 3 ==========

t_matrix subMatrix(t_matrix matrix, partition part, int compo_index);

t_matrix matrixPower(t_matrix M, int n);

float* computeDistribution(float *initial_dist, t_matrix M, int n, int size);

float* findStationaryDistribution(t_matrix M, float epsilon, int max_iter);

// ========== BONUS : PÉRIODICITÉ ==========

int gcd(int *vals, int nbvals);

int getPeriod(t_matrix sub_matrix);

#endif
