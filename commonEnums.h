#ifndef COMMON_ENUMS_DECL_H
#define COMMON_ENUMS_DECL_H

/**********************************************************************/
/* Типы лексем */
enum tok_types {DELIMITER,  // знаки пунктуации и операторы
				IDENTIFIER, // имена переменных и функций
				NUMBER,     // числовая константа
				KEYWORD,    // зарезервированные слова (while f.e.)
                TEMP,		// вспомогательный тип токена для определения
							// является токен KEYWORD или IDENTIFIER
				STRING,		// строка
				BLOCK};		// { или }
				

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


#endif // COMMON_ENUMS_DECL_H