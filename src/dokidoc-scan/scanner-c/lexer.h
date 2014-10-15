/* lexer.h -
 */

#ifndef _LEXER_H_
#define _LEXER_H_

#include "base.h"



typedef struct _Lexer Lexer;



Lexer *lexer_new ( const gchar *filename );
Token *lexer_lex ( Lexer *lexer,
                   DokLocation *loc );
Token *lexer_lex_include ( Lexer *lexer,
                           DokLocation *loc );



#endif
