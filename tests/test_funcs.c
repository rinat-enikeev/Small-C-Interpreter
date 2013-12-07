#include "test.h"

char *source="test_funcs.test";

void testcase(void) {
  assert(find_var("b")==3);
  printf("b==3      OK\n");

  assert(find_var("c")==7);
  printf("c==7      OK\n");

  assert(find_var("g")==120);
  printf("g==120    OK\n");
}

