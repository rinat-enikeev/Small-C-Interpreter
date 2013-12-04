#include "common.h"
#include <assert.h>

// {{* added array functionality
extern struct array_type {
  char arr_name[ID_LEN];
  int arr_type;
  void *value;
} global_arrays[NUM_GLOBAL_ARRAYS];
// added array functionality *}}

extern int garr_index;

void testcase_arrays(void);

int load_program(char *p, char *fname);
int get_token(void);
void prescan(void);

int main(void) {
  if((p_buf = (char *) malloc(PROG_SIZE))==NULL) {
    printf("Can not allocate memory");
    exit(1);
  }

  char *source = "prescan.test";
  if(!load_program(p_buf, source)) {
    exit(1);
  }
  prog = p_buf;
  testcase_arrays();

  return 0;
}

void testcase_arrays(void) {
  printf("Test input source is: \n");
  printf("________________________________________\n");
  printf(p_buf);
  printf("________________________________________\n");

  printf("entering prescan...\n");
  prescan(); // int [] array;
  printf("prescan ended.\n");

  assert(garr_index == 1);
  printf("global array index == 1: OK \n");
  assert(global_arrays[0].arr_type == INT);
  printf("global array type is INT: OK \n");
  assert(!strcmp(global_arrays[0].arr_name, "array"));
  printf("global array name is 'array': OK \n");
}
