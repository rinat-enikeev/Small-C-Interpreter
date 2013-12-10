#ifndef INTERPRETER_H
#define INTERPRETER_H
/* Функции и переменные в интерпретаторе */
struct var_type;
struct commands {
	char command[20];
	char tok;
};
extern struct commands table[];
void decl_local_array(void);
void decl_global_array(void);
void decl_global_var(void);
int load_program(char *p, char *fname);
char *find_func(char *name);
void call(void);
void get_params(void);
void get_args(void);
void func_push(int i);
int  func_pop(void);
void func_ret(void);
void local_var_push(struct var_type i);
void free_arr(void);
void exec_if(void);
void find_eob(void);
void exec_while(void);
void exec_do(void);
void exec_for(void);
void prescan(void);
int is_var(char *s);
int is_arr(char *s);
void assign_var(char *var_name, int value);
void assign_arr_element(char *arr_name, int position, int value);
int arr_exists(char *name);
int find_arr_element(char *arr_name, int position);
int find_var(char *s);
#endif //INTERPRETER_H
