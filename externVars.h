#ifndef EXTERN_VAR_DECL_H
#define EXTERN_VAR_DECL_H

#include <setjmp.h>

extern char token_type; /* ÒÓ‰ÂÊËÚ ÚËÔ ÎÂÍÒÂÏ˚ */
extern char tok;		/* ‚ÌÛÚÂÌÌÂÂ ÔÂ‰ÒÚ‡‚ÎÂÌËÂ ÎÂÍÒÂÏ˚ */
extern char *prog;		/* ÚÂÍÛ˘ÂÂ ÔÓÎÓÊÂÌËÂ ‚ ËÒıÓ‰ÌÓÏ ÚÂÍÒÚÂ ÔÓ„‡ÏÏ˚ */
extern char token[80];	/* ÒÚÓÍÓ‚ÓÂ ÔÂ‰ÒÚ‡‚ÎÂÌËÂ ÎÂÍÒÂÏ˚ */

extern char *p_buf;		/* ÛÍ‡Á‡ÚÂÎ¸ Ì‡ Ì‡˜‡ÎÓ ·ÛÙÂ‡ ÔÓ„‡ÏÏ˚ */
extern jmp_buf e_buf;   /* ÒÓ‰ÂÊËÚ ‰‡ÌÌ˚Â ‰Îˇ longjmp() */

extern int gvar_index;
extern int garr_index;
extern int lvartos; /* индекс в стеке локальных переменных */
extern int larrtos; /* индекс в стеке локальных переменных */
extern int functos;


#endif //EXTERN_VAR_DECL_H