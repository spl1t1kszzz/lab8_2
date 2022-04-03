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
    for (int i = 1; i < *numNodes + 1; ++i) {
        *(matrix + i) = calloc(*numNodes + 1, sizeof(int));
    }
    return matrix;
}

// This functions frees the matrix.
void freeMatrix(int **matrix, const int *numNodes) {
    for (int i = 1; i < *numNodes + 1; i++)
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

// This function creates tags array.
bool* createTags(int* numNodes) {
    bool* tags = calloc(*numNodes + 1, sizeof(bool));
    *(tags + 1) = true;
    return tags;
}

// This function finds the closest untagged node.
void findClosestNode(int** matrix, int* numNodes, bool* tags, int index, int* first, int* second, unsigned int* min) {
    for (int i = 1; i < *numNodes + 1; ++i) {
        // If node is untagged and if there is a way, we are updating the values.
        if (!*(tags + i) && *(*(matrix + index) + i) && (unsigned int) *(*(matrix + i) + index) < *min){
            *min = (unsigned int) *(*(matrix + i) + index);
            *first = index;
            *second = i;
        }
    }
    return;
}

// This is the Prim's algorithm.
void prim(int** matrix, int* numNodes, bool*tags, FILE* out) {
    int count = 1;
    while (count < *numNodes) {
        unsigned int min = infinity;
        int first = 0;
        int second = 0;
        for (int i = 1; i < *numNodes + 1; i++)
            if (*(tags + i))
                findClosestNode(matrix, numNodes, tags,i, &first, &second, &min);
        fprintf(out, "%d %d\n", first, second);
        *(tags + second) = true;
        count++;
    }
}