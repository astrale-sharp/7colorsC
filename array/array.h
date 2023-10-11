#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef T
#error "No type defined !"
#endif

#ifndef TName
#error "No type defined !"
#endif

#define CONCATBIS(A, B) A##B
#define CONCAT(A, B) CONCATBIS(A, B)

#define VecType CONCAT(Vec_, TName)
#define vec_new CONCAT(vec_new_, TName)
#define vec_pop CONCAT(vec_pop_, TName)
#define vec_push CONCAT(vec_push_, TName)
#define vec_free CONCAT(vec_free_, TName)
#define vec_print CONCAT(vec_print_, TName)

typedef struct {
  size_t len;
  size_t capacity;
  T *data;
} VecType;

VecType vec_new();
T vec_pop(VecType *vec);
void vec_push(VecType *vec, T elem);
void vec_free(VecType *vec);
void vec_print(VecType *vec, void (*print_func)(T));

VecType vec_new() {
  VecType l = (VecType){
      .len = 0,
      .capacity = 4,
      .data = malloc(0),
  };
  return l;
}

void vec_push(VecType *vec, T elem) {
  if (vec->len + 1 > vec->capacity) {
    vec->capacity *= 2;
    T *newdata = malloc(vec->capacity * sizeof(T));

    for (size_t i = 0; i < vec->len; i++) {
      newdata[i] = vec->data[i];
    }
    free(vec->data);
    vec->data = newdata;
  }
  vec->data[vec->len] = elem;
  vec->len++;
}

T vec_pop(VecType *vec) {
  if (vec->len != 0) {
    vec->len--;
    return vec->data[vec->len];
  } else {
    printf("panic vecT pop");
    exit(1);
  }
}

void vec_free(VecType *vec) { free(vec->data); }

void vec_print(VecType *vec, void (*print_func)(T)) {
  for (size_t i = 0; i < vec->len; i++) {
    print_func(vec->data[i]);
  }
  printf("\n");
}

#undef CONCATBIS
#undef CONCAT
#undef VecType
#undef vec_new
#undef vec_pop
#undef vec_push
#undef vec_free
#undef vec_print