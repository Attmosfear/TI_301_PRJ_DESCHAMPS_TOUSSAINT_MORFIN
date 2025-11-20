#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "utils.h"

// ========== FONCTIONS DE CRÉATION ET MANIPULATION ==========

t_matrix graphToMatrix(graph *g) {
    t_matrix m;
    m.rows = g->num_edges;
    m.cols = g->num_edges;

    // Allouer la matrice
    m.data = malloc(m.rows * sizeof(float*));
    for (int i = 0; i < m.rows; i++) {
        m.data[i] = calloc(m.cols, sizeof(float));
    }

    // Remplir la matrice avec les probabilités
    for (int i = 0; i < g->num_edges; i++) {
        cell *current = g->edges[i].head;
        while (current) {
            int j = current->end_edge - 1;  // Convertir en index base 0
            m.data[i][j] = current->weight;
            current = current->next;
        }
    }

    return m;
}

t_matrix createEmptyMatrix(int n) {
    t_matrix m;
    m.rows = n;
    m.cols = n;
    m.data = malloc(n * sizeof(float*));
    for (int i = 0; i < n; i++) {
        m.data[i] = calloc(n, sizeof(float));
    }
    return m;
}

t_matrix createIdentityMatrix(int n) {
    t_matrix m = createEmptyMatrix(n);
    for (int i = 0; i < n; i++) {
        m.data[i][i] = 1.0f;
    }
    return m;
}

void copyMatrix(t_matrix *dest, t_matrix src) {
    if (dest->rows != src.rows || dest->cols != src.cols) {
        fprintf(stderr, "Erreur: dimensions incompatibles pour copyMatrix\n");
        return;
    }

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            dest->data[i][j] = src.data[i][j];
        }
    }
}

