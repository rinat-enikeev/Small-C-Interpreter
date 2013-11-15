#ifndef EXTERN_VAR_DECL_H
#include "externVars.h"
#endif

#ifndef COMMON_ENUMS_DECL_H
#include "commonEnums.h"
#endif

#ifndef RESTRICTIONS_DECL_H
#include "restrictions.h"
#endif


char token_type; /* ÒÓ‰ÂÊËÚ ÚËÔ ÎÂÍÒÂÏ˚ */
char tok;		/* ‚ÌÛÚÂÌÌÂÂ ÔÂ‰ÒÚ‡‚ÎÂÌËÂ ÎÂÍÒÂÏ˚ */
char *prog;    /* текущая позиция в исходном тексте программы */
char *p_buf;   /* указывает на начало буфера программы */
char token[80];	/* ÒÚÓÍÓ‚ÓÂ ÔÂ‰ÒÚ‡‚ÎÂÌËÂ ÎÂÍÒÂÏ˚ */
jmp_buf e_buf; /* содержит информацию для longjmp() */

int functos;  /* индекс вершины стека вызова функции */
int func_index; /* индекс в таблице функций */
int gvar_index; /* индекс в таблице глобальных переменных */
int garr_index; /* index of global arrays */
int lvartos; /* индекс в стеке локальных переменных */
int larrtos; /* индекс в стеке локальных переменных */

struct commands { /* Ú‡·ÎËˆ‡ Á‡ÂÁÂ‚ËÓ‚‡ÌÌ˚ı ÒÎÓ‚ */
    char command[20];
    char tok;
} table[] = { /* ¬ ˝ÚÛ Ú‡·ÎËˆÛ */
    "if", IF, /* ÍÓÏ‡Ì‰˚ ‰ÓÎÊÌ˚ ·˚Ú¸ ‚‚Â‰ÂÌ˚ Ì‡ ÌËÊÌÂÏ Â„ËÒÚÂ. */
    "else", ELSE,
    "for", FOR,
    "do", DO,
    "while", WHILE,
    "char", CHAR,
    "int", INT,
    "return", RETURN,
    "end", END,
    "", END  /* ÍÓÌÂˆ Ú‡·ÎËˆ˚ */
};

struct func_type {
    char func_name[ID_LEN];
    int ret_type;
    char *loc;  /* ‡‰ÂÒ ÚÓ˜ÍË ‚ıÓ‰‡ ‚ Ù‡ÈÎ */
} func_table[NUM_FUNC];

int call_stack[NUM_FUNC];

/* Ã‡ÒÒË‚ ˝ÚËı ÒÚÛÍÚÛ ÒÓ‰ÂÊËÚ ËÌÙÓÏ‡ˆË˛
 Ó „ÎÓ·‡Î¸Ì˚ı ÔÂÂÏÂÌÌ˚ı.
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
    void *value;
} global_arrays[NUM_GLOBAL_ARRAYS];
// added array functionality *}}

struct array_type local_arr_stack[NUM_LOCAL_ARRS];



void decl_global(void), sntx_err(int error), putback(void), decl_global_array(void);
int get_token(void);
int load_program(char *p, char *fname);
char *find_func(char *name);
void call(void);
void get_params(void), get_args(void), func_push(int i);
int func_pop(void);
void func_ret(void);
void local_push(struct var_type i);


/* Õ‡ÈÚË ‡‰ÂÒ‡ ‚ÒÂı ÙÛÌÍˆËÈ ‚ ÔÓ„‡ÏÏÂ
 Ë Á‡ÔÓÏÌËÚ¸ „ÎÓ·‡Î¸Ì˚Â ÔÂÂÏÂÌÌ˚Â. */
