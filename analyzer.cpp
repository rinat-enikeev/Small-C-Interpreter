/* –ÂÍÛÒË‚Ì˚È ÌËÒıÓ‰ˇ˘ËÈ ÒËÌÚ‡ÍÒË˜ÂÒÍËÈ ‡Ì‡ÎËÁ‡ÚÓ
   ˆÂÎÓ˜ËÒÎÂÌÌ˚ı ‚˚‡ÊÂÌËÈ, ÒÓ‰ÂÊ‡˘Ëı ÔÂÂÏÂÌÌ˚Â
   Ë ‚˚ÁÓ‚˚ ÙÛÌÍˆËÈ.
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

// {{ from memory.cpp interpreter
int is_var(char *s);
int is_arr(char *s);
void assign_var(char *var_name, int value);
int find_var(char *s);
void assign_arr_element(char *arr_name, int position, int value);
int find_arr_element(char *arr_name, int position);
int arr_exists(char *name);
char *find_func(char *name);
void call();
// }}


// {{ core functons
int get_token(void);

void eval_exp(int *value);
void eval_exp0(int *value);
void eval_exp1(int *value);
void eval_exp2(int *value);
void eval_exp3(int *value);
void eval_exp4(int *value);
void eval_exp5(int *value);
void atom(int *value);
// }} */

// {{ analyzer inner functions
int look_up(char *s);
int isdelim(char c), iswhite(char c);
void sntx_err(int error);
void putback(void);
void arr_index_atom(int *value);
int internal_func(char *s);
char *find_func(char *name);
// }}

/* “‡·ÎËˆ‡ Á‡ÂÁÂ‚ËÓ‚‡ÌÌ˚ı ÒÎÓ‚ */
extern struct commands {
  char command[20];
  char tok;
} table[];


int print(void);

struct intern_func_type {
    char *f_name; /* имя функции */
    int (*p)();   /* указатель на функцию */
} intern_func[] = {
    "print", print,
    "", 0  /* этот список заканчивается нулем */
};


/* Точка входа в синтаксический анализатор выражений. */
void eval_exp(int *value)
{
    get_token();
    if(!*token) {
        sntx_err(NO_EXP);
        return;
    }
    if(*token == ';') {
        *value = 0; /* пустое выражение */
        return;
    }

    eval_exp0(value);
    putback(); /* возврат последней лексемы во входной поток */
}

/* Обработка выражения в присваивании */
void eval_exp0(int *value)
{
    char temp[ID_LEN];  /* содержит имя переменной,
                         которой присваивается значение */
    register int temp_tok;
    
    if(token_type == IDENTIFIER) {
        if(is_var(token)) {  /* если эта переменная,
                              посмотреть, присваивается ли ей значение */
            
            if (is_arr(token)) {
                sntx_err(SYNTAX); // todo: make message: redefinition of array is illegal
            }
            
            strcpy(temp, token);
            temp_tok = token_type;
            get_token();
            if(*token == '=') {  /* это присваивание */
                get_token();
                eval_exp0(value);  /* вычислить присваемое значение */
                assign_var(temp, *value);  /* присвоить значение */
                return;
            }
            else {  /* не присваивание */
                putback();  /* востановление лексемы */
                strcpy(token, temp);
                token_type = temp_tok;
            }
        } else if (is_arr(token)) {
            
            strcpy(temp, token);
            temp_tok = token_type;
            get_token();
            
            if (*token == '[') {
                
                get_token();
                eval_exp0(value); // вычисление выражения в [] скобках
                
                int arr_index = *value;
                
                get_token(); // '=' or ';'
                
                if(*token == '=') {  /* это присваивание */
                    get_token();
                    eval_exp0(value);  /* вычислить присваемое значение */
                    assign_arr_element(temp, arr_index, *value);  /* присвоить значение */
                    return;
                }
                else {  /* не присваивание */
                    putback();  /* востановление лексемы */
                    strcpy(token, temp);
                    token_type = temp_tok;
                }

            } else {
                sntx_err(SYNTAX);   // todo: assigning array is illegal
            }
            
        }
    }
    eval_exp1(value);
}

/* Обработка операций сравнения. */
void eval_exp1(int *value)
{
    int partial_value;
    register char op;
    char relops[7] = {
        LT, LE, GT, GE, EQ, NE, 0
    };
    
    eval_exp2(value);
    op = *token;
    if(strchr(relops, op)) {
        get_token();
        eval_exp2(&partial_value);
        switch(op) {  /* вычисление результата операции сравнения */
            case LT:
                *value = *value < partial_value;
                break;
            case LE:
                *value = *value <= partial_value;
                break;
            case GT:
                *value = *value > partial_value;
                break;
            case GE:
                *value = *value >= partial_value;
                break;
            case EQ:
                *value = *value == partial_value;
                break;
            case NE:
                *value = *value != partial_value;
                break;
        }
    }
}

