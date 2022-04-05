#include <stdio.h>
#include "functions.h"

int main() {
    FILE* in = fopen("in.txt", "r");
    FILE* out = fopen("out.txt", "w");
    int numNodes, numEdges = 0;
    if (-1 == scanNodesAndEdges(in, out, &numNodes, &numEdges)) {
        fclose(in);
        fclose(out);
        return 0;
    }
    int **matrix = createMatrix(&numNodes);
    if (-1 == fillMatrix(in, out, matrix, &numNodes, &numEdges)) {
        freeMatrix(matrix,numNodes);
        fclose(in);
        fclose(out);
        return 0;
    }
    bool* tags = calloc(numNodes + 1, sizeof(bool));
    unsigned int* len = calloc(numNodes + 1, sizeof(unsigned int));
    int* sel = calloc(numNodes + 1, sizeof(int));
    prim(matrix, numNodes, tags, len, sel, out);
    freeMatrix(matrix, numNodes);
    free(tags);
    free(sel);
    free(len);
    fclose(in);
    fclose(out);
    return 0;
}
