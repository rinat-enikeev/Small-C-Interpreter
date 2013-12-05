#include "test.h"

char *source="test_loops.test";

void testcase(void) {
  assert(find_var("c")==65);
  printf("c==65  OK\n");

  assert(find_var("d")==9);
  printf("d==9   OK\n");

  assert(find_var("b")==12);
  printf("b==12  OK\n");
}

