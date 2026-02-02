#include "solutions.h"
#include <stdlib.h>
#include <string.h>

int running_total(int x) {
    static int total = 0;
    total += x;
    return total;
}

int *reverse(const int *arr, size_t len) {
    int *out = (int *)calloc(len, sizeof(int));
    if (!out) return NULL;
    for (size_t i = 0; i < len; i++) out[i] = arr[len - 1 - i];
    return out;
}

void reverse_in_place(int *arr, size_t len) {
    for (size_t i = 0; i < len / 2; i++) {
        int t = arr[i];
        arr[i] = arr[len - 1 - i];
        arr[len - 1 - i] = t;
    }
}

int num_occurrences(const int *arr, size_t len, int value) {
    int c = 0;
    for (size_t i = 0; i < len; i++) if (arr[i] == value) c++;
    return c;
}

int *remove_duplicates(const int *arr, size_t len, size_t *new_len) {
    int *out = (int *)malloc(len * sizeof(int));
    if (!out) return NULL;
    size_t k = 0;
    for (size_t i = 0; i < len; i++) {
        int seen = 0;
        for (size_t j = 0; j < k; j++) {
            if (out[j] == arr[i]) {
                seen = 1;
                break;
            }
        }
        if (!seen) out[k++] = arr[i];
    }
    *new_len = k;
    return out;
}

char *string_reverse(const char *str) {
    if (!str) return NULL;
    size_t n = strlen(str);
    char *out = (char *)malloc(n + 1);
    if (!out) return NULL;
    for (size_t i = 0; i < n; i++) out[i] = str[n - 1 - i];
    out[n] = '\0';
    return out;
}

int *transpose(const int *matrix, size_t rows, size_t cols) {
    int *out = (int *)malloc(rows * cols * sizeof(int));
    if (!out) return NULL;
    for (size_t r = 0; r < rows; r++)
        for (size_t c = 0; c < cols; c++)
            out[c * rows + r] = matrix[r * cols + c];
    return out;
}

char **split_string(const char *str, char delim, size_t *count) {
    if (!count) return NULL;
    *count = 0;
    if (!str) return NULL;

    size_t cap = 4;
    char **out = (char **)malloc(cap * sizeof(char *));
    if (!out) return NULL;

    size_t i = 0;
    while (str[i]) {
        while (str[i] == delim) i++;
        if (!str[i]) break;

        size_t start = i;
        while (str[i] && str[i] != delim) i++;
        size_t len = i - start;

        char *tok = (char *)malloc(len + 1);
        if (!tok) {
            free_string_array(out, *count);
            return NULL;
        }
        memcpy(tok, str + start, len);
        tok[len] = '\0';

        if (*count == cap) {
            cap *= 2;
            char **tmp = (char **)realloc(out, cap * sizeof(char *));
            if (!tmp) {
                free(tok);
                free_string_array(out, *count);
                return NULL;
            }
            out = tmp;
        }
        out[(*count)++] = tok;
    }
    return out;
}

void free_string_array(char **arr, size_t count) {
    if (!arr) return;
    for (size_t i = 0; i < count; i++) free(arr[i]);
    free(arr);
}
