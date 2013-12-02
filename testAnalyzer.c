#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

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
    char *source = "analyzer.test";
	if(!load_program(p_buf, source)) exit(1);

	prog = p_buf;
	testcase_global_vars();

	return 0;
}

void testcase_global_vars(void)
{
	printf("Test input source is: \n");
    printf("________________________________________ \n");
    printf(p_buf);
    printf("________________________________________ \n");
	
	prog = p_buf;

	get_token(); // "int"
	assert(tok == INT);
	assert(token_type == KEYWORD);
    printf("1 token is INT, type is KEYWORD: OK \n");

	get_token(); // "i"
	assert(token_type == IDENTIFIER);
	printf("2 token type is IDENTIFIER: OK \n");

	get_token(); // ","
	assert(token_type == DELIMITER);
	printf("3 token type is DELIMITER: OK \n");

	get_token(); // "j"
	assert(token_type == IDENTIFIER);
	printf("4 token type is IDENTIFIER: OK \n");

	get_token(); // ";"
	assert(token_type == DELIMITER);
	printf("5 token type is DELIMITER: OK \n");

	get_token(); // "char"
	assert(tok == CHAR);
	assert(token_type == KEYWORD);
	printf("6 token is CHAR, type is KEYWORD: OK \n");

	get_token(); // "ch"
	assert(token_type == IDENTIFIER);
	printf("7 token type is IDENTIFIER: OK \n");

	get_token(); // ";"
	assert(token_type == DELIMITER);
	printf("8 token type is DELIMITER: OK \n");
    
    get_token(); // int
    assert(token_type == KEYWORD);
    printf("9 token type is KEYWORD: OK \n");
    
    get_token(); // main
    assert(token_type == IDENTIFIER);
    printf("10 token type is IDENTIFIER: OK \n");
    
    get_token(); // (
    assert(token_type == DELIMITER);
    printf("11 token type is DELIMITER: OK \n");
    
    get_token(); // )
    assert(token_type == DELIMITER);
    printf("12 token type is DELIMITER: OK \n");
    
    get_token(); // {
    assert(token_type == BLOCK);
    printf("13 token type is BLOCK: OK \n");
    
    get_token(); // {
    assert(token_type == BLOCK);
    printf("14 token type is BLOCK: OK \n");
    
}