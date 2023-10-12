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

#define VecType CONCAT(TName, )
#define vec_new CONCAT(VecType, _new)
#define vec_pop CONCAT(VecType, _pop)
#define vec_get CONCAT(VecType, _get)
#define vec_push CONCAT(VecType, _push)
#define vec_free CONCAT(VecType, _free)
#define vec_print CONCAT(VecType, _print)

typedef struct {
  size_t len;
  size_t capacity;
  T *data;
} VecType;

static VecType vec_new();
static T vec_pop(VecType *vec);
static void vec_push(VecType *vec, T elem);
static void vec_free(VecType *vec);
static void vec_print(VecType *vec, void (*print_func)(T));
static T *vec_get(VecType *vec, size_t idx);

VecType vec_new() {
  VecType l = (VecType){
      .len = 0,
      .capacity = 4,
      .data = malloc(4 * sizeof(T)),
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

T *vec_get(VecType *vec, size_t idx) {
  if (vec->len <= idx) {
    printf("out of bounds error in");
    printf(" %s with idx: %li and len: %li", "#vec_get", idx, vec->len);
    exit(1);
  } else {
    return &vec->data[idx];
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