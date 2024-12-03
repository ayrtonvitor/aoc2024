#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))

int is_safe(int *nums, int len, int discard);


int valid_seq(int *seq) {
    int x = seq[0];
    int y = seq[1];
    int z = seq[2];
    return !(
        (x - y) * (y - z) < 0 ||
        ABS(x - y) < 1 ||
        ABS(x - y) > 3 ||
        ABS(y - z) < 1 ||
        ABS(y - z) > 3
    );
}

int is_safe_without(int *nums, int len, int ind) {
    int *newnums = malloc((len - 1) * sizeof(int));
    int diff = 0;
    for (int i = 0; i < len; i++) {
        if (i == ind) {
            diff = 1;
            continue;
        }
        newnums[i - diff] = nums[i];
    }
    int safe = is_safe(newnums, len-1, 0);

    free(newnums);

    return safe;
}

int retry_is_safe(int *nums, int len, int curr) {
    if (curr == len-1) {
        return 1;
    }
    if (curr == 2) {
        return is_safe_without(nums, len, 0) ||
            is_safe_without(nums, len, 1) ||
            is_safe_without(nums, len, 2);
    }
    return is_safe_without(nums, len, curr) ||
        is_safe_without(nums, len, curr-1);
}

int is_safe(int *nums, int len, int discard) {
    for (int i = 2; i < len; i++) {
        int seq[] = { nums[i-2], nums[i-1], nums[i] };

        if (valid_seq(seq) == 0) {
            if (discard) {
                return retry_is_safe(nums, len, i);
            }

            return 0;
        }
    }

    return 1;
}

void solve(FILE **file) {
    int *nums = malloc(sizeof(int));
    int res = 0;
    int retry_res = 0;
    char line[100];
    while (fgets(line, sizeof(line), *file)) {
        int count = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == ' ') {
                count++;
            }
        }

        int i = 0;
        nums = realloc(nums, (count + 1) * sizeof(int));
        char *token = strtok(line, " ");
        while (token != NULL) {
            nums[i] = atoi(token);
            i++;
            token = strtok(NULL, " ");
        }

        res += is_safe(nums, count + 1, 0);
        retry_res += is_safe(nums, count + 1, 1);
    }
    printf("result: %d\n", res);
    printf("result with retry: %d\n", retry_res);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("File name required\n");
        return 1;
    }

    FILE *file;
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    solve(&file);

    fclose(file);
    return 0;
}
