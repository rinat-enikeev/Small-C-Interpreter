#include "test.h"

char *source="test_if.test";

void testcase(void) {
	assert(!strcmp(global_vars[0].var_name, "a") && (global_vars[0].v_type==INT));
	printf("int a;  OK\n");

	assert(!strcmp(global_vars[1].var_name, "b") && (global_vars[1].v_type==INT));
	printf("int b;  OK\n");

	assert(global_vars[0].value==10);
	printf("a==10   OK\n");

	assert(global_vars[1].value==15);
	printf("b==15   OK\n");
}
