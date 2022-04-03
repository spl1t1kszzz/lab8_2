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
        freeMatrix(matrix,&numNodes);
        fclose(in);
        fclose(out);
        return 0;
    }
    bool* tags = createTags(&numEdges);
    prim(matrix, &numNodes, tags, out);
    freeMatrix(matrix, &numNodes);
    free(tags);
    fclose(in);
    fclose(out);
    return 0;
}
