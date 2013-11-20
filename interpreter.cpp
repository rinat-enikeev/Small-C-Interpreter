#ifndef EXTERN_VAR_DECL_H
#include "externVars.h"
#endif

#ifndef COMMON_ENUMS_DECL_H
#include "commonEnums.h"
#endif

#ifndef RESTRICTIONS_DECL_H
#include "restrictions.h"
#endif


/* {{ entry point to analyzer.cpp */
void eval_exp(int *value);
int get_token(void);
// }}


char token_type; /* тип токена */
char tok;		 /* внутреннее представление лексемы */
char *prog;      /* текущая позиция в исходном тексте программы */
char *p_buf;     /* указывает на начало буфера программы */
char token[80];  /* строковое представление лексемы */
jmp_buf e_buf;   /* содержит информацию для longjmp() */

int functos;	/* индекс вершины стека вызова функции */
int func_index; /* индекс в таблице функций */
int gvar_index; /* индекс в таблице глобальных переменных */
int garr_index; /* index of global arrays */
int lvartos;    /* индекс в стеке локальных переменных */
int larrtos;	/* индекс в стеке локальных переменных */

struct commands { /* Таблица зарезервированных слов */
    char command[20];
    char tok;
} table[] = {  /* В эту таблицу */
    "if", IF,  /* команды должны быть введены на нижнем регистре. */
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

struct func_type {
    char func_name[ID_LEN];
    int ret_type;
    char *loc;  /* адрес точки входа в файл */
} func_table[NUM_FUNC];

int call_stack[NUM_FUNC];

/* Массив этих структур содержит информацию
   о глобальных переменных.
*/
struct var_type {
    char var_name[ID_LEN];
    int v_type;
    int value;
}  global_vars[NUM_GLOBAL_VARS];

struct var_type local_var_stack[NUM_LOCAL_VARS];

// {{* added array functionality
struct array_type {
    char arr_name[ID_LEN];
    int arr_type;
    int *int_arr;
    char *char_arr;
    int length;
} global_arrays[NUM_GLOBAL_ARRAYS];
// added array functionality *}}

struct array_type local_arr_stack[NUM_LOCAL_ARRS];


void decl_global(void), sntx_err(int error), putback(void), decl_global_array(void);
int load_program(char *p, char *fname);
char *find_func(char *name);
void call(void);
void get_params(void), get_args(void), func_push(int i);
int  func_pop(void);
void func_ret(void);
void local_push(struct var_type i);
void free_arr();

int ret_value; /* возвращаемое значение функции */

// {{ program flow functions
void exec_if(void);
void find_eob(void);
void exec_while(void);
void exec_do(void);
void exec_for(void);
// }}



/* Найти адреса всех функций в программе
 и запомнить глобальные переменные. */
void prescan(void)
{
    char *p, *tp;
    char temp[32];
    int datatype;
    int brace = 0;  /* Если brace = 0, то текущая
                     позиция указателя программы находится
                     в не какой-либо функции. */
    
    p = prog;
    func_index = 0;
    
	do {
		while(brace) {  /* обход кода функции */
			get_token();
			if(*token == '{') brace++;
			if(*token == '}') brace--;
		}
        
        tp = prog; /* запоминание текущей позиции */
        get_token();
        
        /* тип глобальной переменной или возвращаемого значения функции */
        if(tok==CHAR || tok==INT) {
            datatype = tok; /* запоминание типа данных */
            
			get_token();
			if(token_type == IDENTIFIER) {
				strcpy(temp, token);
				get_token();
                if (token_type == ARRAY) {
                    prog = tp;
                    decl_global_array();
                } else if(*token != '(') { /* это должна быть глобальная переменная */
					prog = tp; /* возврат в начало объявления */
					decl_global();
				} else if(*token == '(') {  /* это должна быть функция */
					func_table[func_index].loc = prog;
					func_table[func_index].ret_type = datatype;
					strcpy(func_table[func_index].func_name, temp);
					func_index++;
					while(*prog != ')') prog++;
					prog++;
                    /* сейчас prog указывает на открывающуюся
                     фигурную скобку функции */
				} else {
					putback();
				}
			}
		} else if(*token == '{') {
			brace++;
		}
	} while(tok != FINISHED);
    
	prog = p;
}

/* Если индентификатор является переменной, то
 возвращается 1, иначе 0.
 */
int is_var(char *s)
{
    register int i;
    
    /* это локальная переменная ? */
    for(i=lvartos-1; i >= call_stack[functos-1]; i--)
        if(!strcmp(local_var_stack[i].var_name, token))
            return 1;
    
    /* если нет - поиск среди глобальных переменных */
    for(i=0; i < NUM_GLOBAL_VARS; i++)
        if(!strcmp(global_vars[i].var_name, s))
            return 1;
    
    return 0;
}

/* Присваивание переменной значения. */
void assign_var(char *var_name, int value)
{
    register int i;
    
    /* проверка наличия локальной переменной */
    for(i=lvartos-1; i >= call_stack[functos-1]; i--)  {
        if(!strcmp(local_var_stack[i].var_name, var_name)) {
            local_var_stack[i].value = value;
            return;
        }
    }
    if(i < call_stack[functos-1])
    /* если переменная нелокальная,
     ищем ее в таблице глобальных переменных */
        for(i=0; i < NUM_GLOBAL_VARS; i++)
            if(!strcmp(global_vars[i].var_name, var_name)) {
                global_vars[i].value = value;
                return;
            }
    sntx_err(NOT_VAR); /* переменная не найдена */
}

/* Присваивание переменной значения. */
void assign_arr_element(char *arr_name, int position, int value)
{
    register int i;
    
    /* проверка наличия локального массива */
    for(i=larrtos-1; i >= call_stack[functos-1]; i--)  {
        if(!strcmp(local_arr_stack[i].arr_name, arr_name)) {
            
            if (local_arr_stack[i].length <= position) {
                sntx_err(SYNTAX); // todo: index out of bounds exception
            }
            
            if (local_arr_stack[i].arr_type == INT) {
                int *tmpArrStart = local_arr_stack[i].int_arr;
                tmpArrStart = tmpArrStart + position;
                *tmpArrStart = value;
            } else if (local_arr_stack[i].arr_type == CHAR) {
                char *tmpArrStart = local_arr_stack[i].char_arr;
                tmpArrStart = tmpArrStart + position;
                *tmpArrStart = value;
            } else {
                printf("only arrays of type int or char are allowed. ");
                exit(1);
            }
            
            return;
        }
    }
    if(i < call_stack[functos-1])
    /* если переменная нелокальная,
     ищем ее в таблице глобальных переменных */
        for(i=0; i < NUM_GLOBAL_ARRAYS; i++)
            if(!strcmp(global_arrays[i].arr_name, arr_name)) {
                
                if (global_arrays[i].length <= position) {
                    sntx_err(SYNTAX); // todo: index out of bounds exception
                }
                
                if (global_arrays[i].arr_type == INT) {
                    int *tmpArrStart = global_arrays[i].int_arr;
                    tmpArrStart = tmpArrStart + position;
                    *tmpArrStart = value;
                } else if (global_arrays[i].arr_type == CHAR) {
                    char *tmpArrStart = global_arrays[i].char_arr;
                    tmpArrStart = tmpArrStart + position;
                    *tmpArrStart = value;
                } else {
                    printf("only arrays of type int or char are allowed. ");
                    exit(1);
                }
                return;
            }
    sntx_err(SYNTAX); /* todo: NOT_ARR */
}

int is_arr(char *s) {
    register int i;
    
    /* это локальный массив ? */
    for(i=larrtos-1; i >= call_stack[functos-1]; i--)
        if(!strcmp(local_arr_stack[i].arr_name, token))
            return 1;
    
    /* если нет - поиск среди глобальных переменных */
    for(i=0; i < NUM_GLOBAL_ARRAYS; i++)
        if(!strcmp(global_arrays[i].arr_name, s))
            return 1;
    
    return 0;
}

/* Объявление локальной переменной. */
void decl_local(void)
{
    struct var_type i;
    
    get_token();  /* определение типа */
    
    i.v_type = tok;
    i.value = 0;  /* инициализация нулем */
    
    do { /* обработка списка */
        get_token(); /* определение типа пременной */
        strcpy(i.var_name, token);
        local_push(i);
        get_token();
    } while(*token == ',');
    if(*token != ';') sntx_err(SEMI_EXPECTED);
}

/* Возврат из функции. */
void func_ret(void)
{
    int value;
    
    value = 0;
    /* получение возвращаемого значения, если оно есть */
    eval_exp(&value);
    
    ret_value = value;
}



/* Интерпритация одного оператора или блока. Когда
 interp_block() возвращает управление после первого
 вызова, в main() встретилась последняя
 закрывающаяся фигурная скобка или оператор return.
 */
void interp_block(void)
{
    int value;
    char block = 0;
    
    do {
        token_type = get_token();
        
        /* При интерпритации одного операторавозврат
         после первой точки с запятой.
         */
        
        /* определение типа лексемы */
        if(token_type == IDENTIFIER) {
            /* Это не зарегестрированное слово,
             обрабатывается выражение. */
            putback();  /* возврат лексемы во входной поток
                         для дальнейшей обработки функцией eval_exp() */
            eval_exp(&value);  /* обработка выражения */
            if(*token!=';') sntx_err(SEMI_EXPECTED);
        }
        else if(token_type==BLOCK) {
            /* если это граничитель блока */
            if(*token == '{') /* блок */
                block = 1; /* интерпритация блока, а не оператора */
            else return; /* это }, возврат */
        }
        else /* зарезервированное слово */
            switch(tok) {
                case CHAR:
                case INT:     /* объявление локальной переменной */
                    putback();
                    decl_local();
                    break;
                case RETURN:  /* возврат из вызова функции */
                    func_ret();
                    return;
                case IF:      /* обработка оператора if */
                    exec_if();
                    break;
                case ELSE:    /* обработка оператора else */
                    find_eob(); /* поиск конца блока else
                                 и продолжение выполнения */
                    break;
                case WHILE:   /* обработка цикла while */
                    exec_while();
                    break;
                case DO:      /* обработка цикла do-while */
                    exec_do();
                    break;
                case FOR:     /* обработка цикла for */
                    exec_for();
                    break;
                case END:
                    exit(0);
            }
    } while (tok != FINISHED && block);
}

/* Поиск конца блока. */
void find_eob(void)
{
    int brace;
    
    get_token();
    brace = 1;
    do {
        get_token();
        if(*token == '{') brace++;
        else if(*token == '}') brace--;
    } while(brace);
}

/* Вызов функции. */
void call(void)
{
    char *loc, *temp;
    int lvartemp;
    
    loc = find_func(token); /* найти точку входа функции */
    if(loc == NULL)
        sntx_err(FUNC_UNDEF); /* функция не определена */
    else {
        lvartemp = lvartos;  /* запоминание индекса стека
                              локальных переменных */
        get_args();  /* получение аргумента функции */
        temp = prog; /* запоминание адреса возврата */
        func_push(lvartemp);  /* запоминание индекса стека
                               локальных переменных */
        prog = loc;  /* переустановка prog в начало функции */
        get_params(); /* загрузка параметров функции
                       значениями аргументов */
        interp_block(); /* интерпретация функции */
        prog = temp; /* восстановление prog */
        lvartos = func_pop(); /* восстановление стека
                               локальных переменных */
    }
}



/* Заталкивание аргументов функций в стек
 локальных переменных. */
void get_args(void)
{
    int value, count, temp[NUM_PARAMS];
    struct var_type i;
    
    count = 0;
    get_token();
    if(*token != '(') sntx_err(PAREN_EXPECTED);
    
    /* обработка списка значений */
    do {
        // todo       eval_exp(&value);
        temp[count] = value;  /* временное запоминание */
        get_token();
        count++;
    }while(*token == ',');
    count--;
    /* затолкнуть в local_var_stack в обратном порядке */
    for(; count>=0; count--) {
        i.value = temp[count];
        i.v_type = ARG;
        local_push(i);
    }
}

/* Получение параметров функции. */
void get_params(void)
{
    struct var_type *p;
    int i;
    
    i = lvartos-1;
    do { /* обработка списка параметров */
        get_token();
        p = &local_var_stack[i];
        if(*token != ')' ) {
            if(tok != INT && tok != CHAR)
                sntx_err(TYPE_EXPECTED);
            
            p->v_type = token_type;
            get_token();
            
            /* связывание имени пераметров с аргументом,
             уже находящимся в стеке локальных переменных */
            strcpy(p->var_name, token);
            get_token();
            i--;
        }
        else break;
    } while(*token == ',');
    if(*token != ')') sntx_err(PAREN_EXPECTED);
}

/* Запись индекса в стек локальных переменных. */
void func_push(int i)
{
    if(functos>NUM_FUNC)
        sntx_err(NEST_FUNC);
    call_stack[functos] = i;
    functos++;
}

/* Выталкивание индекса в стеке локальных переменных. */
int func_pop(void)
{
    functos--;
    if(functos < 0) sntx_err(RET_NOCALL);
    return call_stack[functos];
}

/* Затолкнуть локальную переменную. */
void local_push(struct var_type i)
{
    if(lvartos > NUM_LOCAL_VARS)
        sntx_err(TOO_MANY_LVARS);
    
    local_var_stack[lvartos] = i;
    lvartos++;
}

/* Возврат адреса точки входа данной функции.
 Возврат NULL, если не надена.
 */
char *find_func(char *name)
{
    register int i;
    
    for(i=0; i < func_index; i++)
        if(!strcmp(name, func_table[i].func_name))
            return func_table[i].loc;
    
    return NULL;
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


// todo: syntx_error informative messages
void decl_global_array(void)
{
	get_token(); // type
	int arrtype = tok;
	global_arrays[garr_index].arr_type = arrtype;
    
    get_token(); // name
	strcpy(global_arrays[garr_index].arr_name, token);
    
	get_token(); // [

	if (*token != '[') sntx_err(SYNTAX);
    
	// important! comma separated definition of arrays are not supported
	// user must define length of an array in definition
	get_token();
	if (token_type != NUMBER) sntx_err(SYNTAX);
	int arr_length = atoi(token);
    
	get_token(); // ]
	if (*token != ']') sntx_err(SYNTAX);
    
    global_arrays[garr_index].length = arr_length;
	
    // don't be dependent on compiler
	if(arrtype == INT) {
		global_arrays[garr_index].int_arr = (int *)malloc(sizeof(int) * arr_length);
	} else if (arrtype == CHAR) {
		global_arrays[garr_index].char_arr = (char *)malloc(sizeof(char) * arr_length);
	}
    
	get_token(); // ;
	if(*token != ';') sntx_err(SEMI_EXPECTED);
	garr_index++;
}

/* Объявление глобальной переменной. */
void decl_global(void)
{
	int vartype;
    
	get_token();  /* определение типа */
    
	vartype = tok; /* запоминание типа переменной */
    
	do {  /* обработка списка */
		global_vars[gvar_index].v_type = vartype;
		global_vars[gvar_index].value = 0;  /* инициализация нулем */
		get_token();  /* определение имени в *token */
		strcpy(global_vars[gvar_index].var_name, token);
		get_token();
		gvar_index++;
	} while(*token == ',');
    
	if(*token != ';') sntx_err(SEMI_EXPECTED);
}

/* получение значения из массива */
int find_arr_element(char *arr_name, int position) {
    register int i;
    
    /* проверка наличия переменной */
    for(i=larrtos-1; i >= call_stack[functos-1]; i--) {
        if(!strcmp(local_arr_stack[i].arr_name, token)) {
            
            if (local_arr_stack[i].length <= position) {
                sntx_err(SYNTAX); // todo: index out of bounds exception
            }
            if (local_arr_stack[i].arr_type == INT) {
                int *tmpArrStart = local_arr_stack[i].int_arr;
                tmpArrStart = tmpArrStart + position;
                return *tmpArrStart;
            } else if (local_arr_stack[i].arr_type == CHAR) {
                char *tmpArrStart = local_arr_stack[i].char_arr;
                tmpArrStart = tmpArrStart + position;
                return *tmpArrStart;
            } else {
                printf("only arrays of type int or char are allowed. ");
                exit(1);
            }

        }
    }
    
    if(i < call_stack[functos-1])
    /* если переменная нелокальная,
     ищем ее в таблице глобальных переменных */
        for(i=0; i < NUM_GLOBAL_ARRAYS; i++)
            if(!strcmp(global_arrays[i].arr_name, arr_name)) {
                
                if (global_arrays[i].length <= position) {
                    sntx_err(SYNTAX); // todo: index out of bounds exception
                }
                
                if (global_arrays[i].arr_type == INT) {
                    int *tmpArrStart = global_arrays[i].int_arr;
                    tmpArrStart = tmpArrStart + position;
                    return *tmpArrStart;
                } else if (global_arrays[i].arr_type == CHAR) {
                    char *tmpArrStart = global_arrays[i].char_arr;
                    tmpArrStart = tmpArrStart + position;
                    return *tmpArrStart;
                } else {
                    printf("only arrays of type int or char are allowed. ");
                    exit(1);
                }
            }
    sntx_err(SYNTAX); /* todo: NOT_ARR */
    return -1;
}

int arr_exists(char *name) {
    register int i;
    /* проверка наличия переменной */
    for(i=larrtos-1; i >= call_stack[functos-1]; i--) {
        if(!strcmp(local_arr_stack[i].arr_name, name)) {
            return 1;
        }
    }
    for(i=0; i < NUM_GLOBAL_ARRAYS; i++) {
        if(!strcmp(global_arrays[i].arr_name, name)) {
            return 1;
        }
    }
    return 0;
}

void free_arr() {
    register int i;
    for(i=0; i < NUM_GLOBAL_ARRAYS; i++) {
        free(global_arrays[i].int_arr);
        free(global_arrays[i].char_arr);
    }
}


/* Получение значения переменной. */
int find_var(char *s)
{
    register int i;
    
    /* проверка наличия переменной */
    for(i=lvartos-1; i >= call_stack[functos-1]; i--)
        if(!strcmp(local_var_stack[i].var_name, token))
            return local_var_stack[i].value;
    
    /* в противном случае проверим,
     может быть это глобальная переменная */
    for(i=0; i < NUM_GLOBAL_VARS; i++)
        if(!strcmp(global_vars[i].var_name, s))
            return global_vars[i].value;
    
    sntx_err(NOT_VAR); /* переменная не найдена */
    return -1; 
}

// {{ program flow functions
/* Выполнение оператора if. */
void exec_if(void)
{
    int cond;
    
    eval_exp(&cond); /* вычисление if-выражения */
    
    if(cond) { /* истина - интерпретация if-предложения */
        interp_block();
    }
    else { /* в противном случае пропуск if-предложения
            и выполнение else-предложения, если оно есть */
        find_eob(); /* поиск конца блока */
        get_token();
        
        if(tok != ELSE) {
            putback();  /* восстановление лексемы,
                         если else-предложение отсутсвует */
            return;
        }
        interp_block();
    }
}

/* Выполнение цикла while. */
void exec_while(void)
{
    int cond;
    char *temp;
    
    putback();
    temp = prog;  /* запоминание адреса начала цикла while */
    get_token();
    eval_exp(&cond);  /* вычисление управляющего выражения */
    if(cond) interp_block();  /* если оно истинно, то выполнить
                               интерпритацию */
    else {  /* в противном случае цикл пропускается */
        find_eob();
        return;
    }
    prog = temp;  /* возврат к началу цикла */
}

/* Выполнение цикла do. */
void exec_do(void)
{
    int cond;
    char *temp;
    
    putback();
    temp = prog;  /* запоминание адреса начала цикла */
    
    get_token(); /* найти начало цикла */
    interp_block(); /* интерпритация цикла */
    get_token();
    if(tok != WHILE) sntx_err(WHILE_EXPECTED);
    eval_exp(&cond); /* проверка условия цикла */
    if(cond) prog = temp; /* если условие истинно,
                           то цикл выполняется, в противном случае происходит
                           выход из цикла */
}
/* Выполнение цикла for. */
void exec_for(void)
{
    int cond;
    char *temp, *temp2;
    int brace ;
    
    get_token();
    eval_exp(&cond);  /* инициализирующее выражение */
    if(*token != ';') sntx_err(SEMI_EXPECTED);
    prog++; /* пропуск ; */
    temp = prog;
    for(;;) {
        eval_exp(&cond);  /* проверка условия */
        if(*token != ';') sntx_err(SEMI_EXPECTED);
        prog++; /* пропуск ; */
        temp2 = prog;
        
        /* поиск начала тела цикла */
        brace = 1;
        while(brace) {
            get_token();
            if(*token == '(') brace++;
            if(*token == ')') brace--;
        }
        
        if(cond) interp_block();  /* если условие выполнено,
                                   то выполнить интерпретацию */
        else {  /* в противном случае обойти цикл */
            find_eob();
            return;
        }
        prog = temp2;
        eval_exp(&cond); /* выполнение инкремента */
        prog = temp;  /* возврат в начало цикла */
    }
}


// }}
