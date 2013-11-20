#ifndef RESTRICTIONS_DECL_H
#define RESTRICTIONS_DECL_H

#define NUM_FUNC		  100   /* максимальное количество функций */
#define NUM_GLOBAL_VARS   400   /* максимальное количество глобальных переменных */
#define NUM_GLOBAL_ARRAYS 400   /* максимальное количество глобальных массивов */
#define NUM_LOCAL_VARS    400   /* максимальное количество локальных переменных */
#define NUM_LOCAL_ARRS    400   /* максимальное количество глобальных массивов */
#define NUM_BLOCK         100   /* максимальная вложенность блоков кода*/
#define ID_LEN            31    /* максимальная длина идентификатора*/
#define NUM_PARAMS        31    /* максимальное количество параметров функции */
#define PROG_SIZE         10000 /* максимальный	размер программы в символах */

#define DEBUG 1

#endif // RESTRICTIONS_DECL_H

#if DEBUG 
#include <assert.h>
#include <iostream>
using namespace std;
#endif

/* Documentation */
// 1. SmallC allows only /* ... */ /* comments */