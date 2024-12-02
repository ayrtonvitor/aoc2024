#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void resizearr(int **vals, size_t cap) {
    int *nvals = realloc(*vals, cap * sizeof(int));
    if (nvals == NULL) {
        free(vals);
        printf("realloc failure\n");
        exit(EXIT_FAILURE);
    }
    *vals = nvals;
    return;
}

int main(int argc, char *argv[]) {
    if (argc !=2) {
        printf("File name required\n");
        return 1;
    }

    FILE *file;
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    size_t len = 0;
    size_t cap = 2;
    int *vals1 = malloc(cap * sizeof(int));
    int *vals2 = malloc(cap * sizeof(int));
    if (vals1 == NULL || vals2 == NULL) {
        printf("Can't create lists\n");
        return 1;
    }

    int n1, n2;
    while (fscanf(file, "%d   %d", &n1, &n2) == 2) {
        if (len >= cap) {
            cap *= 2;
            resizearr(&vals1, cap);
            resizearr(&vals2, cap);
        }
        vals1[len] = n1;
        vals2[len] = n2;
        len++;
    }
    qsort(vals1, len, sizeof(int), compare);
    qsort(vals2, len, sizeof(int), compare);

    long long res = 0;
    for (int i=0; i < len; i++) {
        int x = vals1[i];
        int y = vals2[i];
        res += x > y ? x - y : y - x;
    }
    printf("%lld\n", res);

    fclose(file);
    return 0;
}
