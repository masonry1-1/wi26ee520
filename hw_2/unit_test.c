#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solutions.h"

#define ASSERT_EQ(a,b) do { if ((a)!=(b)) { printf("FAIL %s:%d\n", __FILE__, __LINE__); return 1; } } while(0)
#define ASSERT_STREQ(a,b) do { if (strcmp((a),(b))!=0) { printf("FAIL %s:%d\n", __FILE__, __LINE__); return 1; } } while(0)

int run_unit_tests(void) {
    ASSERT_EQ(running_total(1), 1);
    ASSERT_EQ(running_total(1), 2);
    ASSERT_EQ(running_total(5), 7);
    ASSERT_EQ(running_total(-3), 4);

    int x[] = {10,20,30,40,50};
    int *y = reverse(x, 5);
    ASSERT_EQ(y[0], 50);
    ASSERT_EQ(y[1], 40);
    ASSERT_EQ(y[2], 30);
    ASSERT_EQ(y[3], 20);
    ASSERT_EQ(y[4], 10);
    free(y);

    int z[] = {10,20,30,40,50};
    reverse_in_place(z, 5);
    ASSERT_EQ(z[0], 50);
    ASSERT_EQ(z[1], 40);
    ASSERT_EQ(z[2], 30);
    ASSERT_EQ(z[3], 20);
    ASSERT_EQ(z[4], 10);

    int a[] = {1,1,2,3,1,4,5,2,20,5};
    ASSERT_EQ(num_occurrences(a, 10, 1), 3);

    int b[] = {1,2,2,3,1,4,5,3,2};
    size_t new_len = 0;
    int *u = remove_duplicates(b, 9, &new_len);
    ASSERT_EQ((int)new_len, 5);
    ASSERT_EQ(u[0], 1);
    ASSERT_EQ(u[1], 2);
    ASSERT_EQ(u[2], 3);
    ASSERT_EQ(u[3], 4);
    ASSERT_EQ(u[4], 5);
    free(u);

    char *rev = string_reverse("hello");
    ASSERT_STREQ(rev, "olleh");
    free(rev);

    int m[] = {1,2,3,4,5,6};
    int *t = transpose(m, 2, 3);
    int expected[] = {1,4,2,5,3,6};
    for (int i = 0; i < 6; i++) ASSERT_EQ(t[i], expected[i]);
    free(t);

    size_t cnt = 0;
    char **parts = split_string("apple,banana,,cherry", ',', &cnt);
