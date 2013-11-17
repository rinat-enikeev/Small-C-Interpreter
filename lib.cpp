#ifndef EXTERN_VAR_DECL_H
#include "externVars.h"
#endif

#ifndef COMMON_ENUMS_DECL_H
#include "commonEnums.h"
#endif

#include <stdio.h>

// {{ analyzer.cpp functions
int get_token(void);
void putback(void);
void sntx_err(int error);
void eval_exp(int *value);
// }}

/* Встроенная функция консольного вывода. */
int print(void)
{
    int i;
    
    get_token();
    if(*token!='(')  sntx_err(PAREN_EXPECTED);
    
    get_token();
    if(token_type==STRING) { /* вывод строки */
        printf("%s ", token);
    }
    else {  /* вывод числа */
        putback();
        eval_exp(&i);
        printf("%d ", i);
    }
    
    get_token();
    
    if(*token!=')') sntx_err(PAREN_EXPECTED);
    
    get_token();
    if(*token!=';') sntx_err(SEMI_EXPECTED);
    putback();
    return 0;
}