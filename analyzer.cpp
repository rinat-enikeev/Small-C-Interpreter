/* Рекурсивный нисходящий синтаксический анализатор
   целочисленных выражений, содержащих переменные
   и вызовы функций.
*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#ifndef EXTERN_VAR_DECL_H
#include "externVars.h"
#endif

#ifndef COMMON_ENUMS_DECL_H
#include "commonEnums.h"
#endif

#ifndef RESTRICTIONS_DECL_H
#include "restrictions.h"
#endif

#if DEBUG
#include <iostream>
using namespace std;
#endif


int get_token(void);
int look_up(char *s);

/* Выделяет лексемы из исходного текста программы 
	и возвращает их в качестве своего значения */
int isdelim(char c), iswhite(char c);
void sntx_err(int error);

/* Таблица зарезервированных слов */
extern struct commands {
  char command[20];
  char tok;
} table[];

/* Считывание лексемы из входного потока. */
int get_token(void)
{
#if DEBUG
		cout << "Token type ";
#endif	

	register char *temp; // храним массив символов

	token_type = 0; 
	tok = 0;
	temp = token;
	*temp = '\0';

// {{ пропуск пробелов, символов табуляции и пустой строки
	while(iswhite(*prog) && *prog) ++prog;

	if(*prog == '\r') {
		++prog;
		++prog;
		while(iswhite(*prog) && *prog) ++prog;
	}
// пропуск пробелов, символов табуляции и пустой строки }}

// {{ end of file
	if(*prog == '\0') {
		*token = '\0';
		tok = FINISHED;
		return (token_type = DELIMITER);
	}
// end of file }}

// {{ code block
	if(strchr("{}", *prog)) { /* ограничение блока */
		*temp = *prog;
		temp++;
		*temp = '\0';
		prog++;
		return (token_type = BLOCK);
	}
// code block }}

// {{ array
	if(strchr("[]", *prog)) { /* ограничение блока */
		*temp = *prog;
		temp++;
		*temp = '\0';
		prog++;
#if DEBUG
		cout << " ARRAY ";
		cout << token_type << endl;
#endif
		return (token_type = ARRAY);
	}
// array }}

// {{ is comment
	if(*prog == '/') {
		if(*(prog+1) == '*') {
			prog += 2;
			do { 
				while(*prog != '*') prog++;
				prog++;
			} while (*prog != '/');
			prog++;
		}
	}
// comment }}

// {{ comp. operator
	if(strchr("!<>=", *prog)) { 
		switch(*prog) {
			case '=': if(*(prog+1) == '=') { // ==
				prog++; prog++;
				*temp = EQ;
				temp++; *temp = EQ; temp++;
				*temp = '\0';
			} break;
			case '!': if(*(prog+1) == '=') { // !=
				prog++; prog++;
				*temp = NE;
				temp++; *temp = NE; temp++;
				*temp = '\0';
			} break;
			case '<': 
				if(*(prog+1) == '=') { // <=
				prog++; prog++;
				*temp = LE; temp++; *temp = LE;
				}
				else {                 // <
					prog++;
					*temp = LT;
				}
				temp++;
				*temp = '\0';
				break;
			case '>': 
				if(*(prog+1) == '=') { // >=
					prog++; prog++;
					*temp = GE; temp++; *temp = GE;
				}
				else {                // >
					prog++;
					*temp = GT;
				}
				temp++;
				*temp = '\0';
				break;
		}
		if(*token) return(token_type = DELIMITER);
	}
// comp. operator}}

// {{ delimiter 
	if(strchr("+-*^/%=;(),'", *prog)){ /* разделитель */
		*temp = *prog;
		prog++; /* продвижение на следующую позицию */
		temp++;
		*temp = '\0';
		return (token_type = DELIMITER);
	}
// delimiter }}

// {{ string 
	if(*prog=='"') { /* строка в кавычках */
		prog++;
		while(*prog != '"' && *prog != '\r') *temp++ = *prog++;
		if(*prog == '\r') sntx_err(SYNTAX);
		prog++; *temp = '\0';
		return (token_type = STRING);
  }
// string }}

// {{ number constant
	if(isdigit(*prog)) { /* число */
		while(!isdelim(*prog)) *temp++ = *prog++;
		*temp = '\0';
		return (token_type = NUMBER);
	}
// number constant }}

// {{ is an alphabetic letter
	if(isalpha(*prog)) { /* переменная или оператор */
#if DEBUG
		cout << " alphabetical: ";
#endif		
		while(!isdelim(*prog)) *temp++ = *prog++;
		token_type = TEMP;
	}
// is an alphabetic letter }}

	*temp = '\0';

// {{ check if KEYWORD, otherwise identifier 
	if(token_type==TEMP) {
		tok = look_up(token); /* преобразовать во внутренее представление */
		if(tok) {
#if DEBUG
			cout << " KEYWORD" << endl;
#endif	
			token_type = KEYWORD; /* это зарезервированное слово */
		} else {
#if DEBUG
			cout << " IDENTIFIER" << endl;
#endif	
			token_type = IDENTIFIER;
		}
	}
// }}

	return token_type;
}



/* Вывод сообщения об ошибке. */
void sntx_err(int error)
{
	char *p, *temp;
	int linecount = 0;
	register int i;

	static char *e[]= {
	"синтаксическая ошибка",
	"несбалансированные скобки",
	"выражение отсутствует",
	"ожидается знак равенства",
	"не переменная",
	"ошибка в параметре",
	"ожидается точка с запятой",
	"несбалансированные фигурные скобки",
	"функция не определена",
	"ожидается спецификатор типа",
	"слишком много вложенных вызовов функций",
	"оператор return вне функции",
	"ожидаются скобки",
	"ожидается while",
	"ожидается закрывающаяся кавычка",
	"не строка",
	"слишком много локальных переменных",
	"деление на нуль"
	};
	printf("\n%s", e[error]);
	p = p_buf;
	while(p != prog) {  /* поиск номера строки с ошибкой */
		p++;
		if(*p == '\r') {
			linecount++;
		}
	}
	printf(" in line %d\n", linecount);

	temp = p;
	for(i=0; i < 20 && p > p_buf && *p != '\n'; i++, p--);
	for(i=0; i < 30 && p <= temp; i++, p++) printf("%c", *p);

	longjmp(e_buf, 1); /* возврат в безопасную точку */
}

/* Возвращает true (ИСТИНА), если с - разделитель. */
int isdelim(char c)
{
	if(strchr(" !;,+-<>'/*%^=()[]", c) || c == 9 ||
		c == '\r' || c == 0) return 1;
	return 0;
}

/* Возвращает 1, если с - пробел или табуляция. */
int iswhite(char c)
{
	if(c == ' ' || c == '\t') return 1;
	else return 0;
}


/* Поиск внутреннего представления лексемы
   в таблице лексем.
*/
int look_up(char *s)
{
	register int i;
	char *p;

	/* преобразование в нижний регистр */
	p = s;
	while(*p) { *p = tolower(*p); p++; }

	/* есть ли лексемы в таблице? */
	for(i=0; *table[i].command; i++) {
		if(!strcmp(table[i].command, s)) {

#if DEBUG
			cout << " from table: ";
			cout << s;
#endif

			return table[i].tok;
		}
	}
	return 0; /* незнакомый оператор */
}

/* Возврат лексемы во входной поток. */
void putback(void)
{
  char *t;

  t = token;
  for(; *t; t++) prog--;
}