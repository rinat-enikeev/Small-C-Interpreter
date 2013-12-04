#ifndef COMMON_H
#define COMMON_H

#include "externVars.h"
#include "commonEnums.h"
#include "restrictions.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void prescan(void);
int load_program(char *p, char *fname);
char *find_func(char *name);
void call(void);
void free_arr();

#endif //COMMON_H
