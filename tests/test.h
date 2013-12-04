#ifndef TEST_H
#define TEST_H

#include "../common.h"
#include "../lib.c"
#include "../interpreter.c"
#include "../analyzer.c"
#include <assert.h>

void print_source(void);
void testcase(void);
extern char *source;

int main(int argc, char *argv[]) {
  if((p_buf = (char *)malloc(PROG_SIZE))==NULL) {
    printf("Выделить память не удалось");
    exit(1);
  }
  if(!load_program(p_buf, source)) {
    exit(1);
  }
  print_source();
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
  char *main_ = "main";
  prog = find_func(main_); /* поиск точки входа программы */
  if(!prog) { /* функция main() неправильна или отсутствует */
    printf("main() не найдена.\n");
    exit(1);
  }
  prog--; /* возврат к открывающейся скобке ( */
  strcpy(token, "main");
  call(); /* начало интерпритации main() */
  // free memory from global arrays
  free_arr();
  testcase();
  return 0;
}

void print_source(void) {
  printf("Test input source is: \n");
  printf("________________________________________\n");
  printf(p_buf);
  printf("________________________________________\n");
}

#endif //TEST_H