/*  Суммирование или вычисление двух термов. */
void eval_exp2(int *value)
{
    register char  op;
    int partial_value;
    
    eval_exp3(value);
    while((op = *token) == '+' || op == '-') {
        get_token();
        eval_exp3(&partial_value);
        switch(op) { /* суммирование или вычитание */
            case '-':
                *value = *value - partial_value;
                break;
            case '+':
                *value = *value + partial_value;
                break;
        }
    }
}

/* Умножение или деление двух множителей. */
void eval_exp3(int *value)
{
    register char  op;
    int partial_value, t;
    
    eval_exp4(value);
    while((op = *token) == '*' || op == '/' || op == '%') {
        get_token();
        eval_exp4(&partial_value);
        switch(op) { /* умножение, деление или деление целых */
            case '*':
                *value = *value * partial_value;
                break;
            case '/':
                if(partial_value == 0) sntx_err(DIV_BY_ZERO);
                *value = (*value) / partial_value;
                break;
            case '%':
                t = (*value) / partial_value;
                *value = *value-(t * partial_value);
                break;
        }
    }
}

/* Унарный + или -. */
void eval_exp4(int *value)
{
    register char  op;
    
    op = '\0';
    if(*token == '+' || *token == '-') {
        op = *token;
        get_token();
    }
    eval_exp5(value);
    if(op) {
        if(op == '-') *value = -(*value);
    }
}

/* Обработка выражения в скобках. */
void eval_exp5(int *value)
{
    if((*token == '(')) {
        get_token();
        eval_exp0(value);   /* вычисление подвыражения */
        if(*token != ')') sntx_err(PAREN_EXPECTED);
        get_token();
    }
    else
        atom(value);
}

/* Получение значения числа, переменной или функции. */
void atom(int *value)
{
    int i;
    
    switch(token_type) {
        case IDENTIFIER:
            i = internal_func(token);
            if(i!= -1) {  /* вызов функции из "стандартной билиотеки" */
                *value = (*intern_func[i].p)();
            }
            else
                if(find_func(token)) { /* вызов функции,
                                        определенной пользователем */
                    call();
                    *value = ret_value;
                }
                else
            if (arr_exists(token)) {
                *value = find_arr_element(token, *value);
            } else {
                *value = find_var(token); /* получение значения переменной */
            }
            get_token();
            return;
        case NUMBER: /* числовая константа */
            *value = atoi(token);
            get_token();
            return;
        case DELIMITER: /* это символьная константа? */
            if(*token == '\'') {
                *value = *prog;
                prog++;
                if(*prog!='\'') sntx_err(QUOTE_EXPECTED);
                prog++;
                get_token();
                return ;
            }
            if(*token==')') return; /* обработка пустого выражения */
            else sntx_err(SYNTAX); /* синтаксическая ошибка */
        default:
            sntx_err(SYNTAX); /* синтаксическая ошибка */
    }
}

/* Возвращает идекс функции во внутренней
 библиотеке, или -1, если не найдена.
 */
int internal_func(char *s)
{
    int i;
    
    for(i=0; intern_func[i].f_name[0]; i++) {
        if(!strcmp(intern_func[i].f_name, s))  return i;
    }
    return -1;
}

/* Возврат лексемы во входной поток. */
void putback(void)
{
    char *t;
    
    t = token;
    for(; *t; t++) prog--;
}

