#pragma once
#include <stddef.h>

typedef struct {
  size_t len;
  size_t capacity;
  int *data;
} Vec;

Vec vec_new();
int vec_pop(Vec *vec);
void vec_push(Vec *vec, int elem);
void vec_free(Vec *vec);
void vec_print(Vec *vec);
