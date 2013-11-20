#include <string.h>
#include <stdlib.h>

#ifndef EXTERN_VAR_DECL_H
#include "externVars.h"
#endif

#ifndef COMMON_ENUMS_DECL_H
#include "commonEnums.h"
#endif

#ifndef RESTRICTIONS_DECL_H
#include "restrictions.h"
#endif


/* {{ functions from memory.cpp */
void prescan(void);
int load_program(char *p, char *fname);
char *find_func(char *name);
void call(void);
void free_arr();
 /* }} */


int main(int argc, char *argv[])
{
    printf("Курсовая работа по курсу Среды программирования.\n");
    printf("Интерпретатор small C. Авторы: Ринат Еникеев, Айнур Ишбулдин.\n");
    
    if(argc != 2) {
        printf("Применение: littlec <имя_файла_с_исходным_кодом>\n");
        exit(1);
    }
    
    /* выделение памяти для программы */
    if((p_buf = (char *) malloc(PROG_SIZE))==NULL) {
        printf("Выделить память не удалось");
        exit(1);
    }
    
    /* загрузка программы для выполнения */
    if(!load_program(p_buf, argv[1])) exit(1);
    if(setjmp(e_buf)) exit(1); /* инициализация буфера long jump */
    
    gvar_index = 0;  /* инициализация индекса глобальных переменных */
    
    
    /* установка указателя программы на начало буфера программы */
    prog = p_buf;
    prescan(); /* определение адресов всех функций
                и глобальных переменных программы */
    
    lvartos = 0;     /* инициализация индекса стека локальных переменных */
    functos = 0;     /* инициализация индекса стека вызова (CALL) */
    
    /* первой вызывается main() */
    prog = find_func("main"); /* поиск точки входа программы */
    
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