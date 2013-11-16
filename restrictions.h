#ifndef RESTRICTIONS_DECL_H
#define RESTRICTIONS_DECL_H

#define NUM_FUNC		  100
#define NUM_GLOBAL_VARS   4
#define NUM_GLOBAL_ARRAYS 4
#define NUM_LOCAL_VARS    4
#define NUM_LOCAL_ARRS    4
#define NUM_BLOCK         100
#define ID_LEN            31
#define FUNC_CALLS        31
#define NUM_PARAMS        31
#define PROG_SIZE         10000
#define LOOP_NEST         31
#define FOR_NEST          31

#define DEBUG 1

#endif // RESTRICTIONS_DECL_H

#if DEBUG 
#include <assert.h>
#include <iostream>
using namespace std;
#endif

/* Documentation */
// 1. SmallC allows only /* ... */ /* comments */