/* —˜ËÚ˚‚‡ÌËÂ ÎÂÍÒÂÏ˚ ËÁ ‚ıÓ‰ÌÓ„Ó ÔÓÚÓÍ‡. */
int get_token(void)
{
	register char *temp; // ı‡ÌËÏ Ï‡ÒÒË‚ ÒËÏ‚ÓÎÓ‚

	token_type = 0; 
	tok = 0;
	temp = token;
	*temp = '\0';

// {{ ÔÓÔÛÒÍ ÔÓ·ÂÎÓ‚, ÒËÏ‚ÓÎÓ‚ Ú‡·ÛÎˇˆËË Ë ÔÛÒÚÓÈ ÒÚÓÍË
	while(iswhite(*prog) && *prog) ++prog;

	if(*prog == '\r' || *prog == '\n') {
		++prog;
        // COMPILER_SPECIFIC
		while(iswhite(*prog) && *prog) ++prog;
	}

// ÔÓÔÛÒÍ ÔÓ·ÂÎÓ‚, ÒËÏ‚ÓÎÓ‚ Ú‡·ÛÎˇˆËË Ë ÔÛÒÚÓÈ ÒÚÓÍË }}

// {{ end of file
	if(*prog == '\0') {
		*token = '\0';
		tok = FINISHED;
		return (token_type = DELIMITER);
	}
// end of file }}

// {{ code block
	if(strchr("{}", *prog)) { /* Ó„‡ÌË˜ÂÌËÂ ·ÎÓÍ‡ */
		*temp = *prog;
		temp++;
		*temp = '\0';
		prog++;
		return (token_type = BLOCK);
	}
// code block }}

// {{ array
	if(strchr("[]", *prog)) { /* Ó„‡ÌË˜ÂÌËÂ ·ÎÓÍ‡ */
		*temp = *prog;
		temp++;
		*temp = '\0';
		prog++;
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
	if(strchr("+-*^/%=;(),'", *prog)){ /* ‡Á‰ÂÎËÚÂÎ¸ */
		*temp = *prog;
		prog++; /* ÔÓ‰‚ËÊÂÌËÂ Ì‡ ÒÎÂ‰Û˛˘Û˛ ÔÓÁËˆË˛ */
		temp++;
		*temp = '\0';
		return (token_type = DELIMITER);
	}
// delimiter }}

// {{ string 
	if(*prog=='"') { /* ÒÚÓÍ‡ ‚ Í‡‚˚˜Í‡ı */
		prog++;
		while(*prog != '"' && *prog != '\r') *temp++ = *prog++;
		if(*prog == '\r') sntx_err(SYNTAX);
		prog++; *temp = '\0';
		return (token_type = STRING);
  }
// string }}

// {{ number constant
	if(isdigit(*prog)) { /* ˜ËÒÎÓ */
		while(!isdelim(*prog)) *temp++ = *prog++;
		*temp = '\0';
		return (token_type = NUMBER);
	}
// number constant }}

// {{ is an alphabetic letter
	if(isalpha(*prog)) { /* ÔÂÂÏÂÌÌ‡ˇ ËÎË ÓÔÂ‡ÚÓ */
		while(!isdelim(*prog)) *temp++ = *prog++;
		token_type = TEMP;
	}
// is an alphabetic letter }}

	*temp = '\0';

// {{ check if KEYWORD, otherwise identifier 
	if(token_type==TEMP) {
		tok = look_up(token); /* ÔÂÓ·‡ÁÓ‚‡Ú¸ ‚Ó ‚ÌÛÚÂÌÂÂ ÔÂ‰ÒÚ‡‚ÎÂÌËÂ */
		if(tok) {
			token_type = KEYWORD; /* ˝ÚÓ Á‡ÂÁÂ‚ËÓ‚‡ÌÌÓÂ ÒÎÓ‚Ó */
		} else {
			token_type = IDENTIFIER;
		}
	}
// }}

	return token_type;
}



/* ¬˚‚Ó‰ ÒÓÓ·˘ÂÌËˇ Ó· Ó¯Ë·ÍÂ. */
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
	while(p != prog) {  /* ÔÓËÒÍ ÌÓÏÂ‡ ÒÚÓÍË Ò Ó¯Ë·ÍÓÈ */
		p++;
		if(*p == '\r') {
			linecount++;
		}
	}
	printf(" in line %d\n", linecount);

	temp = p;
	for(i=0; i < 20 && p > p_buf && *p != '\n'; i++, p--);
	for(i=0; i < 30 && p <= temp; i++, p++) printf("%c", *p);

	longjmp(e_buf, 1); /* ‚ÓÁ‚‡Ú ‚ ·ÂÁÓÔ‡ÒÌÛ˛ ÚÓ˜ÍÛ */
}

/* ¬ÓÁ‚‡˘‡ÂÚ true (»—“»Õ¿), ÂÒÎË Ò - ‡Á‰ÂÎËÚÂÎ¸. */
int isdelim(char c)
{
	if(strchr(" !;,+-<>'/*%^=()[]", c) || c == 9 ||
		c == '\r' || c == 0) return 1;
	return 0;
}

/* ¬ÓÁ‚‡˘‡ÂÚ 1, ÂÒÎË Ò - ÔÓ·ÂÎ ËÎË Ú‡·ÛÎˇˆËˇ. */
int iswhite(char c)
{
	if(c == ' ' || c == '\t') return 1;
	else return 0;
}


/* œÓËÒÍ ‚ÌÛÚÂÌÌÂ„Ó ÔÂ‰ÒÚ‡‚ÎÂÌËˇ ÎÂÍÒÂÏ˚
   ‚ Ú‡·ÎËˆÂ ÎÂÍÒÂÏ.
*/
int look_up(char *s)
{
	register int i;
	char *p;

	/* ÔÂÓ·‡ÁÓ‚‡ÌËÂ ‚ ÌËÊÌËÈ Â„ËÒÚ */
	p = s;
	while(*p) { *p = tolower(*p); p++; }

	/* ÂÒÚ¸ ÎË ÎÂÍÒÂÏ˚ ‚ Ú‡·ÎËˆÂ? */
	for(i=0; *table[i].command; i++) {
		if(!strcmp(table[i].command, s)) {
			return table[i].tok;
		}
	}
	return 0; /* ÌÂÁÌ‡ÍÓÏ˚È ÓÔÂ‡ÚÓ */
}
