#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#define maxLen (INT32_MAX)
#define infinity (UINT32_MAX)

// This function creates the matrix.
int **createMatrix(const int *numNodes) {
    int **matrix = calloc(*numNodes + 1, sizeof(int*));
    for (int i = 1; i < *numNodes + 1; ++i)
        *(matrix + i) = calloc(*numNodes + 1, sizeof(int));
    return matrix;
}

// This functions frees the matrix.
void freeMatrix(int **matrix, const int numNodes) {
    for (int i = 1; i < numNodes + 1; i++)
        free(*(matrix + i));
    free(matrix);
}

// This function scans number of nodes and edges.
int scanNodesAndEdges(FILE* in, FILE* out, int* numNodes, int* numEdges) {
    if (1 != fscanf(in, "%d", numNodes))
        return -1;
    if (*numNodes < 0 || *numNodes > 5000) {
        fprintf(out, "%s", "bad number of vertices");
        return -1;
    }
    if (*numNodes == 0) {
        fprintf(out, "%s", "no spanning tree");
        return -1;
    }
    if (1 != fscanf(in, "%d", numEdges))
        return -1;
    if (*numEdges < 0 || *numEdges > *numNodes * (*numNodes - 1) / 2) {
        fprintf(out, "%s", "bad number of edges");
        return -1;
    }
    return 0;
}

// This functions scans values, checks them, and fills the matrix.
int fillMatrix(FILE *in, FILE *out, int **matrix, const int* numNodes, const int *numEdges) {
    bool* tags = calloc(*numNodes + 1, sizeof(bool));
    for (int i = 1; i < *numEdges + 1; ++i) {
        int first, second = 0;
        long long edge = 0;
        if (3 != fscanf(in, "%d %d %lld", &first, &second, &edge)) {
            fprintf(out, "%s", "bad number of lines");
            free(tags);
            return -1;
        }
        // Checking for bad values.
        if (edge < 0 || edge > maxLen) {
            fputs("bad length", out);
            free(tags);
            return -1;
        }
        if (first < 1 || first > *numNodes || second < 1 || second > *numNodes) {
            fprintf(out, "%s", "bad vertex");
            free(tags);
            return -1;
        }
        *(*(matrix + first) + second) = (int) edge;
        *(*(matrix + second) + first) = (int) edge;
        *(tags + first) = true;
        *(tags + second) = true;
    }
    for (int i = 1; i < *numNodes + 1; ++i)
        if (!*(tags + i) && *numNodes != 1) {
            fprintf(out, "%s", "no spanning tree");
            free(tags);
            return -1;
        }
    free(tags);
    return 0;
}

// This functions fills len array.
void fillLen(unsigned int* len, int numNodes) {
    for (int i = 1; i < numNodes + 1; ++i) {
        *(len + i) = infinity;
    }
    *(len + 1) = 0;
}

// This function fills sel array.
void fillSel(int* sel, int numNodes) {
    for (int i = 1; i < numNodes + 1; ++i)
        *(sel + i) = -1;
}

// This function finds the closest vertex.
int findClosestVertex(int numNodes, const unsigned int *len, const bool *tags) {
    int vertex = -1;
    for (int i = 1; i < numNodes + 1; i++)
        if (!*(tags + i) && ((vertex == -1 || *(len + vertex) > *(len + i))))
            vertex = i;
    return vertex;
}

// This is the Prim's algorithm.
void prim(int** matrix, int numNodes, bool*tags, unsigned int* len, int* sel, FILE* out) {
    fillLen(len, numNodes);
    fillSel(sel, numNodes);
    for (int i = 1; i < numNodes + 1; ++i) {
        int vertex = findClosestVertex(numNodes, len, tags);
        if (vertex == -1)
            break;
        *(tags + vertex) = true;
        for (int j = 1; j < numNodes + 1; ++j)
            if ((unsigned int) *(*(matrix + vertex) + j) < *(len + j) && *(*(matrix + vertex) + j) != 0) {
                *(len + j) = (unsigned int)*(*(matrix + vertex) + j);
                *(sel + j) = vertex;
            }
        if (*(sel + vertex) != -1)
            fprintf(out, "%d %d\n", *(sel + vertex) ,vertex);
    }
}