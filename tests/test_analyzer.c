#include "../common.h"
#include "../lib.c"
#include "../interpreter.c"
#include "../analyzer.c"
#include <locale.h>
#include <assert.h>
void testcase(void);
int main(void) {
	//setlocale(LC_ALL, "Russian");
	if((p_buf = (char *) malloc(PROG_SIZE))==NULL) {
		printf("Выделить память не удалось");
		exit(1);
	}
	char *source = "test_analyzer.test";
	if(!load_program(p_buf, source)) {
		exit(1);
	}
	prog = p_buf;
	testcase();
	return 0;
}
void testcase(void) {
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
