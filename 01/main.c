#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void resizearr(int **vals, size_t cap) {
    int *nvals = realloc(*vals, cap * sizeof(int));
    if (nvals == NULL) {
        free(*vals);
        printf("realloc failure\n");
        exit(EXIT_FAILURE);
    }
    *vals = nvals;
    return;
}

int countequals(int *vals, size_t i, size_t len, int val) {
    int counts = 0;
    while (i < len && vals[i] == val) {
        counts++;
        i++;
    }
    return counts;
}

void puzzle1(int len, int *vals1, int *vals2) {
    qsort(vals1, len, sizeof(int), compare);
    qsort(vals2, len, sizeof(int), compare);

    long long diff = 0;
    for (int i=0; i < len; i++) {
        int x = vals1[i];
        int y = vals2[i];
        diff += x > y ? x - y : y - x;
    }
    printf("Total difference: %lld\n", diff);
}

void puzzle2(int len, int *vals1, int *vals2) {
    long long sim = 0;
    size_t l = 0, r = 0;
    while (l < len && r < len) {
        while (l < len && vals1[l] < vals2[r]) { l++; }
        while (r < len && vals1[l] > vals2[r]) { r++; }
        if (l >= len || r >= len) { break; }

        int val = vals1[l];
        int lcounts = countequals(vals1, l, len, val);
        int rcounts = countequals(vals2, r, len, val);
        sim += val * lcounts * rcounts;
        l += lcounts;
        r += rcounts;
    }
    printf("Similarity: %lld\n", sim);
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

    puzzle1(len, vals1, vals2);
    puzzle2(len, vals1, vals2);

    fclose(file);
    return 0;
}
