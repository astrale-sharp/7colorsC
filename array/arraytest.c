#include <assert.h>
#include <stdio.h>
#define T int
#define TName iVec
#include "array.h"
#undef TName
#undef T

#define T iVec
#define TName xyVec
#include "array.h"
#undef T
#undef TName

void pint(int i) { printf("-%i", i); }

void pvint(iVec v) { iVec_print(&v, pint); }

void test() {
  xyVec mega = xyVec_new();
  iVec v = iVec_new();
  iVec v2 = iVec_new();
  iVec_push(&v, 4);
  iVec_push(&v, 6);
  iVec_push(&v, 8);

  iVec_push(&v2, 1);
  iVec_push(&v2, 2);
  iVec_push(&v2, 3);

  xyVec_push(&mega, v);
  xyVec_push(&mega, v2);

  assert(*iVec_get(&v, 0) == 4);
  assert(*iVec_get(&v, 1) == 6);
  assert(*iVec_get(&v, 2) == 8);

  assert(v.data[0] == 4);
  assert(v.data[1] == 6);
  assert(v.data[2] == 8);

  assert(v2.data[0] == 1);
  assert(v2.data[1] == 2);
  assert(v2.data[2] == 3);

  assert(mega.data[0].data == v.data);
  assert(mega.data[1].data == v2.data);

  assert(mega.data[0].data[0] == 4);
  assert(mega.data[0].data[1] == 6);
  assert(mega.data[0].data[2] == 8);

  iVec_print(&v, pint);
  iVec_print(&v2, pint);

  xyVec_print(&mega, pvint);
}

void test2() {}
