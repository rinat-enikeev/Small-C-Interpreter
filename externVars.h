#ifndef EXTERN_VAR_DECL_H
#define EXTERN_VAR_DECL_H
#include <setjmp.h>
extern char token_type; /* содержит тип лексемы из перечисления tok_types */
extern char tok;        /* внутреннее представление лексемы */
extern char *prog;      /* текущая позиция в исходном тексте программы */
extern char token[80];  /* строковое представление лексемы */
extern char *p_buf;     /* указатель на начало буфера программы */
extern jmp_buf e_buf;   /* содержит данные для longjmp() */
extern int gvar_index;  /* индекс в таблице глобальных переменных */
extern int garr_index;  /* индекс в таблице глобальных массивов */
extern int lvartos;     /* индекс в стеке локальных переменных */
extern int larrtos;     /* индекс в стеке локальных переменных */
extern int functos;     /* индекс вершины стека вызова функции */
extern int ret_value;   /* возвращаемое значение функции */
#endif //EXTERN_VAR_DECL_H
