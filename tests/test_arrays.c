#include "test.h"

char *source="test_arrays.test";

void testcase(void) {
	assert(find_var("result1"));
	printf("result1      OK\n");

	assert(find_var("result2"));
	printf("result2      OK\n");

	assert(find_var("result3"));
	printf("result3      OK\n");
}