void multiplyMatrices(t_matrix A, t_matrix B, t_matrix *result) {
    if (A.cols != B.rows) {
        fprintf(stderr, "Erreur: dimensions incompatibles pour la multiplication\n");
        return;
    }

    if (result->rows != A.rows || result->cols != B.cols) {
        fprintf(stderr, "Erreur: dimensions incorrectes pour la matrice résultat\n");
        return;
    }

    // Initialiser le résultat à zéro
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            result->data[i][j] = 0;
        }
    }

    // Multiplication matricielle
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < B.cols; j++) {
            for (int k = 0; k < A.cols; k++) {
                result->data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
}

float diff(t_matrix M, t_matrix N) {
    if (M.rows != N.rows || M.cols != N.cols) {
        fprintf(stderr, "Erreur: dimensions incompatibles pour diff\n");
        return -1;
    }

    float sum = 0;
    for (int i = 0; i < M.rows; i++) {
        for (int j = 0; j < M.cols; j++) {
            sum += fabsf(M.data[i][j] - N.data[i][j]);
        }
    }
    return sum;
}

void printMatrix(t_matrix m) {
    printf("Matrice %dx%d:\n", m.rows, m.cols);
    for (int i = 0; i < m.rows; i++) {
        printf("[ ");
        for (int j = 0; j < m.cols; j++) {
            printf("%.4f ", m.data[i][j]);
        }
        printf("]\n");
    }
    printf("\n");
}

void freeMatrix(t_matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        free(m->data[i]);
    }
    free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

// ========== FONCTIONS POUR LA PARTIE 3 ==========

t_matrix subMatrix(t_matrix matrix, partition part, int compo_index) {
    if (compo_index < 0 || compo_index >= part.nb_classes) {
        fprintf(stderr, "Erreur: index de composante invalide\n");
        return createEmptyMatrix(0);
    }

    classe *c = &part.classes[compo_index];
    int n = c->nb_vertices;

    // Créer la sous-matrice
    t_matrix sub = createEmptyMatrix(n);

    // Extraire les lignes et colonnes correspondant aux sommets de la classe
    for (int i = 0; i < n; i++) {
        int row = c->vertices[i].id - 1;  // Convertir en index base 0
        for (int j = 0; j < n; j++) {
            int col = c->vertices[j].id - 1;
            sub.data[i][j] = matrix.data[row][col];
        }
    }

    return sub;
}

t_matrix matrixPower(t_matrix M, int n) {
    if (M.rows != M.cols) {
        fprintf(stderr, "Erreur: la matrice doit être carrée\n");
        return createEmptyMatrix(0);
    }

    if (n == 0) {
        return createIdentityMatrix(M.rows);
    }

    if (n == 1) {
        t_matrix result = createEmptyMatrix(M.rows);
        copyMatrix(&result, M);
        return result;
    }

    // Multiplication itérative
    t_matrix result = createEmptyMatrix(M.rows);
    t_matrix temp = createEmptyMatrix(M.rows);
    copyMatrix(&result, M);

    for (int i = 1; i < n; i++) {
        multiplyMatrices(result, M, &temp);
        copyMatrix(&result, temp);
    }

    freeMatrix(&temp);
    return result;
}

float* computeDistribution(float *initial_dist, t_matrix M, int n, int size) {
    if (size != M.rows) {
        fprintf(stderr, "Erreur: taille incompatible\n");
        return NULL;
    }

    // Calculer M^n
    t_matrix Mn = matrixPower(M, n);

    // Allouer le vecteur résultat
    float *result = malloc(size * sizeof(float));

    // Multiplier le vecteur initial par M^n
    for (int j = 0; j < size; j++) {
        result[j] = 0;
        for (int i = 0; i < size; i++) {
            result[j] += initial_dist[i] * Mn.data[i][j];
        }
    }

    freeMatrix(&Mn);
    return result;
}

float* findStationaryDistribution(t_matrix M, float epsilon, int max_iter) {
    int n = M.rows;

    // Distribution initiale uniforme
    float *current = malloc(n * sizeof(float));
    float *next = malloc(n * sizeof(float));
    for (int i = 0; i < n; i++) {
        current[i] = 1.0f / n;
    }

    // Itération jusqu'à convergence
    for (int iter = 0; iter < max_iter; iter++) {
        // Calculer next = current * M
        for (int j = 0; j < n; j++) {
            next[j] = 0;
            for (int i = 0; i < n; i++) {
                next[j] += current[i] * M.data[i][j];
            }
        }

        // Calculer la différence
        float delta = 0;
        for (int i = 0; i < n; i++) {
            delta += fabsf(next[i] - current[i]);
        }

        // Copier next dans current
        for (int i = 0; i < n; i++) {
            current[i] = next[i];
        }

        // Vérifier la convergence
        if (delta < epsilon) {
            printf("Convergence atteinte après %d itérations\n", iter + 1);
            free(next);
            return current;
        }
    }

    printf("Convergence non atteinte après %d itérations\n", max_iter);
    free(next);
    return current;
}

// ========== BONUS : PÉRIODICITÉ ==========

int gcd(int *vals, int nbvals) {
    if (nbvals == 0) return 0;

    int result = vals[0];
    for (int i = 1; i < nbvals; i++) {
        int a = result;
        int b = vals[i];
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        result = a;
    }
    return result;
}

int getPeriod(t_matrix sub_matrix) {
    int n = sub_matrix.rows;
    int *periods = malloc(n * sizeof(int));
    int period_count = 0;

    t_matrix power_matrix = createEmptyMatrix(n);
    t_matrix result_matrix = createEmptyMatrix(n);
    copyMatrix(&power_matrix, sub_matrix);

    for (int cpt = 1; cpt <= n; cpt++) {
        int diag_nonzero = 0;
        for (int i = 0; i < n; i++) {
            if (power_matrix.data[i][i] > 0.0f) {
                diag_nonzero = 1;
                break;
            }
        }

        if (diag_nonzero) {
            periods[period_count] = cpt;
            period_count++;
        }

        multiplyMatrices(power_matrix, sub_matrix, &result_matrix);
        copyMatrix(&power_matrix, result_matrix);
    }

    int period = gcd(periods, period_count);

    freeMatrix(&power_matrix);
    freeMatrix(&result_matrix);
    free(periods);

    return period;
}
