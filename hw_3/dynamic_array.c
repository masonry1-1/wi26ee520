#include "dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* private functions *********************************************************/

/* Position in the buffer of the array element at position index */
static int index_to_offset ( const DynamicArray * da, int index ) {
    return index + da->origin;
}

/* Position of the element at buffer position 'offset' */
static int offset_to_index ( const DynamicArray * da, int offset ) {
    return offset - da->origin;
}

/* Non-zero if and only if offset lies ouside the buffer */
static int out_of_buffer ( DynamicArray * da, int offset ) {
    return offset < 0 || offset >= da->capacity;
}

/* Makes a new buffer that is twice the size of the old buffer,
   copies the old information into the new buffer, and deletes
   the old buffer */
static void extend_buffer ( DynamicArray * da ) {

    double * temp = (double *) calloc ( 2 * da->capacity, sizeof(double) );
    int new_origin = da->capacity - (da->end - da->origin)/2,
           new_end = new_origin + (da->end - da->origin);

    for ( int i=0; i<DynamicArray_size(da); i++ ) {
        temp[new_origin+i] = DynamicArray_get(da,i);
    }

    free(da->buffer);
    da->buffer = temp;

    da->capacity = 2 * da->capacity;
    da->origin = new_origin;
    da->end = new_end;

    return;
}

/* registry for "num_arrays" and "destroy_all" ********************************/

static DynamicArray ** __all_arrays = NULL;
static int __all_arrays_count = 0;
static int __all_arrays_capacity = 0;

static void __register_array(DynamicArray *da) {
    if (__all_arrays_count >= __all_arrays_capacity) {
        int new_cap = (__all_arrays_capacity == 0) ? 16 : (__all_arrays_capacity * 2);
        DynamicArray **tmp = (DynamicArray **)realloc(__all_arrays, sizeof(DynamicArray*) * new_cap);
        assert(tmp != NULL);
        __all_arrays = tmp;
        __all_arrays_capacity = new_cap;
    }
    __all_arrays[__all_arrays_count++] = da;
}

/* public functions **********************************************************/

DynamicArray * DynamicArray_new(void) {
    DynamicArray * da = (DynamicArray *) malloc(sizeof(DynamicArray));
    assert(da != NULL);
    da->capacity = DYNAMIC_ARRAY_INITIAL_CAPACITY;
    da->buffer = (double *) calloc ( da->capacity, sizeof(double) );
    assert(da->buffer != NULL);
    da->origin = da->capacity / 2;
    da->end = da->origin;
    __register_array(da);
    return da;
}

void DynamicArray_destroy(DynamicArray * da) {
    free(da->buffer);
    da->buffer = NULL;
    return;
}

int DynamicArray_size(const DynamicArray * da) {
    assert(da->buffer != NULL);
    return da->end - da->origin;
}

char * DynamicArray_to_string(const DynamicArray * da) {
    assert(da->buffer != NULL);
    char * str = (char *) calloc (20,DynamicArray_size(da)),
         temp[20];
    int j = 1;
    str[0] = '[';
    for ( int i=0; i < DynamicArray_size(da); i++ ) {
        if ( DynamicArray_get(da,i) == 0 ) {
            snprintf ( temp, 20, "0" );
        } else {
            snprintf ( temp, 20, "%.5lf", DynamicArray_get(da,i) );
        }
        if ( i < DynamicArray_size(da) - 1 ) {
            sprintf( str + j, "%s,", temp);
            j += (int)strlen(temp) + 1;
        } else {
            sprintf( str + j, "%s", temp);
            j += (int)strlen(temp);
        }

    }
    str[j] = ']';
    return str;
}

void DynamicArray_print_debug_info(const DynamicArray * da) {

    char * s = DynamicArray_to_string(da);
    printf ( "  %s\n", s);
    printf ( "  capacity: %d\n  origin: %d\n  end: %d\n  size: %d\n\n",
      da->capacity,
      da->origin,
      da->end,
      DynamicArray_size(da));

    free(s);

}

void DynamicArray_set(DynamicArray * da, int index, double value) {
    assert(da->buffer != NULL);
    assert ( index >= 0 );
    while ( out_of_buffer(da, index_to_offset(da, index) ) ) {
        extend_buffer(da);
    }
    da->buffer[index_to_offset(da, index)] = value;
    if ( index >= DynamicArray_size(da) ) {
        da->end = index_to_offset(da,index+1);
    }
}

double DynamicArray_get(const DynamicArray * da, int index) {
    assert(da->buffer != NULL);
    assert ( index >= 0 );
    if ( index >= DynamicArray_size(da) ) {
        return 0;
    } else {
        return da->buffer[index_to_offset(da,index)];
    }
}

void DynamicArray_push(DynamicArray * da, double value ) {
    DynamicArray_set(da, DynamicArray_size(da), value );
}

void DynamicArray_push_front(DynamicArray * da, double value) {
    assert(da->buffer != NULL);
    while ( da->origin == 0 ) {
        extend_buffer(da);
    }
    da->origin--;
    DynamicArray_set(da,0,value);
}

double DynamicArray_pop(DynamicArray * da) {
    assert(DynamicArray_size(da) > 0);
    double value = DynamicArray_get(da, DynamicArray_size(da)-1);
    DynamicArray_set(da, DynamicArray_size(da)-1, 0.0);
    da->end--;
    return value;
}

