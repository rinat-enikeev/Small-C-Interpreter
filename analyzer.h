#ifndef ANALYZER_H
#define ANALYZER_H
/* Функции в анализаторе */
/* Используемые другими модулями */
int get_token(void);
void sntx_err(int error);
void putback(void);
void eval_exp(int *value);
/* Внутренние */
void eval_exp0(int *value); // =
void eval_exp1(int *value); // < > <= >=
void eval_exp2(int *value); // + -
void eval_exp3(int *value); // * /
void eval_exp4(int *value); // унарный + -
void eval_exp5(int *value); // ()
void atom(int *value);      // terminal
int look_up(char *s);
int isdelim(char c);
int iswhite(char c);
int internal_func(char *s);
#endif //ANALYZER_H
