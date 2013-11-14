#ifndef EXTERN_VAR_DECL_H
#define EXTERN_VAR_DECL_H

#include <setjmp.h>

extern char token_type; /* содержит тип лексемы */
extern char tok;		/* внутреннее представление лексемы */
extern char *prog;		/* текущее положение в исходном тексте программы */
extern char token[80];	/* строковое представление лексемы */

extern char *p_buf;		/* указатель на начало буфера программы */
extern jmp_buf e_buf;   /* содержит данные для longjmp() */

#endif //EXTERN_VAR_DECL_H