double DynamicArray_pop_front(DynamicArray * da) {
    assert(DynamicArray_size(da) > 0);
    double value = DynamicArray_get(da, 0);
    da->origin++;
    return value;
}

DynamicArray * DynamicArray_map(const DynamicArray * da, double (*f) (double)) {
    assert(da->buffer != NULL);
    DynamicArray * result = DynamicArray_new();
    for ( int i=0; i<DynamicArray_size(da); i++ ) {
        DynamicArray_set(result, i, f(DynamicArray_get(da, i)));
    }
    return result;
}

DynamicArray * DynamicArray_subarray(DynamicArray * da, int a, int b) {

  assert(da->buffer != NULL);
  assert(b >= a);

  DynamicArray * result = DynamicArray_new();

  for (int i=a; i<b; i++) {
      DynamicArray_push(result,DynamicArray_get(da, i));
  }

  return result;
}

/* EXERCISES *****************************************************************/

static int __dbl_cmp(const void *pa, const void *pb) {
    double a = *(const double *)pa;
    double b = *(const double *)pb;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

double DynamicArray_last ( const DynamicArray * da ) {
    assert(da->buffer != NULL);
    assert(DynamicArray_size(da) > 0);
    return DynamicArray_get(da, DynamicArray_size(da) - 1);
}

double DynamicArray_first ( const DynamicArray * da ) {
    assert(da->buffer != NULL);
    assert(DynamicArray_size(da) > 0);
    return DynamicArray_get(da, 0);
}

DynamicArray * DynamicArray_copy ( const DynamicArray * da ) {
    assert(da->buffer != NULL);
    DynamicArray * r = DynamicArray_new();
    int n = DynamicArray_size(da);
    for (int i = 0; i < n; i++) {
        DynamicArray_set(r, i, DynamicArray_get(da, i));
    }
    return r;
}

DynamicArray * DynamicArray_range ( double a, double b, double step) {
    assert(step != 0.0);

    DynamicArray * r = DynamicArray_new();
    int i = 0;

    if (step > 0.0) {
        assert(a <= b);
        for (double x = a; x <= b + 1e-12; x += step) {
            DynamicArray_set(r, i++, x);
        }
    } else {
        assert(a >= b);
        for (double x = a; x >= b - 1e-12; x += step) {
            DynamicArray_set(r, i++, x);
        }
    }
    return r;
}

DynamicArray * DynamicArray_concat ( const DynamicArray * a, const DynamicArray * b ) {
    assert(a->buffer != NULL);
    assert(b->buffer != NULL);

    DynamicArray * r = DynamicArray_new();
    int na = DynamicArray_size(a);
    int nb = DynamicArray_size(b);

    for (int i = 0; i < na; i++) {
        DynamicArray_push(r, DynamicArray_get(a, i));
    }
    for (int i = 0; i < nb; i++) {
        DynamicArray_push(r, DynamicArray_get(b, i));
    }

    return r;
}

double DynamicArray_min ( const DynamicArray * da ) {
    assert(da->buffer != NULL);
    int n = DynamicArray_size(da);
    assert(n > 0);
    double m = DynamicArray_get(da, 0);
    for (int i = 1; i < n; i++) {
        double v = DynamicArray_get(da, i);
        if (v < m) m = v;
    }
    return m;
}

double DynamicArray_max ( const DynamicArray * da ) {
    assert(da->buffer != NULL);
    int n = DynamicArray_size(da);
    assert(n > 0);
    double m = DynamicArray_get(da, 0);
    for (int i = 1; i < n; i++) {
        double v = DynamicArray_get(da, i);
        if (v > m) m = v;
    }
    return m;
}

double DynamicArray_sum ( const DynamicArray * da ) {
    assert(da->buffer != NULL);
    int n = DynamicArray_size(da);
    double s = 0.0;
    for (int i = 0; i < n; i++) {
        s += DynamicArray_get(da, i);
    }
    return s;
}

double DynamicArray_mean ( const DynamicArray * da ) {
    assert(da->buffer != NULL);
    int n = DynamicArray_size(da);
    assert(n > 0);
    return DynamicArray_sum(da) / (double)n;
}

double DynamicArray_median ( const DynamicArray * da ) {
    assert(da->buffer != NULL);
    int n = DynamicArray_size(da);
    assert(n > 0);

    double *tmp = (double *)malloc(sizeof(double) * n);
    assert(tmp != NULL);

    for (int i = 0; i < n; i++) tmp[i] = DynamicArray_get(da, i);
    qsort(tmp, n, sizeof(double), __dbl_cmp);

    double med;
    if (n % 2 == 1) {
        med = tmp[n / 2];
    } else {
        double a = tmp[(n / 2) - 1];
        double b = tmp[n / 2];
        med = (a + b) / 2.0;
    }

    free(tmp);
    return med;
}

int DynamicArray_is_valid(const DynamicArray * da) {
    if (da == NULL) return 0;
    return da->buffer != NULL;
}

int DynamicArray_num_arrays() {
    return __all_arrays_count;
}

void DynamicArray_destroy_all() {
    for (int i = 0; i < __all_arrays_count; i++) {
        DynamicArray *da = __all_arrays[i];
        if (da != NULL && da->buffer != NULL) {
            DynamicArray_destroy(da);
        }
    }
}
