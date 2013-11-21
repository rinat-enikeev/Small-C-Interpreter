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

// {{ from memory.cpp
int load_program(char *p, char *fname);
int get_token(void);
// }}

int main(void)
{
	if((p_buf = (char *) malloc(PROG_SIZE))==NULL) {
		printf("Can not allocate memory");
		exit(1);
	}
    char *source = new char[13];
    strcpy(source, "analyzer.test");
	if(!load_program(p_buf, source)) exit(1);

	prog = p_buf;
	testcase_global_vars();

	return 0;
}

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
    
    get_token(); // int
    assert(token_type == KEYWORD);
    cout << "9 token type is KEYWORD: OK" << endl;
    
    get_token(); // main
    assert(token_type == IDENTIFIER);
    cout << "10 token type is IDENTIFIER: OK" << endl;
    
    get_token(); // (
    assert(token_type == DELIMITER);
    cout << "11 token type is DELIMITER: OK" << endl;
    
    get_token(); // )
    assert(token_type == DELIMITER);
    cout << "12 token type is DELIMITER: OK" << endl;
    
    get_token(); // {
    assert(token_type == BLOCK);
    cout << "13 token type is BLOCK: OK" << endl;
    
    get_token(); // {
    assert(token_type == BLOCK);
    cout << "14 token type is BLOCK: OK" << endl;
    
}