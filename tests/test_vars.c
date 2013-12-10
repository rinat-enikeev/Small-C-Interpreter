#include "test.h"

char *source="test_vars.test";

void testcase(void) {
	assert(find_var("a")==5);
	printf("a==5  OK\n");

	assert(find_var("b")==2);
	printf("b==2  OK\n");

	assert(find_var("c")==6);
	printf("c==6  OK\n");

	assert(find_var("d")==4);
	printf("d==4  OK\n");
}