void prescan(void)
{
	char *p, *tp;
	char temp[32];
	int datatype;
	int brace = 0;  /* ≈ÒÎË brace = 0, ÚÓ ÚÂÍÛ˘‡ˇ
                     ÔÓÁËˆËˇ ÛÍ‡Á‡ÚÂÎˇ ÔÓ„‡ÏÏ˚ Ì‡ıÓ‰ËÚÒˇ
                     ‚ ÌÂ Í‡ÍÓÈ-ÎË·Ó ÙÛÌÍˆËË. */
    
	p = prog;
	func_index = 0;
	do {
		while(brace) {  /* Ó·ıÓ‰ ÍÓ‰‡ ÙÛÌÍˆËË */
			get_token();
			if(*token == '{') brace++;
			if(*token == '}') brace--;
		}
        
		tp = prog; /* Á‡ÔÓÏËÌ‡ÌËÂ ÚÂÍÛ˘ÂÈ ÔÓÁËˆËË */
		get_token();
        
		/* ÚËÔ „ÎÓ·‡Î¸ÌÓÈ ÔÂÂÏÂÌÌÓÈ ËÎË ‚ÓÁ‚‡˘‡ÂÏÓ„Ó ÁÌ‡˜ÂÌËˇ ÙÛÌÍˆËË */
		if(tok==CHAR || tok==INT) {
			datatype = tok; /* Á‡ÔÓÏËÌ‡ÌËÂ ÚËÔ‡ ‰‡ÌÌ˚ı */
            
			get_token();
			if(token_type == IDENTIFIER) {
				strcpy(temp, token);
				get_token();
                if (token_type == ARRAY) {
                    prog = tp;
                    decl_global_array();
                } else if(*token != '(') { /* ˝ÚÓ ‰ÓÎÊÌ‡ ·˚Ú¸ „ÎÓ·‡Î¸Ì‡ˇ ÔÂÂÏÂÌÌ‡ˇ */
					prog = tp; /* ‚ÓÁ‚‡Ú ‚ Ì‡˜‡ÎÓ Ó·˙ˇ‚ÎÂÌËˇ */
					decl_global();
				} else if(*token == '(') {  /* ˝ÚÓ ‰ÓÎÊÌ‡ ·˚Ú¸ ÙÛÌÍˆËˇ */
					func_table[func_index].loc = prog;
					func_table[func_index].ret_type = datatype;
					strcpy(func_table[func_index].func_name, temp);
					func_index++;
					while(*prog != ')') prog++;
					prog++;
					/* ÒÂÈ˜‡Ò prog ÛÍ‡Á˚‚‡ÂÚ Ì‡ ÓÚÍ˚‚‡˛˘Û˛Òˇ
                     ÙË„ÛÌÛ˛ ÒÍÓ·ÍÛ ÙÛÌÍˆËË */
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
        // todo        interp_block(); /* интерпретация функции */
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
#if DEBUG
	cout << "Declaring global array: " << endl;
#endif
    
	get_token(); // type
	int arrtype = tok;
	global_arrays[garr_index].arr_type = arrtype;
    
    get_token(); // name
#if DEBUG
	assert(token_type == IDENTIFIER);
    cout << token << endl;
#endif
	strcpy(global_arrays[garr_index].arr_name, token);
    
	get_token(); // [
#if DEBUG
    assert(token_type == ARRAY);
	cout << token << endl;
#endif
	if (*token != '[') sntx_err(SYNTAX);
    
	// important! comma separated definition of arrays are not supported
	// user must define length of an array in definition
	get_token();
	if (token_type != NUMBER) sntx_err(SYNTAX);
	int arr_length = tok;
    
	get_token(); // ]
	if (*token != ']') sntx_err(SYNTAX);
    
	// don't be dependent on compiler
	if(arrtype == INT) {
		global_arrays[garr_index].value = malloc(sizeof(int) * arr_length);
	} else if (arrtype == CHAR) {
		global_arrays[garr_index].value = malloc(sizeof(char) * arr_length);
	}
    
	get_token(); // ;
	if(*token != ';') sntx_err(SEMI_EXPECTED);
	garr_index++;
}

/* Œ·˙ˇ‚ÎÂÌËÂ „ÎÓ·‡Î¸ÌÓÈ ÔÂÂÏÂÌÌÓÈ. */
void decl_global(void)
{
	int vartype;
    
	get_token();  /* ÓÔÂ‰ÂÎÂÌËÂ ÚËÔ‡ */
    
	vartype = tok; /* Á‡ÔÓÏËÌ‡ÌËÂ ÚËÔ‡ ÔÂÂÏÂÌÌÓÈ */
    
	do { /* Ó·‡·ÓÚÍ‡ ÒÔËÒÍ‡ */
		global_vars[gvar_index].v_type = vartype;
		global_vars[gvar_index].value = 0;  /* ËÌËˆË‡ÎËÁ‡ˆËˇ ÌÛÎÂÏ */
		get_token();  /* ÓÔÂ‰ÂÎÂÌËÂ ËÏÂÌË */
		strcpy(global_vars[gvar_index].var_name, token);
		get_token();
		gvar_index++;
	} while(*token == ',');
    
	if(*token != ';') sntx_err(SEMI_EXPECTED);
}
