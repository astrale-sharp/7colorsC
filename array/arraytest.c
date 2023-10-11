#include <assert.h>
#define T int
#define TName int
#include "array.h"
#undef TName
#undef T

#define T Vec_int
#define TName Vec_int
#include "array.h"
#undef T
#undef TName

void pint(int i) { printf("-%i", i); }

void pvint(Vec_int v) {
    vec_print_int(&v,pint);
}

void test() {
    Vec_Vec_int mega = vec_new_Vec_int();
  Vec_int v = vec_new_int();
  Vec_int v2 = vec_new_int();
  vec_push_int(&v, 4);
  vec_push_int(&v, 6);
  vec_push_int(&v, 8);

  vec_push_int(&v2, 1);
  vec_push_int(&v2, 2);
  vec_push_int(&v2, 3);
//   ARNAUDTODO uncomment
//   vec_push_Vec_int(&mega, v2);
//   vec_push_Vec_int(&mega, v);

  vec_print_int(&v, pint);
  vec_print_int(&v2, pint);

//   vec_print_Vec_int(&mega, pvint);

//   assert(8 == vec_pop_int(&v));
//   assert(6 == vec_pop_int(&v));
//   assert(4 == vec_pop_int(&v));
}

void test2() {}
