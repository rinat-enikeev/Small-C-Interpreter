#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define PROG_SIZE       10000

void testcase_global_vars(void);
int load_program(char *p, char *fname);

char token_type; /* содержит тип лексемы */
char tok;		/* внутреннее представление лексемы */
char *prog;		/* текущее положение в исходном тексте программы */
char *p_buf;		/* указатель на начало буфера программы */
char token[80];	/* строковое представление лексемы */
jmp_buf e_buf;   /* содержит данные для longjmp() */

int get_token(void);

/**********************************************************************/
/* Типы лексем */
enum tok_types {DELIMITER,  // знаки пунктуации и операторы
				IDENTIFIER, // имена переменных и функций
				NUMBER,     // числовая константа
				KEYWORD,    // зарезервированные слова (while f.e.)
                TEMP,		// вспомогательный тип токена для определения
							// является токен KEYWORD или IDENTIFIER
				STRING,		// строка
				BLOCK,		// { или }
				ARRAY};		// [ или ]

/* Зарезервированные слова */
enum tokens {ARG, 
			CHAR, 
			INT, 
			IF, 
			ELSE, 
			FOR, 
			DO, 
			WHILE,
            SWITCH, 
			RETURN, 
			EOL, 
			FINISHED, 
			END};

struct commands { /* таблица зарезервированных слов */
	char command[20];
	char tok;
} table[] = { /* В эту таблицу */
	"if", IF, /* команды должны быть введены на нижнем регистре. */
	"else", ELSE,
	"for", FOR,
	"do", DO,
	"while", WHILE,
	"char", CHAR,
	"int", INT,
	"return", RETURN,
	"end", END,
	"", END  /* конец таблицы */
};


/* Эти константы используются для вызова функции sntx_err()
   в случае синтаксической ошибки. 
   ВНИМАНИЕ: константа SYNTAX используется тогда, когда
   интерпритатор не может квалифицировать ошибку.
*/
enum error_msg
     {SYNTAX, UNBAL_PARENS, NO_EXP, EQUALS_EXPECTED,
      NOT_VAR, PARAM_ERR, SEMI_EXPECTED,
      UNBAL_BRACES, FUNC_UNDEF, TYPE_EXPECTED,
      NEST_FUNC, RET_NOCALL, PAREN_EXPECTED,
      WHILE_EXPECTED, QUOTE_EXPECTED, NOT_TEMP,
      TOO_MANY_LVARS, DIV_BY_ZERO};

enum double_ops {LT=1,  // < 
				 LE,	// <=
				 GT,	// >
				 GE,	// >=
				 EQ,	// == 
				 NE};	// !=

/**********************************************************************/




int main(void)
{
	/* выделение памяти для программы */
	if((p_buf = (char *) malloc(PROG_SIZE))==NULL) {
		printf("Can not allocate memory");
		exit(1);
	}

	/* загрузка программы для выполнения */
	if(!load_program(p_buf, "vars.test")) exit(1);

	cout << p_buf << endl;

	/* установка указателя программы на начало буфера программы */
	prog = p_buf;
	testcase_global_vars(); 

	return 0;
}

/* Загрузка программы. */
int load_program(char *p, char *fname)
{
	FILE *fp;
	int i=0;

	if((fp=fopen(fname, "rb"))==NULL) return 0;

	i = 0;
	do {
		*p = getc(fp);
		p++; i++;
	} while(!feof(fp) && i<PROG_SIZE);

	if(*(p-2) == 0x1a) *(p-2) = '\0'; /* программа кончается
									нулевым символом */
	else *(p-1) = '\0';
	fclose(fp);
	return 1;
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
}