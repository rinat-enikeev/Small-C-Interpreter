#include "../tests/test.h"

char *source="test_funcs.test";

void testcase(void) {
	assert(find_var("b")==3);
	printf("b==3      OK\n");

	assert(find_var("c")==7);
	printf("c==7      OK\n");

	assert(find_var("g")==120);
	printf("g==120    OK\n");

	assert(find_var("h")==20);
	printf("h==20    OK\n");

	assert(garr_index == 1);
	printf("global array index == 1: OK \n");
	assert(global_arrays[0].arr_type == INT);
	printf("global array type is INT: OK \n");
	int i;
	for(i=0; i < 10; ++i) {
		printf("%d\n",global_arrays[0].int_arr[i]);
	}
	assert(global_arrays[0].int_arr[0]==30);
	printf("arr_glob[0]==30  OK\n");
	assert(global_arrays[0].int_arr[9]==21);
	printf("arr_glob[9]==21  OK\n");
}
