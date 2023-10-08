#include "array.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


Vec vec_new() {
  Vec l = (Vec){
      .len = 0,
      .capacity = 4,
      .data = malloc(0),
  };
  return l;
}

void vec_push(Vec *vec, int elem) {
  if (vec->len + 1 > vec->capacity) {
    vec->capacity *= 2;
    int *newdata = malloc(vec->capacity * sizeof(int));

    for (size_t i = 0; i < vec->len; i++) {
      newdata[i] = vec->data[i];
    }
    free(vec->data);
    vec->data = newdata;
  }
  vec->data[vec->len] = elem;
  vec->len++;
}

int vec_pop(Vec *vec) {
  if (vec->len != 0) {
    vec->len--;
    return vec->data[vec->len];
  } else {
    printf("ne fait pas ça");
    exit(1);
  }
}

void vec_free(Vec *vec) { free(vec->data); }

void vec_print(Vec *vec) {
  for (size_t i = 0; i < vec->len; i++) {
    printf("-%i", vec->data[i]);
  }
}

void test(){
    assert(0);
}