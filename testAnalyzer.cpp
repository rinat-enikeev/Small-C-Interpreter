#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>

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
int load_program(char *p, char *fname);
int get_token(void);

int main(void)
{
	/* выделение памяти для программы */
	if((p_buf = (char *) malloc(PROG_SIZE))==NULL) {
		printf("Can not allocate memory");
		exit(1);
	}

	/* загрузка программы для выполнения */
	if(!load_program(p_buf, "analyzer.test")) exit(1);

	/* установка указателя программы на начало буфера программы */
	prog = p_buf;
	testcase_global_vars();
    
    

	return 0;
}

/* Тест синтаксического разбора кода: 

	int i, j; 
	char ch;

*/
void testcase_global_vars(void)
{
	cout << "Test input source is: \n" << endl;
	cout << "________________________________________ \n" << endl;
	cout << p_buf << endl;
	cout << "________________________________________ \n" << endl;
	
	prog = p_buf;

	get_token(); // "int"
	assert(tok == INT);
	assert(token_type == KEYWORD);
	cout << "1 token is INT, type is KEYWORD: OK" << endl;

	get_token(); // "i"
	assert(token_type == IDENTIFIER);
	cout << "2 token type is IDENTIFIER: OK" << endl;

	get_token(); // ","
	assert(token_type == DELIMITER);
	cout << "3 token type is DELIMITER: OK" << endl;

	get_token(); // "j"
	assert(token_type == IDENTIFIER);
	cout << "4 token type is IDENTIFIER: OK" << endl;

	get_token(); // ";"
	assert(token_type == DELIMITER);
	cout << "5 token type is DELIMITER: OK" << endl;

	get_token(); // "char"
	assert(tok == CHAR);
	assert(token_type == KEYWORD);
	cout << "6 token is CHAR, type is KEYWORD: OK" << endl;

	get_token(); // "ch"
	assert(token_type == IDENTIFIER);
	cout << "7 token type is IDENTIFIER: OK" << endl;

	get_token(); // ";"
	assert(token_type == DELIMITER);
	cout << "8 token type is DELIMITER: OK" << endl;
}