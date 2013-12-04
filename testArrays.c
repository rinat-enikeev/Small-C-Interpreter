#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#ifndef COMMON_ENUMS_DECL_H
#include "commonEnums.h"
#endif

#ifndef RESTRICTIONS_DECL_H
#include "restrictions.h"
#endif

#ifndef EXTERN_VAR_DECL_H
#include "externVars.h"
#endif


void testcase_global_vars(void);

// {{ from memory.cpp
int load_program(char *p, char *fname);
int get_token(void);
void prescan(void);
char *find_func(char *name);
void call(void);
void free_arr();
/* }} */
// }}

int main(void) {
  if((p_buf = (char *) malloc(PROG_SIZE))==NULL) {
    printf("Can not allocate memory");
    exit(1);
  }
  char *source = "arrays.test";
  if(!load_program(p_buf, source)) {
    exit(1);
  }

  if(setjmp(e_buf)) {
    exit(1);  /* инициализация буфера long jump */
  }

  gvar_index = 0;  /* инициализация индекса глобальных переменных */


  /* установка указателя программы на начало буфера программы */
  prog = p_buf;
  prescan(); /* определение адресов всех функций
                и глобальных переменных программы */

  lvartos = 0;     /* инициализация индекса стека локальных переменных */
  functos = 0;     /* инициализация индекса стека вызова (CALL) */

  /* первой вызывается main() */
  char *main = "main";
  prog = find_func(main); /* поиск точки входа программы */

  if(!prog) { /* функция main() неправильна или отсутствует */
    printf("main() не найдена.\n");
    exit(1);
  }

  prog--; /* возврат к открывающейся скобке ( */
  strcpy(token, "main");
  call(); /* начало интерпритации main() */

  // free memory from global arrays
  free_arr();

  return 0;
}