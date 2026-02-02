#ifndef SOLUTIONS_H
#define SOLUTIONS_H

#include <stddef.h>

int running_total(int x);

int *reverse(const int *arr, size_t len);
void reverse_in_place(int *arr, size_t len);

int num_occurrences(const int *arr, size_t len, int value);

int *remove_duplicates(const int *arr, size_t len, size_t *new_len);

char *string_reverse(const char *str);

int *transpose(const int *matrix, size_t rows, size_t cols);

char **split_string(const char *str, char delim, size_t *count);
void free_string_array(char **arr, size_t count);

#endif
