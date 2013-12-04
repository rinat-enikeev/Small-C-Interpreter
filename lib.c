#include "externVars.h"
#include "commonEnums.h"
#include "analyzer.h"
#include "libc.h"
#include <stdio.h>
#include <stdlib.h>

/* Вывод символа на экран. */
int call_put_char() {
  int value;

  eval_exp(&value);
  printf("%c", value);
  return value;
}

/* Вызов функции puts(). */
int call_put_string(void) {
  get_token();
  if(*token!='(') {
    sntx_err(PAREN_EXPECTED);
  }
  get_token();
  if(token_type!=STRING) {
    sntx_err(QUOTE_EXPECTED);
  }
  puts(token);
  get_token();
  if(*token!=')') {
    sntx_err(PAREN_EXPECTED);
  }

  get_token();
  if(*token!=';') {
    sntx_err(SEMI_EXPECTED);
  }
  putback();
  return 0;
}

/* Считывание символа с консоли. Если компилятор
 не поддерживает _getche(), то следует
 использвать getchar() */
int call_getche() {
  char ch;
  ch = getchar();
  while(*prog!=')') {
    prog++;
  }
  prog++;   /* продвижение к концу строки */
  return ch;
}

/* Считывание целого числа с клавиатуры. */
int getnum(void) {
  char s[80];

  gets(s);
  while(*prog != ')') {
    prog++;
  }
  prog++;  /* продвижение к концу строки */
  return atoi(s);
}

/* Встроенная функция консольного вывода. */
int print(char *s) {
  int i;

  get_token();
  if(*token!='(') {
    sntx_err(PAREN_EXPECTED);
  }

  get_token();
  if(token_type==STRING) { /* вывод строки */
    printf("%s ", token);
  } else { /* вывод числа */
    putback();
    eval_exp(&i);
    printf("%d ", i);
  }

  get_token();

  if(*token!=')') {
    sntx_err(PAREN_EXPECTED);
  }

  get_token();
  if(*token!=';') {
    sntx_err(SEMI_EXPECTED);
  }
  putback();
  return 0;
}
